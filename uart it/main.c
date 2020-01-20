#include "main.h"       


int main()
{
	
	SystemInit();
	SystemCoreClockUpdate();
	HAL_Init();
	
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn,0);
	
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn,1);
	
	configureButton();
	ConfigurationForUsart();
	

	
	while(1)
	{
			//If rx complete flag is set board is ready to receive 
			if(rxcomplete)
			{
				rxcomplete=0;
				HAL_UART_Receive_IT(&huart,(uint8_t *)rxbuffer,1);
				//transmit whatever you receive to the host
				HAL_UART_Transmit(&huart,(uint8_t*)rxbuffer,1,HAL_MAX_DELAY);
			}
	}
	
}
