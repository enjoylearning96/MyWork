#include "stm32f1xx_hal.h"
#include "main.h"
#include "OLED.h"
#include "tim.h"
#include "MPU6050.h"

#define Control_Errornum 100

int16_t Control_ErrorX[Control_Errornum]={0};
int16_t Kp,Ki,Kd;
uint8_t Control_State=0; //转动方向：0，正转，1，反转2,制动
int16_t Control_Power=0;
int16_t Control_Target=0;
uint8_t Speed_Level0=0;
uint8_t Speed_Level1=30;
uint8_t Speed_Level2=60;
uint8_t Speed_Level3=90;



void Control_PID_Init(int16_t Kp,int16_t Ki,int16_t Kd)
{
    Kp=Kp;
    Ki=Ki;
    Kd=Kd;
}

/**
  * @brief  电机转动方向控制
  * @param  Wheel 目标轮：0：左轮1.右轮
  * @param  Direction 转动方向：0，正转，1，反转2,制动
  * @retval 无
  */
void Control_Run(uint8_t Wheel,uint8_t Direction)
{
    uint16_t Control1_Pin,Control2_Pin;
    GPIO_TypeDef *Control1_Port;
    GPIO_TypeDef *Control2_Port;
    if (Wheel==0)
    {
        Control1_Pin=Left_Back_Control_1_Pin;
        Control2_Pin=Left_Back_Control_2_Pin;
        Control1_Port=Left_Back_Control_1_GPIO_Port;
        Control2_Port=Left_Back_Control_2_GPIO_Port;
    }
    else if (Wheel==1)
    {
        Control1_Pin=Right_Back_Control_1_Pin;
        Control2_Pin=Right_Back_Control_2_Pin;
        Control1_Port=Right_Back_Control_1_GPIO_Port;
        Control2_Port=Right_Back_Control_2_GPIO_Port;
    }

    if (Direction==0)
    {
        HAL_GPIO_WritePin(Control1_Port, Control1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(Control2_Port, Control2_Pin,GPIO_PIN_RESET);
        Control_State=0;
    }
    else if (Direction==1)
    {
        HAL_GPIO_WritePin(Control1_Port, Control1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Control2_Port, Control2_Pin,GPIO_PIN_SET);
        Control_State=1;
    }
    else if (Direction==2)
    {
        HAL_GPIO_WritePin(Control1_Port, Control1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(Control2_Port, Control2_Pin,GPIO_PIN_SET);
        Control_State=2;
    }  
}

/**
  * @brief  电机转动速度PD控制
  * @param  误差值数组,正负代表位于哪一侧，
  * @param  i 当前位置
  * @retval 无
  */
void PD_Control(void)
{
    Control_Power=((AX-Control_Target)*Kp/5000)+(GY*Kd/5000);
}

/**
  * @brief  电机转动速度控制
  * @param  TIMx 目标定时器
  * @param  Compare CCR设定值
  * @param  channel CCR设定通道
  * @retval 无
  */
void Control_SetSpeed(TIM_TypeDef* TIMx, uint16_t Compare,uint8_t channel)
{
    if (channel==1)
    {
        TIMx->CCR1 = Compare;
    }
    else if (channel==2)
    {
        TIMx->CCR2 = Compare;
    }
    else if (channel==3)
    {
        TIMx->CCR3 = Compare;
    }
    else if (channel==4)
    {
        TIMx->CCR4 = Compare;
    }
}

/**
  * @brief  控制器控制小车
  * @param  TIMx 目标定时器
  * @param  Channel_Left 左轮对应通道
  * @param  Channel_Right 右轮对应通道
  * @param  Signal 存储收到的控制信号的数组Signal[0]：0，无动作；1，前进；2，后退；3，停止；Signal[1]：0，无动作；1，左；2，右；3，加速；4，减速
  * @retval 无
  */
 static uint8_t Speed=0;//记录速度
void Control_WithControllor(TIM_TypeDef* TIMx,uint8_t Channel_Left,uint8_t Channel_Right,uint8_t *Signal)
{
    
    if (Signal[0]==0)
    {
    }   
    else if (Signal[0]==1)
    {
        Control_Run(0,0);
        Control_Run(1,0);
        Control_SetSpeed(TIMx, Speed, Channel_Left);
        Control_SetSpeed(TIMx, Speed, Channel_Right);            
    }
    else if (Signal[0]==2)
    {
        Control_Run(0,1);
        Control_Run(1,1);
        Control_SetSpeed(TIMx, Speed, Channel_Left);
        Control_SetSpeed(TIMx, Speed, Channel_Right);  
    }
    else if (Signal[0]==3)
    {
        Control_Run(0,2);
        Control_Run(1,2); 
    }
    
    


    if (Signal[1]==0)
    {
        /* code */
    }
    else if (Signal[1] == 1)
    {
        if (Signal[0] == 0)
        {
            Control_SetSpeed(TIMx, 0, Channel_Left);// 左轮锁死
            Control_SetSpeed(TIMx, Speed, Channel_Right);
        }
        else
        {       
            Control_SetSpeed(TIMx, Speed - 30, Channel_Left); // 左轮速度降低
            Control_SetSpeed(TIMx, Speed, Channel_Right);
        }
    }
    else if (Signal[1] == 2)
    {
        if (Signal[0] == 0)
        {
            Control_SetSpeed(TIMx, 0, Channel_Right);// 右轮锁死
            Control_SetSpeed(TIMx, Speed, Channel_Left);
        }
        else
        {       
            Control_SetSpeed(TIMx, Speed - 30, Channel_Right); // 右轮速度降低
            Control_SetSpeed(TIMx, Speed, Channel_Left);
        }
    }
    else if (Signal[1] == 3)
    {
        if (Speed == 0)
        {
            Control_SetSpeed(TIMx, Speed_Level1, Channel_Left);
            Control_SetSpeed(TIMx, Speed_Level1, Channel_Right);
            Speed = Speed_Level1;
        }
        
        else if (Speed == Speed_Level1)
        {
            Control_SetSpeed(TIMx, Speed_Level2, Channel_Left);
            Control_SetSpeed(TIMx, Speed_Level2, Channel_Right);
            Speed = Speed_Level2;
        }
        else if (Speed == Speed_Level2)
        {
            Control_SetSpeed(TIMx, Speed_Level3, Channel_Left);
            Control_SetSpeed(TIMx, Speed_Level3, Channel_Right);
            Speed = Speed_Level3;
        }
        else if (Speed == Speed_Level3)
        {
            Control_SetSpeed(TIMx, Speed_Level3, Channel_Left);
            Control_SetSpeed(TIMx, Speed_Level3, Channel_Right);
            Speed = Speed_Level3;
        }
    }
    else if (Signal[1] == 4)
    {
        if (Speed == Speed_Level0)
        {
            Control_SetSpeed(TIMx, Speed_Level0, Channel_Left);
            Control_SetSpeed(TIMx,Speed_Level0, Channel_Right);
            Speed = Speed_Level0;
        }
        
        else if (Speed == Speed_Level1)
        {
            Control_SetSpeed(TIMx, Speed_Level0, Channel_Left);
            Control_SetSpeed(TIMx,Speed_Level0, Channel_Right);
            Speed = Speed_Level0;
        }
        else if (Speed == Speed_Level2)
        {
            Control_SetSpeed(TIMx, Speed_Level1, Channel_Left);
            Control_SetSpeed(TIMx, Speed_Level1, Channel_Right);
            Speed = Speed_Level1;
        }
        else if (Speed == Speed_Level3)
        {
            Control_SetSpeed(TIMx, Speed_Level2, Channel_Left);
            Control_SetSpeed(TIMx, Speed_Level2, Channel_Right);
            Speed = Speed_Level2;
        }
    }
}
