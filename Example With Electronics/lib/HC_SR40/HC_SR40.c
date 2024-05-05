// #include "tim.h"
// #include "OLED.h"
// #include "HC_SR40.h"

// uint32_t HC_SR40_Value=0;//测距结果，单位mm
// uint32_t Time_Detect=0;
// uint32_t Time_Danger=0;
// uint32_t Fc=72000000/4/60;//计次标准频率fc
// uint8_t HC_Sr40_Signal_SendFlag=0;

// uint32_t Distance_Trans_Time(uint8_t Distance);

// /**
//   * @brief  HC_SR40初始化，设置探测距离和危险距离
//   * @param  
//   * @retval 无
//   */
// void HC_SR04_Init(uint32_t Distance_Detect,uint32_t Distance_Danger)
// {
//   Time_Detect=Distance_Trans_Time(Distance_Detect);
//   Time_Danger=Distance_Trans_Time(Distance_Danger);
// }

// /**
//   * @brief  将距离转化为高电平持续时间,输出结果为微秒
//   * @param  Distance 待转化距离
//   * @retval 高电平持续时间
//   */
// uint32_t Distance_Trans_Time(uint8_t Distance)
// {
//   uint32_t Time=0;
//   Time=Distance*2*1000/340;
//   return Time;
// }

// /**
//   * @brief  将高电平持续时间转化为距离,输出结果为毫米
//   * @param  Time 待转化时间，单位微秒us
//   * @retval 距离
//   */
// uint8_t Time_Trans_Distance(uint32_t Time)
// {
//   uint8_t Distance=Time*340/2/1000;
//   return Distance;
// }

// /**
//   * @brief  开始发送请求测量信号，要求至少10微秒;因为在中断中无法使用库函数延时，通过置标志位，在主循环检测并触发等待的方法完成
//   * @param  
//   * @retval 无
//   */
// void HC_SR04_SendRequest_Start(void)
// {
//     HAL_GPIO_WritePin(HC_SR04_SignalOutput_GPIO_Port,HC_SR04_SignalOutput_Pin,GPIO_PIN_SET);
//     HC_Sr40_Signal_SendFlag=1;
// }

// /**
//   * @brief  停止发送请求测量信号,并清空标志位
//   * @param  
//   * @retval 无
//   */
// void HC_SR04_SendRequest_End(void)
// {
//     HAL_GPIO_WritePin(HC_SR04_SignalOutput_GPIO_Port,HC_SR04_SignalOutput_Pin,GPIO_PIN_RESET);
//     HC_Sr40_Signal_SendFlag=0;
// }

// /**
//   * @brief  读取IC值，计算距离
//   * @param  
//   * @retval 无
//   */
// uint32_t HC_SR04_GetData(void)
// {
//   uint32_t start_time = HAL_GetTick();
//   uint32_t Value=0;
//   HAL_TIM_IC_Start(&htim3,TIM_CHANNEL_1);
//   HAL_TIM_IC_Start(&htim3,TIM_CHANNEL_2);
//   while((Value=HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2))==0)
//   {
//     if (HAL_GetTick()-start_time>(Time_Detect+30))
//     {
//       return 0;
//     }
    
//   }
//   OLED_ShowNum(3,1,Value,16);
//    OLED_ShowNum(4,1,HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1),16);
//   HC_SR40_Value=Time_Trans_Distance(Value*1000000/Fc);//单位统一为微秒
//   return HC_SR40_Value;
// }



