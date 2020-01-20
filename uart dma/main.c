#include "stm32f4xx_hal.h"             
#include<string.h>

/*
*		There are three different methods for using uart-tx with dma 
*
*		1-using HAL_UART_Transmit_DMA and linking dma with UART_HandleTypeDef
*		2-using HAL_DMA_Start_IT which will activate dma interrupts 
*		3-using HAL_DMA_Start 
*
*/
	
GPIO_InitTypeDef hgpio;
UART_HandleTypeDef huart;
DMA_HandleTypeDef hdmatx;

void configureGPIO(void);
void configureUART(void);
void configureDMAtx(void);
void configureLed(void);

void SysTick_Handler(void) {
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
char  msg[]="Hi from Stm32f407vgt6\r";
int main()
{
	HAL_Init();
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn,0);
	SystemCoreClockUpdate();
	configureGPIO();
	configureDMAtx();
	configureUART();
	configureLed();
	
	//Method 1
	//HAL_UART_Transmit_DMA(&huart,(uint8_t*)msg,strlen(msg));
	
	//Method 2
	//HAL_DMA_Start_IT(&hdmatx,(uint32_t)&msg,(uint32_t)&huart.Instance->DR,strlen(msg));
	//huart.Instance->CR3 |= USART_CR3_DMAT;

	//Method 3
	//HAL_DMA_Start(&hdmatx,(uint32_t)&msg,(uint32_t)&huart.Instance->DR,strlen(msg));
	//huart.Instance->CR3 |=USART_CR3_DMAT;
	
	
	while(1)
		{
	
		}
		
		
	}

void configureLed(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	hgpio.Mode=GPIO_MODE_OUTPUT_PP;
	hgpio.Pin=GPIO_PIN_12;
	hgpio.Pull=GPIO_NOPULL;
	hgpio.Speed=GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOD,&hgpio);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
	
}
void configureGPIO(void)
{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		hgpio.Mode=GPIO_MODE_AF_PP;
		hgpio.Pin=GPIO_PIN_2 | GPIO_PIN_3;
		hgpio.Speed=GPIO_SPEED_FAST;
		hgpio.Alternate=GPIO_AF7_USART2;
		HAL_GPIO_Init(GPIOA,&hgpio);
}
void configureUART(void)
{
	__HAL_RCC_USART2_CLK_ENABLE();
	huart.Instance=USART2;
	huart.Init.BaudRate=115200;
	huart.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	huart.Init.Mode=UART_MODE_TX_RX;
	huart.Init.OverSampling=UART_OVERSAMPLING_16;
	huart.Init.Parity=UART_PARITY_NONE;
	huart.Init.StopBits=UART_STOPBITS_1;
	huart.Init.WordLength=UART_WORDLENGTH_8B;
	//activate in method 1
	//huart.hdmatx=&hdmatx;

	HAL_UART_Init(&huart);
}
void dmacompletecallback(DMA_HandleTypeDef * hdma1)
{
	//implement yout code here for method 2
}
void configureDMAtx(void)
{
	__DMA1_CLK_ENABLE();
	hdmatx.Instance=DMA1_Stream6;
	hdmatx.Init.Channel=DMA_CHANNEL_4;
	hdmatx.Init.Direction=DMA_MEMORY_TO_PERIPH;
	hdmatx.Init.PeriphInc=DMA_PINC_DISABLE;
	hdmatx.Init.MemInc=DMA_MINC_ENABLE;
	hdmatx.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;
	hdmatx.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
	hdmatx.Init.FIFOMode=DMA_FIFOMODE_DISABLE;
	hdmatx.Init.Priority=DMA_PRIORITY_HIGH;
	hdmatx.Init.Mode=DMA_CIRCULAR;
	//activate for method 2
	//hdmatx.XferCpltCallback=&dmacompletecallback;
	//HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	//HAL_NVIC_SetPriority(DMA1_Stream6_IRQn,1,0);
	
	HAL_DMA_Init(&hdmatx);
	
}
void DMA1_Stream6_IRQHandler()
{
	HAL_DMA_IRQHandler(&hdmatx);
}
