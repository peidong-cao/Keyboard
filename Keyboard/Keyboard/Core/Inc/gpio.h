/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.h
 * @brief   This file contains all the function prototypes for
 *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
/*#define KEY_R0 GPIO_PIN_3
#define KEY_R0_GPIO_Port GPIOA

#define KEY_R1 GPIO_PIN_4
#define KEY_R1_GPIO_Port GPIOA

#define KEY_R2 GPIO_PIN_5
#define KEY_R2_GPIO_Port GPIOA

#define KEY_R3 GPIO_PIN_6
#define KEY_R3_GPIO_Port GPIOA

#define KEY_C0 GPIO_PIN_7
#define KEY_C0_GPIO_Port GPIOA

#define KEY_C1 GPIO_PIN_0
#define KEY_C1_GPIO_Port GPIOB

#define KEY_C2 GPIO_PIN_1
#define KEY_C2_GPIO_Port GPIOB

#define KEY_C3 GPIO_PIN_2
#define KEY_C3_GPIO_Port GPIOB*/

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

