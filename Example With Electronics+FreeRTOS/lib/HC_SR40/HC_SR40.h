/* #ifndef __HC_SR40_H
#define __HC_SR40_H



extern uint8_t HC_Sr40_Signal_SendFlag;


void HC_SR04_Init(uint32_t Distance_Detect,uint32_t Distance_Danger);
void HC_SR04_SendRequest_Start(void);
void HC_SR04_SendRequest_End(void);

uint32_t HC_SR04_GetData(void);



#endif
 */