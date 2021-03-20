#include "stm32f4xx_hal.h"

void GPIO_Config(void);
void TIM_Config(void);

TIM_HandleTypeDef htim2;

int main(void)
{
	//HAL Initialisation
	HAL_Init();
	//SystemClock_Config();
	//Config functions
	GPIO_Config();
	TIM_Config();
	//NVIC_SetPriority(TIM2_IRQn, 0x03); //Can set priority if needed
  NVIC_EnableIRQ(TIM2_IRQn); //Enabling the timer 2 global interrupt
	//Start Input Capture TIM2-Channel1
	HAL_TIM_Base_Start_IT(&htim2); //Starts the timer
	
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
		HAL_Delay(1000);
	}
}

void GPIO_Config(void)
{
	//Enable port D clock
	__HAL_RCC_GPIOD_CLK_ENABLE();
	//Init TypeDef to configure GPIO pins
	GPIO_InitTypeDef myGPIO_Init;
	//basic configurations
	myGPIO_Init.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;							//PD12-15
	myGPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;	
	//Implement the configurations
	HAL_GPIO_Init(GPIOD, &myGPIO_Init);
	
}

void TIM_Config(void)
{
	//Enable TIM2 clock
	__HAL_RCC_TIM2_CLK_ENABLE();
	
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 16000; //reducing clock to 1MHz
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;//1 count corresponds to 1ms, 1000 count results in interrupt trigger for every 1 second
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    
  }
	
}



//Timer 2 interrupt subroutine
void TIM2_IRQHandler(void)
{
	HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
	HAL_TIM_IRQHandler(&htim2); //This function clears the interrupt flag and resets the timer
}

void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}