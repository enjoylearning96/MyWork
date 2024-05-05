#include "US-016.h"
#include "adc.h"
#include "OLED.h"
#include "dma.h"

uint32_t US106_Data[1]={0};
void US106_Init(void)
{

}

void US106_GetData_3m(void)
{
    HAL_ADC_Start_DMA(&hadc1,US106_Data,1);
    // HAL_ADC_Stop(&hadc1);
}
