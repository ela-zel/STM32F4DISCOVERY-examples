#include "stm32f4xx_hal.h"


//this example uses assambly language to toggle LEDs
//1 - enable the clock register for the peripheral 
//2 - configure the pins as outputs using direction registers
//3 - write the desired value to the output data register

void DelayMS(int n);
void MSDelay(uint32_t n);


int main(void)
{
	
	//start with enabling the Peripheral clock for Port D
	RCC->AHB1ENR |= (1<<3);
	GPIOD->MODER &= ~0xFF000000; // clearing the pin 12|13|14|15 mode
	GPIOD->MODER |= 0x55000000; // set pin 12|13|14|15 mode to output
	
	while(1)
	{
		/******************First Method******************/
//		GPIOD->ODR |= 0x0000F000; // turn on LED3 & LED4 & LED5 & LED6
//		DelayMS(100);
//		GPIOD->ODR &= ~0x0000F000; // turn off LED3 & LED4 & LED5 & LED6
//		DelayMS(100);
		/******************Second Method******************/
		// using the XOR operator to demonstrate the toggle capabilitie of this operator
//		GPIOD->ODR ^= (0xFUL<<12);
//		MSDelay(500);
		/******************Third Method******************/
		// Toggling the LEDs using the BSRR registre to avoid interruption
		GPIOD->BSRR = 0x0000F000;
		MSDelay(500);
		GPIOD->BSRR = 0xF0000000;
		MSDelay(500);
	}
//	  cout << "Quel age avez-vous ?" << endl;
//    int ageUtilisateur(0); //On prepare une case mémoire pour stocker un entier
//    //cin >> ageUtilisateur; //On fait entrer un nombre dans cette case
//    cout << "Vous avez " << ageUtilisateur << " ans !" <<  endl; //Et on l'affiche
}

/* 16 MHz SYSCLK */
/*we didn't active any timer so we cant use the HAL_DELAY() Function
this is the reason behind defining our own delay function*/
void DelayMS(int n)
{
	int i;
	for (;n>0;n--)
	for(i=0;i<3195;i++);
}
//this is another way to define a delay function
void MSDelay(uint32_t n)
{
	for(uint32_t i=0; i<n*4000; i++);
}