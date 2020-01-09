/**
 ******************************************************************************
 * @file    MotionAW_Manager.c
 * @author  MEMS Software Solutions Team
 * @brief   This file contains Activity Recognition Wrist interface functions
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
#include "MotionAW_Manager.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup ACTIVITY_RECOGNITION_WRIST ACTIVITY RECOGNITION WRIST
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialises MotionAW algorithm
 * @param  None
 * @retval None
 */
void MotionAW_manager_init(void)
{
  char acc_orientation[3];

  MotionAW_Initialize();

#ifdef USE_IKS01A2
  acc_orientation[0] = 'n';
  acc_orientation[1] = 'w';
  acc_orientation[2] = 'u';
#endif

  MotionAW_SetOrientation_Acc(acc_orientation);
}

/**
 * @brief  Run activity recognition wrist algorithm. This function collects and scale data from accelerometer and calls the Activity Recognition Wrist Algo
 * @param  data_in  Structure containing input data
 * @param  data_out  Structure containing output data
 * @param  timestamp  time in [ms]
 * @retval None
 */
void MotionAW_manager_run(MAW_input_t *data_in, MAW_activity_t *data_out, int64_t timestamp)
{
  MAW_output_t data;

  MotionAW_Update(data_in, &data, timestamp);
  *data_out = data.current_activity;
}

/**
 * @brief  Get the library version
 * @param  version  Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionAW_manager_get_version(char *version, int *length)
{
  *length = (int)MotionAW_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
