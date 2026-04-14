/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define SD_CS_Pin GPIO_PIN_4
#define SD_CS_GPIO_Port GPIOA
#define SD_CD_Pin GPIO_PIN_4
#define SD_CD_GPIO_Port GPIOC
#define SD_CD_EXTI_IRQn EXTI4_IRQn
#define SIM_STATUS_Pin GPIO_PIN_0
#define SIM_STATUS_GPIO_Port GPIOB
#define SIM_PWR_Pin GPIO_PIN_14
#define SIM_PWR_GPIO_Port GPIOE
#define SIM_HRST_Pin GPIO_PIN_15
#define SIM_HRST_GPIO_Port GPIOE
#define SIM_RST_Pin GPIO_PIN_8
#define SIM_RST_GPIO_Port GPIOD
#define ST_RELAY1_Pin GPIO_PIN_10
#define ST_RELAY1_GPIO_Port GPIOD
#define ST_RELAY2_Pin GPIO_PIN_11
#define ST_RELAY2_GPIO_Port GPIOD
#define ST_SW2_Pin GPIO_PIN_12
#define ST_SW2_GPIO_Port GPIOD
#define ST_SW4_Pin GPIO_PIN_13
#define ST_SW4_GPIO_Port GPIOD
#define ST_SW3_Pin GPIO_PIN_14
#define ST_SW3_GPIO_Port GPIOD
#define ST_SW1_Pin GPIO_PIN_15
#define ST_SW1_GPIO_Port GPIOD
#define LED_PWR_Pin GPIO_PIN_3
#define LED_PWR_GPIO_Port GPIOD
#define LED_SENSOR_Pin GPIO_PIN_4
#define LED_SENSOR_GPIO_Port GPIOD
#define LED_DRIVER_Pin GPIO_PIN_5
#define LED_DRIVER_GPIO_Port GPIOD
#define LED_SERVER_Pin GPIO_PIN_6
#define LED_SERVER_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
