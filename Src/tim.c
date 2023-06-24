/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "usart.h"
int Remoter_Ch1=0,Remoter_Ch2=0,Remoter_Ch3=0,Remoter_Ch4=0,Remoter_Adjustment=-110;

uint8_t TIM1CH1_CAPTURE_STA = 0,TIM1CH2_CAPTURE_STA = 0,TIM1CH3_CAPTURE_STA = 0,TIM1CH4_CAPTURE_STA = 0;	
uint16_t TIM1CH1_CAPTURE_VAL,TIM1CH2_CAPTURE_VAL,TIM1CH3_CAPTURE_VAL,TIM1CH4_CAPTURE_VAL;

/* USER CODE END 0 */

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

/* TIM1 init function */
void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

}
/* TIM3 init function */
void MX_TIM3_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 71;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim3);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();
  
    __HAL_RCC_GPIOE_CLK_ENABLE();
    /**TIM1 GPIO Configuration    
    PE9     ------> TIM1_CH1
    PE11     ------> TIM1_CH2
    PE13     ------> TIM1_CH3
    PE14     ------> TIM1_CH4 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_TIM1_ENABLE();

    /* TIM1 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_BRK_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_IRQn);
    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM3 GPIO Configuration    
    PB5     ------> TIM3_CH2 
    */
    GPIO_InitStruct.Pin = LED_0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_0_GPIO_Port, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_TIM3_PARTIAL();

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();
  
    /**TIM1 GPIO Configuration    
    PE9     ------> TIM1_CH1
    PE11     ------> TIM1_CH2
    PE13     ------> TIM1_CH3
    PE14     ------> TIM1_CH4 
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_14);

    /* TIM1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_BRK_IRQn);
    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	HAL_UART_Transmit(&huart1, "a", 10, 10);
	
	if(htim == (&htim1))
	{
		if((TIM1CH1_CAPTURE_STA & 0X80) == 0)//还未成功捕获
		{	
			if (TIM_GET_ITSTATUS(&htim1, TIM_IT_CC1) != RESET) //A capture event occurred on channel 1 //通道1发生捕获事件
			{
				if(TIM1CH1_CAPTURE_STA&0X40)				//捕获到一个下降沿 		
				{	  			
					TIM1CH1_CAPTURE_STA|=0X80;				//标记成功捕获到一次高电平脉宽
					TIM1CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_1);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//配置TIM1通道1上升沿捕获
				}
				else  										//还未开始,第一次捕获上升沿
				{
					TIM1CH1_CAPTURE_STA=0;					//清空
					TIM1CH1_CAPTURE_VAL=0;
					TIM1CH1_CAPTURE_STA|=0X40;				//标记捕获到了上升沿
					
					__HAL_TIM_DISABLE(&htim1);	//关闭定时器
					__HAL_TIM_SET_COUNTER(&htim1,0);
					TIM_RESET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器1通道1设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim1);	//使能定时器
				}		
			}		
		}
		
		if((TIM1CH2_CAPTURE_STA & 0X80) == 0)//还未成功捕获
		{	
			if (TIM_GET_ITSTATUS(&htim1, TIM_IT_CC2) != RESET) //A capture event occurred on channel 1 //通道1发生捕获事件
			{
				if(TIM1CH2_CAPTURE_STA&0X40)				//捕获到一个下降沿 		
				{	  			
					TIM1CH2_CAPTURE_STA|=0X80;				//标记成功捕获到一次高电平脉宽
					TIM1CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_2);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//配置TIM1通道1上升沿捕获
				}
				else  										//还未开始,第一次捕获上升沿
				{
					TIM1CH2_CAPTURE_STA=0;					//清空
					TIM1CH2_CAPTURE_VAL=0;
					TIM1CH2_CAPTURE_STA|=0X40;				//标记捕获到了上升沿
					
					__HAL_TIM_DISABLE(&htim1);	//关闭定时器
					__HAL_TIM_SET_COUNTER(&htim1,0);
					TIM_RESET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//定时器1通道1设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim1);	//使能定时器
				}		
			}		
		}
		
		if((TIM1CH3_CAPTURE_STA & 0X80) == 0)//还未成功捕获
		{	
			if (TIM_GET_ITSTATUS(&htim1, TIM_IT_CC3) != RESET) //A capture event occurred on channel 1 //通道1发生捕获事件
			{
				if(TIM1CH3_CAPTURE_STA&0X40)				//捕获到一个下降沿 		
				{	  			
					TIM1CH3_CAPTURE_STA|=0X80;				//标记成功捕获到一次高电平脉宽
					TIM1CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_3);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//配置TIM1通道1上升沿捕获
				}
				else  										//还未开始,第一次捕获上升沿
				{
					TIM1CH3_CAPTURE_STA=0;					//清空
					TIM1CH3_CAPTURE_VAL=0;
					TIM1CH3_CAPTURE_STA|=0X40;				//标记捕获到了上升沿
					
					__HAL_TIM_DISABLE(&htim1);	//关闭定时器
					__HAL_TIM_SET_COUNTER(&htim1,0);
					TIM_RESET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//定时器1通道1设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim1);	//使能定时器
				}		
			}		
		}
		
		if((TIM1CH4_CAPTURE_STA & 0X80) == 0)//还未成功捕获
		{	
			if (TIM_GET_ITSTATUS(&htim1, TIM_IT_CC4) != RESET) //A capture event occurred on channel 1 //通道1发生捕获事件
			{
				if(TIM1CH4_CAPTURE_STA&0X40)				//捕获到一个下降沿 		
				{	  			
					TIM1CH4_CAPTURE_STA|=0X80;				//标记成功捕获到一次高电平脉宽
					TIM1CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_4);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//配置TIM1通道1上升沿捕获
				}
				else  										//还未开始,第一次捕获上升沿
				{
					TIM1CH4_CAPTURE_STA=0;					//清空
					TIM1CH4_CAPTURE_VAL=0;
					TIM1CH4_CAPTURE_STA|=0X40;				//标记捕获到了上升沿
					
					__HAL_TIM_DISABLE(&htim1);	//关闭定时器
					__HAL_TIM_SET_COUNTER(&htim1,0);
					TIM_RESET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//定时器1通道1设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim1);	//使能定时器
				}		
			}		
		}
		
	
	}
	HAL_UART_Transmit(&huart1, "b", 10, 10);
}	



/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
