#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common

GPIO_InitTypeDef hgpio;
UART_HandleTypeDef huart;

uint8_t txbuffer1[]="Leds is blinking\n\r";
uint8_t txbuffer2[]="Leds is not blinking\n\r";

char  rx;
uint8_t legthofstr(uint8_t * str)
{
	uint8_t counter=0;
	while(str[counter])
	{
		counter++;
	}
	return counter;
}

void SysTick_Handler(void) {
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
int main()
{
	SystemInit();
	HAL_Init();
	
	//Enabling clock for usart and leds
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	
	//Gpio confiiguration for usart
	hgpio.Mode=GPIO_MODE_AF_PP;
	hgpio.Pin=GPIO_PIN_2 | GPIO_PIN_3;
	hgpio.Alternate=GPIO_AF7_USART2;
	hgpio.Speed=GPIO_SPEED_FAST;
	hgpio.Pull=GPIO_PULLUP;
	
	HAL_GPIO_Init(GPIOA,&hgpio);
	
	//gpio configureation for leds
	hgpio.Mode=GPIO_MODE_OUTPUT_PP;
	hgpio.Pin=GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	hgpio.Speed=GPIO_SPEED_HIGH;
	hgpio.Pull=GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOD,&hgpio);
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn,0);
	
	//Configuration for uart
	huart.Instance=USART2;
	huart.Init.BaudRate=115200;
	huart.Init.Mode=UART_MODE_TX_RX;
	huart.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	huart.Init.OverSampling=UART_OVERSAMPLING_16;
	huart.Init.Parity=UART_PARITY_NONE;
	huart.Init.StopBits=UART_STOPBITS_1;
	huart.Init.WordLength=UART_WORDLENGTH_8B;
	HAL_UART_Init(&huart);
	
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,GPIO_PIN_SET);

	
	while(1)
	{
		HAL_UART_Receive(&huart,(uint8_t *)rx,1,HAL_MAX_DELAY);
		HAL_Delay(500);
		if(rx=='a')
		{
				HAL_UART_Transmit(&huart,(uint8_t *)txbuffer1,legthofstr(txbuffer1),HAL_MAX_DELAY);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,GPIO_PIN_SET);
		}
		else
		{
				HAL_UART_Transmit(&huart,(uint8_t *)txbuffer2,legthofstr(txbuffer2),HAL_MAX_DELAY);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,GPIO_PIN_RESET);
		}	

		
		
	}
	
	
	return 0;
}
