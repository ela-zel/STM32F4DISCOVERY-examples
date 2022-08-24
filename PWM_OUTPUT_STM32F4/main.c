#include "stm32F4xx.h"

int main(void)
{
	RCC->AHB1ENR |= 1; /* enable GPIOA clock */
	
	GPIOA->MODER &= ~0x00000C00; /* Clear PA0 For Alternate Function */
	GPIOA->MODER |= 0x00000800; /* Configure PA0 For Alternate Function */
	GPIOA->AFR[0] |= 0x00100000; /* PA0 Pin for tim2 */
	
	/************ setup TIM2 ************/
	RCC->APB1ENR |= 1; /* enable TIM2 clock */
	TIM2->PSC = 10 -1; /* divided by 10 */
	TIM2->ARR = 26667 - 1; /* divided by 26667 */
	TIM2->CNT = 0; 
	TIM2->CCMR1 = 0x0060; /* PWM mode */
	TIM2->CCER = 1; /* enable PWM Ch1 */
	TIM2->CCR1 = 8889 - 1; /* pulse width 1/3 of the period */
	TIM2->CR1 = 1; /* enable timer */
	while(1)
	{
		
	}
}