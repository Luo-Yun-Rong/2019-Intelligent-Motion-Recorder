/**
 ******************************************************************************
 * @file    main.h
 * @author  MEMS Software Solutions Team
 * @brief   This file contains definitions for the main.c file.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed under Software License Agreement
 * SLA0077, (the "License"). You may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *     www.st.com/content/st_com/en/search.html#q=SLA0077-t=keywords-page=1
 *
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#ifdef USE_IKS01A2
#include "nucleo_l476rg_bus.h"
#include "nucleo_l476rg_errno.h"
#include "iks01a2_env_sensors.h"
#include "iks01a2_env_sensors_ex.h"
#include "iks01a2_motion_sensors.h"
#include "iks01a2_motion_sensors_ex.h"
#endif

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  AW_MODE,
  SD_MODE,
  SM_MODE
} program_state_t;

typedef enum
{
  FLASH_FULL,
  FLASH_READY,
} flash_state_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
#if ((defined (USE_STM32F4XX_NUCLEO)) || (defined (USE_STM32L1XX_NUCLEO)) || (defined (USE_STM32L4XX_NUCLEO)))

/* Definition for TIMx clock resources : Timer used for algorithm */
#define TIM_ALGO                          TIM3
#define TIM_ALGO_CLK_ENABLE               __TIM3_CLK_ENABLE
#define TIM_ALGO_CLK_DISABLE              __TIM3_CLK_DISABLE

/* Definition for TIMx's NVIC */
#define TIM_ALGO_IRQn                     TIM3_IRQn
#define TIM_ALGO_IRQHandler               TIM3_IRQHandler

#else
#error Not supported platform
#endif

/* Enable sensor masks */
#define PRESSURE_SENSOR                         0x00000001U
#define TEMPERATURE_SENSOR                      0x00000002U
#define HUMIDITY_SENSOR                         0x00000004U
#define UV_SENSOR                               0x00000008U /* for future use */
#define ACCELEROMETER_SENSOR                    0x00000010U
#define GYROSCOPE_SENSOR                        0x00000020U
#define MAGNETIC_SENSOR                         0x00000040U

/* Exported functions --------------------------------------------------------*/
void Error_Handler(void);
void RTC_DateRegulate(uint8_t y, uint8_t m, uint8_t d, uint8_t dw);
void RTC_TimeRegulate(uint8_t hh, uint8_t mm, uint8_t ss);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
