#include "stm32f4xx.h"

int main(void)
{
	__disable_irq();
	
	RCC->AHB1ENR |= 0x8;
	GPIOD->MODER &= ~0xFF000000;
	GPIOD->MODER |= 0x55000000;
	
	/* setup TIM2 */
	RCC->APB1ENR |= 0x1; /* enable TIM2 clock */
	TIM2->PSC = 16000-1; /* divided by 16000 */
	TIM2->ARR = 1000-1; /* divided by 1000 */
	TIM2->CR1 = 1;/* enable counter */
	TIM2->DIER |= 1; /* enable UIE */
	NVIC_EnableIRQ(TIM2_IRQn); /* enable interrupt in NVIC */
	
	__enable_irq(); /* global enable IRQs */
	while(1)
	{
		
	}
}

void TIM2_IRQHandler(void)
{
	TIM2->SR = 0; // Clear Update Interrupt Flag (UIF)
	GPIOD->ODR ^= 0xF000; // Toggle LEDs
}