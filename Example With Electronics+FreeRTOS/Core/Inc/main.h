/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWM_Left_Pin GPIO_PIN_0
#define PWM_Left_GPIO_Port GPIOA
#define PWM_Right_Pin GPIO_PIN_1
#define PWM_Right_GPIO_Port GPIOA
#define Left_Back_Control_2_Pin GPIO_PIN_4
#define Left_Back_Control_2_GPIO_Port GPIOA
#define Left_Back_Control_1_Pin GPIO_PIN_5
#define Left_Back_Control_1_GPIO_Port GPIOA
#define Right_Back_Control_1_Pin GPIO_PIN_6
#define Right_Back_Control_1_GPIO_Port GPIOA
#define Right_Back_Control_2_Pin GPIO_PIN_7
#define Right_Back_Control_2_GPIO_Port GPIOA
#define HC_STATE_Pin GPIO_PIN_8
#define HC_STATE_GPIO_Port GPIOA
#define TX_HC_05_Pin GPIO_PIN_9
#define TX_HC_05_GPIO_Port GPIOA
#define RX_HC_05_Pin GPIO_PIN_10
#define RX_HC_05_GPIO_Port GPIOA
#define HC_EN_Pin GPIO_PIN_11
#define HC_EN_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
