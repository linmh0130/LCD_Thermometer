#include "stm8l15x.h"
#include "ht1621.h"

/*
void LED_Config(void)
{
	GPIOC->DDR |= 0x10;
	GPIOC->CR1 |= 0x10;
	GPIOC->CR2 |= 0x00;
	GPIOC->ODR = 0x00;
}*/

void TIM2_Config(void)
{
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE); // Enable Peripheral Clock for TIM2
	TIM2->IER = 0x01; // TIM2 interupt enable
	TIM2->EGR = 0x01; // TIM2 upload event generation
	TIM2->PSCR = 0x07; // Prescaler, 2M/(2^5) = 62500
	TIM2->ARRH = 244; // 122*256+18 = 31250
	TIM2->ARRL = 36;
	TIM2->CR1 = 0x01; // TIM2 enable
	// As CLK_CKDIVR = 0x18 after reset, High speed internal (HSI)
	// clock is prescaled by 8, then output is 2MHz when fosi = 16MHz
}

void HT1621Port_Init(void)
{
	GPIOB->DDR|=0x0e;
	GPIOB->CR1|=0x0e;
	GPIOB->CR2|=0x00;
}


void I2C_Config(void)
{
	CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);
	I2C_DeInit(I2C1);
	I2C_Init(I2C1, 100000, 0xA0,
           I2C_Mode_I2C, I2C_DutyCycle_2,
           I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
	// I2C_ITConfig(I2C1, (I2C_IT_TypeDef)(I2C_IT_EVT | I2C_IT_BUF), ENABLE);
	// master receiver without interrupt
}


#define SLAVE_ADDRESS 0x90
__IO uint8_t RxBuffer[32];
__IO uint8_t NumOfBytes = 1;
__IO uint8_t Rx_Idx =0;
char Welcome[] = "\033[2J\033[1;1HLM/75TMP275 Receiver Demo\n";

void Read_I2C(void)
{
	//while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1, ENABLE); // Start and into Master Mode
	/* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	/* Send slave Address for write */
	
  I2C_Send7bitAddress(I2C1, SLAVE_ADDRESS, I2C_Direction_Receiver);
  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	/* While there is data to be read */
	//GPIOC->ODR = 0x00;
	
	while(NumOfBytes)
	{
		/* The last bytes need STOP but not ACK */
		if (NumOfBytes == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);

      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);

      /* Poll on RxNE Flag */
      while ((I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET));
			/* Read a byte */
			RxBuffer[Rx_Idx++] = I2C_ReceiveData(I2C1);
			/* Decrement the read bytes counter */
			NumOfBytes--;
    }
    /* Test on EV7 and clear it */
    if (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) )
    {
			/* Read a byte */
			RxBuffer[Rx_Idx++] = I2C_ReceiveData(I2C1);
			/* Decrement the read bytes counter */
			NumOfBytes--;
    }
  }
	Rx_Idx = 0;
}

void main(void)
{
	uint8_t data;
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
	//UART_SendStr(Welcome);
	//LED_Config();
	I2C_Config();
	HT1621Port_Init();
	Ht1621_VCC_1();
	Ht1621_GND_0();
	Ht1621_Init();        // Initialize LCD
  Delay(100);           // Delay for a few time
	ClearLCD();           // Clear LCD display
	TIM2_Config();
	rim();
	
	
	Read_I2C();
	data = RxBuffer[0];
	//GPIOC->ODR = 0x10;
	n4 = HT1621_VOID;
	if ((data&0x80)!=0)
	{
		data = (data^0xff)+1;
		n4 = HT1621_MINUS;
	}
	n3 = data/10;
	n2 = data%10;
	n1 = 10;
	Ht1621WrOneData(7 , 0x10);
	Display();
	wfi();
	//Delayms(2000);
	//WWDG->CR |= 0x80;
	//I2C_SoftwareResetCmd(I2C1,ENABLE);
	//I2C_DeInit(I2C1);
}

@far @interrupt void TIM2_UPD_OVF_BRK_IRQHandler (void)
{
	TIM2->SR1 = 0x00;
	WWDG->CR |= 0x80; // Software Reset
	return;
}