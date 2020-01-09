/**
 ******************************************************************************
 * @file    MotionFA_Manager.c
 * @author  MEMS Software Solutions Team
 * @brief   This file contains Fitness Activities interface functions
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
#include "MotionSM_Manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup FITNESS_ACTIVITIES FITNESS ACTIVITIES
 * @{
 */

/* Private variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialisze MotionSM algorithm
 * @param  None
 * @retval None
 */
void MotionSM_manager_init(void)
{
  char acc_orientation[3];
  MotionSM_Initialize();

#if (defined (USE_IKS01A2))
  /* LSM6DSL accelerometer onboard */
  acc_orientation[0] = 'n';
  acc_orientation[1] = 'w';
  acc_orientation[2] = 'u';

#endif

  MotionSM_SetOrientation_Acc(acc_orientation);
}
/**
 * @brief  Run Sleeping monitor algorithm
 * @param  data_in Structure containing input data
 * @param  data_out Structure containing ouput data
 * @retval None
 */
void MotionSM_manager_run(MSM_input_t *data_in, MSM_output_t *data_out)
{
	MotionSM_Update(data_in, data_out);
}

/**
 * @brief  Get the library version
 * @param  version  Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionSM_manager_get_version(char *version, int *length)
{
  *length = (int)MotionSM_GetLibVersion(version);
}

/**
 * @brief  Reset counter or current activity
 * @param  None
 * @retval None
 */
void MotionSM_manager_reset_counter(void)
{
	MotionSM_Reset();
}




