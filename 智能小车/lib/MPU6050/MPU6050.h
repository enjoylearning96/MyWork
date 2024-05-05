#ifndef __MPU6050_H
#define __MPU6050_H

extern int16_t AX,AY,AZ,TEMP,GX,GY,GZ;

void MPU6050_Init(void);
void MPU6050_GetData(void);


#endif

