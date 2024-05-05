#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "MPU6050_Reg.h"
#include "OLED.h"
#include "Control.h"

#define MPU6050_ADDRESS		0xD0		//MPU6050的I2C从机地址

uint8_t MPU6050_Data[15]={0};

uint8_t MPU6050_PWR_MGMT_1_SET=0x01;
uint8_t MPU6050_PWR_MGMT_2_SET=0x00;
uint8_t MPU6050_Samplate =0x09;
uint8_t MPU6050_Con=0x06;
uint8_t MPU6050_GYPO_Con=0x18;//0001 1000不自检，最大量程11->3
uint8_t MPU6050_ACCEL_Con=0x18;

int16_t Data_Buff=0x0000;
int16_t AX,AY,AZ,TEMP,GX,GY,GZ;

void MPU6050_Init(void)
{
    //MemAddSize=1|2,对应地址大小为8位和16位
    HAL_I2C_Mem_Write_DMA(&hi2c2,MPU6050_ADDRESS,MPU6050_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT,&MPU6050_PWR_MGMT_1_SET,1);//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
    //完成后未采集到后续数据，疑似通信速度过快,已解决，等待BUSY置0；内存地址应自增，外设不用，因为就一个字节
    while (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_BUSY)==1)
    {
        /* code */
    }     
    HAL_I2C_Mem_Write_DMA(&hi2c2,MPU6050_ADDRESS,MPU6050_PWR_MGMT_2, I2C_MEMADD_SIZE_8BIT,&MPU6050_PWR_MGMT_2_SET,1);//电源管理寄存器2，保持默认值0，所有轴均不待机
    while (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_BUSY)==1)
    {
        /* code */
    }     
    HAL_I2C_Mem_Write_DMA(&hi2c2,MPU6050_ADDRESS,MPU6050_SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT,&MPU6050_Samplate,1);
   while (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_BUSY)==1)
    {
        /* code */
    }     
    //采样率分频寄存器，配置采样率,Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV),陀螺仪输出率Gyroscope Output Rate具体见说明书寄存器25 
    HAL_I2C_Mem_Write_DMA(&hi2c2,MPU6050_ADDRESS,MPU6050_CONFIG, I2C_MEMADD_SIZE_8BIT,&MPU6050_Con,1);//配置寄存器，配置DLPF
    while (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_BUSY)==1)
    {
        /* code */
    }     
    HAL_I2C_Mem_Write_DMA(&hi2c2,MPU6050_ADDRESS,MPU6050_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT,&MPU6050_GYPO_Con,1);//陀螺仪配置寄存器，选择满量程为±2000°/s
    HAL_Delay(1);
    while (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_BUSY)==1)
    {
        /* code */
    }
    HAL_I2C_Mem_Write_DMA(&hi2c2,MPU6050_ADDRESS,MPU6050_ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT,&MPU6050_ACCEL_Con,1);//加速度计配置寄存器，选择满量程为±16g   
    HAL_Delay(1);
    while (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_BUSY)==1)
    {
        /* code */
    }
}

void Data_GYPO_Conversion(int16_t Data_Measure)
{
    int16_t range=0x0;
    Data_Buff=Data_Measure;
    if (MPU6050_GYPO_Con==0x00)
    {
        range=0xfa;
    }
    else if (MPU6050_GYPO_Con==0x08)
    {
        range=0x1f4;
    }
    else if (MPU6050_GYPO_Con==0x10)
    {
        range=0x3e8;
    }
    else if (MPU6050_GYPO_Con==0x18)
    {
        range=0x7d0;
    }  
    Data_Buff=((Data_Buff&(0x7fff))*range/0x7fff);//负数符号位为1
    if (Data_Measure<0)
    {
        Data_Buff=(Data_Buff)|(0x80000);
    }     
}

void Data_ACCEL_Conversion(int16_t Data_Measure)
{
    int16_t Divisor=0x0;
    Data_Buff=Data_Measure;
    if (MPU6050_ACCEL_Con==0x00)
    {
        Divisor=0x4000/0xa;
    }
    else if (MPU6050_ACCEL_Con==0x08)
    {
        Divisor=0x333;
    }
    else if (MPU6050_ACCEL_Con==0x10)
    {
        Divisor=0x1000/0xa;
    }
    else if (MPU6050_ACCEL_Con==0x18)
    {
        Divisor=0xcc;
    }  
    Data_Buff=(((Data_Buff)&(0x7fff))/Divisor);//负数符号位为1,注意应先乘后除，因为小数点后面的数会丢失
    if (Data_Measure<0)
    {
        Data_Buff=(Data_Buff)|(0x80000);
    }     
}

void MPU6050_GetData(void)
{
    //每个数据分为低八位和高八位
   HAL_I2C_Mem_Read_DMA(&hi2c2,MPU6050_ADDRESS,MPU6050_ACCEL_XOUT_H,I2C_MEMADD_SIZE_8BIT,MPU6050_Data,14);
   AX=(MPU6050_Data[0]<<8)|MPU6050_Data[1];
   AY=(MPU6050_Data[2]<<8)|MPU6050_Data[3];
   AZ=(MPU6050_Data[4]<<8)|MPU6050_Data[5];
   TEMP=(MPU6050_Data[6]<<8)|MPU6050_Data[7];
   GX=(MPU6050_Data[8]<<8)|MPU6050_Data[9];
   GY=(MPU6050_Data[10]<<8)|MPU6050_Data[11];
   GZ=(MPU6050_Data[12]<<8)|MPU6050_Data[13];
   //OLED_ShowSignedNum(1,1,AX-Control_Target,5);
   //OLED_ShowSignedNum(1,8,GX,5);
//    OLED_ShowSignedNum(2,1,AY,5);
   //OLED_ShowSignedNum(2,8,GY,5);
//    OLED_ShowSignedNum(3,1,AZ,5);
//    OLED_ShowSignedNum(3,8,GZ,5);
//    OLED_ShowSignedNum(4,1,TEMP,5);//-40-85℃
//     Data_GYPO_Conversion(GZ);
    //Data_ACCEL_Conversion(AZ);//模式寄存器检测+Z轴校正
   //OLED_ShowSignedNum(4,8,Data_Buff,5);
}

