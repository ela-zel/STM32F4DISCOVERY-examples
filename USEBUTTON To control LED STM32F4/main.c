#include "stm32f4xx_hal.h"


/* in this program we want to use the push button to turn On/Off the LEDs*/

void MSDelay(uint32_t n);

int main(void)
{
	
	//lets start with enabling the clock for Port D & Port A
	RCC->AHB1ENR |= (1<<3); // Enable GPIOD Clock
	RCC->AHB1ENR |= 1; // Enable GPIOA Clock
	//Clear pin mode & configure as output Port D
	GPIOD->MODER &= ~0xFF000000;
	GPIOD->MODER |= 0x55000000; // Output Configuration
	//Clear pin mode & configure as input Port A
	GPIOA->MODER &= ~0x00000003;
	GPIOA->MODER |= 0x00000000; // Configure as input (Not important)
	
	while(1)
	{
		//Start by reading the GPIOA_IDR and test the value of PIN0
		if(GPIOA->IDR & 0x0001)
		{
			GPIOD->BSRR = 0x0000F000;
		}
		else
		{
			GPIOD->BSRR = 0xF0000000;
		}
		
	}
}

void MSDelay(uint32_t n)
{
	for(uint32_t i=0; i<n*4000;i++);
}