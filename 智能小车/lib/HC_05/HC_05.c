#include "stm32f1xx_hal.h"
#include "main.h"
#include "OLED.h"
#include "usart.h"
#include "string.h"

//接收数据缓存数组
uint8_t Word_exchange[100]={0};
//指令
uint8_t Key_Version[]={"AT+VERSION?\r\n"};
uint8_t Key_ORGL[]={"AT+ORGL\r\n"};
uint8_t Key_ADDR[]={"AT+ADDR?\r\n"};
uint8_t Key_NAME[]={"AT+NAME?\r\n"};
uint8_t Key_ROLE[]={"AT+ROLE?\r\n"};
uint8_t Key_Test[]={"AT\r\n"};

//传输等待时间和上电等待时间
#define Time_Wait 10
#define Time_Power 6000


void EN_Set(uint8_t State)
{
    if(State==1)
    {
        HAL_GPIO_WritePin(HC_EN_GPIO_Port,HC_EN_Pin,GPIO_PIN_SET);
    }
    else if (State==0)
    {
        HAL_GPIO_WritePin(HC_EN_GPIO_Port,HC_EN_Pin,GPIO_PIN_RESET);
    }    
}


/**
  * @brief  HC_05进入命令模式，需在规定时间内外部接通电源
  * @retval 无
  */
void HC_05_Command_Enter(void)
{
    EN_Set(1);
    OLED_ShowString(1,1,"Please Power");
    HAL_Delay(Time_Power);
    OLED_Clear();
    OLED_ShowString(2,1,"C:");
    OLED_ShowString(3,1,"R:");
}

void HC_05_Command_Exit(void)
{
    EN_Set(0);
}

/**
  * @brief  HC_05指令执行
  * @param  Command 指令
  * @retval 无
  */
void HC_05_Command(uint8_t * Command)
{ 
    uint8_t HAL_State; 
    HAL_State=HAL_UART_Transmit(&huart1,Command, strlen(Command), Time_Wait);//command是字符串数组，无法使用sizeof获取
    OLED_ShowNum(4,1,strlen(Command),1);
    if(HAL_State!=HAL_OK)
    {
        OLED_ShowString(1,1,"               ");
        OLED_ShowString(1,1,"Trans_Error:");
        OLED_ShowNum(1,15,HAL_State,1);
    }
    while (!(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC))){};
    //注意此处，UART和USART不同
    HAL_State=HAL_UART_Receive(&huart1,Word_exchange,100,Time_Wait);
    if((HAL_State!=HAL_OK)&&(HAL_State!=HAL_TIMEOUT))
    {
        OLED_ShowString(1,1,"               ");
        OLED_ShowString(1,1,"Rec_Error:");
        OLED_ShowNum(1,15,HAL_State,1);
    }
    while ((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE))){};
    OLED_ShowString(2,3,"              ");
    for (size_t i = 0; Command[i] !='\r'; i++)
    {
        if ((3+i)<=16)
        {
            OLED_ShowChar(2,3+i,Command[i]);
        }
    }
    OLED_ShowString(3,3,"              ");
    for (size_t i = 0; Word_exchange[i] !='\r'; i++)
    {
        if ((3+i)<=16)
        {
            OLED_ShowChar(3,3+i,Word_exchange[i]);
        }
    }    
}

void HC_05_SelfTest(void)
{
    HC_05_Command(Key_Test); 
}
