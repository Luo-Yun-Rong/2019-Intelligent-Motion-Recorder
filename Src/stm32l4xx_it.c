/**
 ******************************************************************************
 * @file    stm32l4xx_it.c
 * @author  MEMS Software Solutions Team
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_it.h"
#include "MotionAW_Manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup ACTIVITY_RECOGNITION_WRIST ACTIVITY RECOGNITION WRIST
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
extern volatile uint8_t FlashEraseRequest;
extern flash_state_t FlashState;
extern uint8_t DataLoggerActive;

/* Private function prototypes -----------------------------------------------*/
void TIM_ALGO_IRQHandler(void);

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  for (;;)
  {
  }
}

/**
 * @brief  This function handles Debug Monitor exception
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32L4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l4xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles TIM_ALGO global interrupt.
 * @param  None
 * @retval None
 */
void TIM_ALGO_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&AlgoTimHandle);
}

/**
 * @brief  This function handles External line 10-15 interrupt request
 * @param  None
 * @retval None
 */
void EXTI15_10_IRQHandler(void)
{
  if (__HAL_GPIO_EXTI_GET_IT(KEY_BUTTON_PIN) != (uint32_t)RESET)
  {
    /* Button locked when active GUI datastream */
    if (DataLoggerActive != 0U)
    {
      __HAL_GPIO_EXTI_CLEAR_IT(KEY_BUTTON_PIN);
      return;
    }

    if (FlashState == FLASH_FULL)
    {
      HAL_Delay(3500);

      if (BSP_PB_GetState(BUTTON_KEY) == (uint32_t)GPIO_PIN_RESET)
      {
        FlashEraseRequest = 1;
      }

      __HAL_GPIO_EXTI_CLEAR_IT(KEY_BUTTON_PIN);
      return;
    }

    HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
  }
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
