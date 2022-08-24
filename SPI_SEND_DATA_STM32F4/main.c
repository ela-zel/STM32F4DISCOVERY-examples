#include "stm32f4xx.h"

/*
1) Enable the SPI clock at RCC_APBxENR register
2) Enable the GPIOx clock at RCC_AHB1ENR register
3) Configure the port pins for SPI by GPIOx_MODER and GPIOx_AFRy registers
4) Configure a port pin for Slave Select by GPIOx_MODER register
5) Configure SPI_CR1 register to select data bit ordering, clock polarity, 
clock phase, Baud rate, SPI master mode, data frame size, and others.
6) Clear or leave SPI_CR2 in default
7) Enable the SPI by setting SPE (SPI enable) bit of SPI_CR1 register
8) Monitor the TXE (Transfer buffer Empty) flag bit of the SPI_SR (status) 
register until the transmit data register is empty
9) Assert slave select pin
10) Write data to SPI_DR (data) register to be transmitted
11) Wait until BSY flag is cleared in SPI_SR then deassert slave select
10) To transmit another character, go to step 8
*/

/*Using SPI1 to send A to Z characters
*
* SPI1 is configured as master with software slave select. 
* Clock rate is set to 1 MHz. Polarity/Phase are 0, 0
*
* PA7 MOSI
* PA5 SCK
* PA4 SS
*
*/

void DelayMS(uint32_t n);
void SPI1_Init(void);
void SPI1_Write(unsigned char Data);

int main(void)
{
	
	char c;
	SPI1_Init();
	
	while(1)
	{
		for(c='A'; c<'Z'; c++)
		{
			SPI1_Write(c); /* write the letter through SPI1 */
			DelayMS(100);
		}
	}
}


void DelayMS(uint32_t n)
{
	int i=0;
	for(; n>0; n--)
	for(i=0; i<3195; i++);
}
void SPI1_Init(void)
{
	RCC->AHB1ENR |= 1; /* enable GPIOA clock */
	RCC->APB2ENR |= 0x1000; /* enable SPI1 clock */
	
	/* PORTA 5, 7 for SPI1 MOSI and SCLK */
	GPIOA->MODER &= ~0xCC00; /* clear pin mode */
	GPIOA->MODER |= 0x8800; /* set pin alternate mode */
	GPIOA->AFR[0] &= ~0xF0F00000; /* clear alt mode */
	GPIOA->AFR[0] |= 0x50500000; /* set alt mode SPI1 */
	
	/* PORTA4 as GPIO output for SPI slave select */
	GPIOA->MODER &= ~0x00000300; /* clear pin mode */
	GPIOA->MODER |= 0x00000100; /* set pin output mode */
	SPI1->CR1 |= 0x031C; /* set the Baud rate, 8-bit data frame */
	SPI1->CR2 = 0;
	SPI1->CR1 |= 0x0040;/* enable SPI1 module */
}

/* This function enables slave select, writes one byte to SPI1, 
wait for transmission complete and deassert slave select. */

void SPI1_Write(unsigned char Data)
{
	while (!(SPI1->SR & 2)) {} /* wait until Transfer buffer Empty */
	GPIOA->BSRR = 0x00100000; /* assert slave select */
	SPI1->DR = Data; /* write data */
	while(SPI1->SR & 0x80) {};/* wait for transmission done */
	GPIOA->BSRR = 0x00000010; /* deassert slave select */
}