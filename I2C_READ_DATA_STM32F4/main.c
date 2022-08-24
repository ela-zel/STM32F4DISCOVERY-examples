/*
**************Configuring I2C for master data transmission**************

Here are the steps to configure the I2C for master data transmission and receiving.

1. Enable the clock to GPIOB that has the I2C I/O pins
2. Enable the clock to I2C using the RCC_APB1ENR register.
3. Configure the port pins for I2C by with GPIOx_MODER and GPIOx_AFRL registers
4. Configure the port pins for I2C to be open-drain output with pull-up resistors
5. Reset the I2C using bit 15 of I2C_CR1
6. Configure I2C_CR1 and I2C_CR2 registers to select I2C master mode
7. Set the SCL frequency using I2C_CCR register
8. Configure the maximum rise time of I2C SCL using I2C_TRISE register
9. Enable the I2C by setting PE=1 bit of I2C_CR2 register

**************Send a byte of data to a slave device**************

1. Monitor the BUSY bit of I2C_SR2 (Status 2) register until the bus is idle
2. Write to START bit (bit 8) of I2C_CR1 (Control 1) register to generate a START condition on the bus
3. Wait until the START condition is complete, the SB bit (bit 0) of I2C_SR1 register is set
4. Write the slave address with the R/W bit (bit 0) cleared (for a write) to I2C_DR (Data Register), 
this will transmit the slave address
5. Wait until ADDR bit (bit 1) of I2C_SR1 register is set signaling that the slave address is sent
6. Read I2C_SR1 and I2C_SR2 to clear the ADDR bit
7. Wait until TxE bit of I2C_SR1 register is set signaling that the transmit data register is empty
8. Write the register/memory address to I2C_DR register
9. Wait until TxE bit of I2C_SR1 register is set signaling that the transmit data register is empty
10. Write the data to I2C_DR register
11. Wait until BTF (Byte Transfer Finished) bit of I2C_SR1 register is set signaling that the data is sent
12. Write to STOP bit (bit 9) of I2C_CR1 register to generate a STOP condition on the bus The procedure 
listed above is a simplified version. In real-world application, the error bits of the I2C_SR1 register 
should be checked after each byte is sent to make sure there was no bus collision and the slave responded 
with an acknowledge.

**************Read a byte of data from a slave device**************

1. Monitor the BUSY bit of I2C_SR2 (Status 2) register until the bus is idle
2. Write to START bit (bit 8) of I2C_CR1 (Control 1) register to generate a START condition on the bus
3. Wait until the START condition is complete, the SB bit (bit 0) of I2C_SR1 register is set
4. Write the slave address with the R/W bit (bit 0) cleared (for a write) to I2C_DR (Data Register), 
this will transmit the slave address
5. Wait until ADDR bit (bit 1) of I2C_SR1 register is set signaling that the slave address is sent
6. Read I2C_SR1 and I2C_SR2 to clear the ADDR bit
7. Wait until TxE bit of I2C_SR1 register is set signaling that the transmit data register is empty
8. Write the register/memory address to I2C_DR register
9. Wait until TxE bit of I2C_SR1 register is set signaling that the transmit data register is empty
10. Write to START bit (bit 8) of I2C_CR1 (Control 1) register to generate a RESTART condition
11. Wait until the RESTART condition is complete, the SB bit (bit 0) of I2C_SR1 register is set
12. Write the slave address with the R/W bit (bit 0) set (for a read) to I2C_DR (Data Register), 
this will transmit the slave address with read
13. Wait until ADDR bit (bit 1) of I2C_SR1 register is set signaling that the slave address is sent
14. Clear ACK bit (bit 10) of I2C_CR1 register, preparing to return a NACK
15. Read I2C_SR1 and I2C_SR2 to clear the ADDR bit
16. Write to STOP bit (bit 9) of I2C_CR1 register to generate a STOP condition on the bus
17. Wait until RxNE bit of I2C_SR1 register is set signaling that the receive data register is not empty
18. Read I2C_DR register to retrieve the data

*/

#include "stm32f4xx.h"
#define SLAVE_ADDR 0x68 /* 1101 000. DS1337 */

void delay_ms(int n);
void I2C1_Init(void);
int I2C1_ByteWrite(char saddr, char maddr, char data);

int main(void)
{
	
	I2C1_Init();
	I2C1_ByteWrite(SLAVE_ADDR, 0x0E, 0);
	
	while(1)
	{
		
	}
}

void delay_ms(int n)
{
	int i;
	for( ; n>0; n--)
	for(i=0; i<3195; i++);
}
void I2C1_Init(void)
{
	RCC->AHB1ENR |= 2; /* Enable GPIOB clock */
	RCC->APB1ENR |= 0x00200000; /* Enable I2C1 clock */
	
	/* configure PB8, PB9 pins for I2C1 */
	GPIOB->AFR[1] &= ~0x000000FF; /* PB8, PB9 I2C1 SCL, SDA */
	GPIOB->AFR[1] |= 0x00000044;
	GPIOB->MODER &= ~0x000F0000; /* PB8, PB9 use alternate function */
	GPIOB->MODER |= 0x000A0000;
	GPIOB->OTYPER |= 0x00000300; /* output open-drain */
	GPIOB->PUPDR &= ~0x000F0000; /* with pull-ups */
	GPIOB->PUPDR |= 0x00050000;
	I2C1 ->CR1 = 0x8000; /* software reset I2C1 */
	I2C1->CR1 &= ~0x8000; /* out of reset */
	I2C1->CR2 = 0x0010; /* peripheral clock is 16 MHz */
	I2C1->CCR = 80; /* standard mode, 100kHz clock */
	I2C1->TRISE = 17; /* maximum rise time */
	I2C1->CR1 |= 0x0001; /* enable I2C1 module */
	
}

/* this funtion writes a byte of data to the memory location maddr of
* a device with I2C slave device address saddr.
* For simplicity, no error checking or error report is done. */

int I2C1_ByteWrite(char saddr, char maddr, char data)
{
	volatile int tmp;
	while (I2C1->SR2 & 2); /* wait until bus not busy */
	I2C1->CR1 |= 0x100; /* generate start */
	while (!(I2C1->SR1 & 1)); /* wait until start flag is set */
	I2C1->DR = saddr << 1; /* transmit slave address */
	while (!(I2C1->SR1 & 2)); /* wait until addr flag is set */
	tmp = I2C1->SR2; /* clear addr flag */
	while (!(I2C1->SR1 & 0x80)); /* wait until data register empty */
	I2C1->DR = maddr; /* send memory address */
	while (!(I2C1->SR1 & 0x80)); /* wait until data register empty */
	I2C1->DR = data; /* transmit data */
	while (!(I2C1->SR1 & 4)); /* wait until transfer finished */
	I2C1->CR1 |= 0x200; /* generate stop */
	return 0;
}
