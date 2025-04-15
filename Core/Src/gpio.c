/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
     PE4   ------> SAI4_D2
     PA10   ------> USB_OTG_HS_ID
     PE5   ------> SAI4_CK2
     PA9   ------> USB_OTG_HS_VBUS
     PH7   ------> SPI5_MISO
     PF11   ------> SPI5_MOSI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, Detectn_Pin|LCD_DISP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, ARD_D8_Pin|STMOD_17_Pin|STMOD_19_Pin|STMOD_18_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, LCD_BL_CTRL_Pin|ARD_D7_Pin|MEMS_LED_Pin|ARD_D4_Pin
                          |ARD_D2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, USER_LED2_Pin|USER_LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(STMOD_20_GPIO_Port, STMOD_20_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, LCD_RST_Pin|USB_FS_PWR_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Detectn_Pin LCD_DISP_Pin */
  GPIO_InitStruct.Pin = Detectn_Pin|LCD_DISP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : SAI4_D2_Pin SAI4_CK2_Pin */
  GPIO_InitStruct.Pin = SAI4_D2_Pin|SAI4_CK2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_SAI4;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D8_Pin STMOD_17_Pin STMOD_19_Pin STMOD_18_Pin */
  GPIO_InitStruct.Pin = ARD_D8_Pin|STMOD_17_Pin|STMOD_19_Pin|STMOD_18_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_FS_OVCR_Pin CTP_INT_Pin */
  GPIO_InitStruct.Pin = USB_FS_OVCR_Pin|CTP_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : Blue_button_B2_used_for_wakeup_Pin */
  GPIO_InitStruct.Pin = Blue_button_B2_used_for_wakeup_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Blue_button_B2_used_for_wakeup_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_FS_ID_Pin */
  GPIO_InitStruct.Pin = USB_FS_ID_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
  HAL_GPIO_Init(USB_FS_ID_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_BL_CTRL_Pin ARD_D7_Pin MEMS_LED_Pin ARD_D4_Pin
                           ARD_D2_Pin */
  GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin|ARD_D7_Pin|MEMS_LED_Pin|ARD_D4_Pin
                          |ARD_D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_FS_VBUS_Pin */
  GPIO_InitStruct.Pin = USB_FS_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_FS_VBUS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : uSD_Detect_Pin */
  GPIO_InitStruct.Pin = uSD_Detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(uSD_Detect_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USER_LED2_Pin USER_LED1_Pin */
  GPIO_InitStruct.Pin = USER_LED2_Pin|USER_LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI5_MISO_Pin */
  GPIO_InitStruct.Pin = SPI5_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
  HAL_GPIO_Init(SPI5_MISO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI5_MOSI_Pin */
  GPIO_InitStruct.Pin = SPI5_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
  HAL_GPIO_Init(SPI5_MOSI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Audio_Int_Pin */
  GPIO_InitStruct.Pin = Audio_Int_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Audio_Int_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : STMOD_20_Pin */
  GPIO_InitStruct.Pin = STMOD_20_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(STMOD_20_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : STMOD_11_INT_Pin */
  GPIO_InitStruct.Pin = STMOD_11_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(STMOD_11_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RST_Pin USB_FS_PWR_EN_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|USB_FS_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC2, SYSCFG_SWITCH_PC2_CLOSE);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC3, SYSCFG_SWITCH_PC3_CLOSE);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
