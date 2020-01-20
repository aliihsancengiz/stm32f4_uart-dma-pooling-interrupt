#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx_hal.h"

uint8_t txbuffer[]="I am trigerred by pa0 button\r";
uint8_t rxbuffer[1];

//flag for receiving it
uint8_t rxcomplete=1;

//Handler for gpio and uart
GPIO_InitTypeDef hgpio;
UART_HandleTypeDef huart;

/*
*		@brief : calculate the length of given string 
*		@param : string which length is calculated
*		@ret 	 : length of string
*/
uint8_t legthofstr(uint8_t * str)
{
	uint8_t counter=0;
	while(str[counter])
	{
		counter++;
	}
	return counter;
}

/*
*		@brief : configuration for uart
*		@param : none
*		@ret 	 : none
*/
void ConfigurationForUsart()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	
	//Gpio confiiguration for usart
	hgpio.Mode=GPIO_MODE_AF_PP;
	hgpio.Pin=GPIO_PIN_2 | GPIO_PIN_3;
	hgpio.Alternate=GPIO_AF7_USART2;
	hgpio.Speed=GPIO_SPEED_FAST;
	hgpio.Pull=GPIO_PULLUP;
	
	HAL_GPIO_Init(GPIOA,&hgpio);
	
	huart.Instance=USART2;
	huart.Init.BaudRate=115200;
	huart.Init.Mode=UART_MODE_TX_RX;
	huart.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	huart.Init.OverSampling=UART_OVERSAMPLING_16;
	huart.Init.Parity=UART_PARITY_NONE;
	huart.Init.StopBits=UART_STOPBITS_1;
	huart.Init.WordLength=UART_WORDLENGTH_8B;
	HAL_UART_Init(&huart);
	
	
}


/*
*		@brief : configure the button(PA0) on chip for triggering uart_it 
*		@param : none
*		@ret 	 : none
*/
void configureButton()
{
		//Button and External interrupt Handler
		GPIO_InitTypeDef hbutton;
		
		//Enabling GPIOA clock
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		//pa0 as input,puldown,high speed
		hbutton.Mode=GPIO_MODE_INPUT;
		hbutton.Pin=GPIO_PIN_0;
		hbutton.Speed=GPIO_SPEED_FREQ_HIGH;
		hbutton.Pull=GPIO_PULLDOWN;
		//Initialize the Button
		HAL_GPIO_Init(GPIOA,&hbutton);
		
		//Set external interrrupt mode as interrupt and rising edge
		EXTI->IMR  |=(0x00000001);
		EXTI->RTSR |=(0x00000001);
		
		//Enable the external interrupt
		NVIC_EnableIRQ(EXTI0_IRQn);
		NVIC_SetPriority(EXTI0_IRQn,1);
}	

/*
*		@brief : IT Handler for button  
*		@param : none
*		@ret 	 : none
*/
void EXTI0_IRQHandler()
{	
	if(EXTI->PR & 0x1)	//Checking if pending register is set
	{
		//If interrupt is fired then trigger transmission
		HAL_UART_Transmit_IT(&huart,txbuffer,legthofstr(txbuffer));
		EXTI->PR |=0x1;		//CLEAR_BIT pending register
	}
}
void USART2_IRQHandler()
{
	HAL_UART_IRQHandler(&huart);
}
//Handler for incrementing Tick which is used for perecise delay function
void SysTick_Handler(void) 
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {
	rxcomplete=1;
}
#endif
