#ifndef __CONTROL_H
#define __CONTROL_H

extern int16_t *Control_ErrorX;
extern uint8_t Control_State;
extern int16_t Control_Power;
extern int16_t Kp,Ki,Kd;
extern int16_t Control_Target;

void Control_PID_Init(int16_t Kp,int16_t Ki,int16_t Kd);
void Control_Run(uint8_t Wheel,uint8_t Direction);
void PD_Control(void);
void Control_SetSpeed(TIM_TypeDef* TIMx, uint16_t Compare,uint8_t channel);
void Control_WithControllor(TIM_TypeDef* TIMx,uint8_t Channel_Left,uint8_t Channel_Right,uint8_t *Signal);


#endif

