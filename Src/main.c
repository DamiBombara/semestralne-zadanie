/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>



// I2C slave device useful information

char buffer[] = "Funguje komunikacia\n\r" ;
char mod1[] = "chmod 1";
char mod2[] = "chmod 2";
uint8_t stringData[35];
uint8_t mode;

char tx_data[1000];
void SystemClock_Config(void);

void proccesDmaData(uint8_t* data, uint16_t len, uint16_t pos);


int main(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	SystemClock_Config();

	MUSinit();
	MX_USART2_UART_Init();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();

	USART2_RegisterCallback(proccesDmaData);

	mode = 1;
	//playNWA(2);

	sprintf(tx_data, "*****ZAHRATIE FAREBNEJ MELODIE - SEMESTRALNE ZADANIE*****\r\n\n"
	                    "DOSTUPNE MODY: \r\n"
	                    "1 - NAPISTE RETAZEC POUZITELNYCH ZNAKOV A STLACTE ENTER\r\n"
	                    "2 - KLIKNITE JEDEN Z POUZITELNYCH ZNAKOV A PREHRA SA VAM TON (PIANO)\r\n\n"
	                    "AKTUALNE NASTAVENY MOD: 1\r\n\n"
	                    "PRE ZMENENIE MODU POUZITE PRIKAZ: chmod 1 alebo chmod 2\r\n\n"
	                    "POUZITELNE ZNAKY: \r\n"
	                    "• a - C7  \r\n"
	                    "• s - C#7 \r\n"
	                    "• d - D7  \r\n"
	                    "• f - D#7 \r\n"
	                    "• g - E7  \r\n"
	                    "• h - F7  \r\n"
	                    "• j - F#7 \r\n"
	                    "• k - G7  \r\n"
	                    "• l - G#7 \r\n"
	                    "• u - A7  \r\n"
	                    "• i - A#7 \r\n"
	                    "• o - B7  \r\n\n");

    USART2_PutBuffer(tx_data, sizeof(tx_data));

  while (1)
  {
	  //playNWA(1);
	  //USART2_PutBuffer(buffer, sizeof(buffer));
	  //LL_mDelay(2000);
  }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}

void proccesDmaData(uint8_t* data, uint16_t len, uint16_t pos) {
	uint8_t j = 0;
	for (uint8_t i = 0; i < len; i++) {
		if (*(data+i) == '\n' || *(data+i) == '\r') continue;
		stringData[j] = (*(data+i));
		j++;
	}
	stringData[j] = '\0';

	if (strcmp(stringData, mod1) == 0) {
		mode = 1;
		char buff[77];
		sprintf(buff, "Aktivovali ste mod: 1 - NAPISTE RETAZEC POUZITELNYCH ZNAKOV A STLACTE ENTER\r\n");
		USART2_PutBuffer(buff, sizeof(buff));
	}

	else if (strcmp(stringData, mod2) == 0) {
		mode = 2;
		char buff[95];
		sprintf(buff, "Aktivovali ste mod: 2 - KLIKNITE JEDEN Z POUZITELNYCH ZNAKOV A PREHRA SA VAM TON (PIANO)\r\n");
		USART2_PutBuffer(buff, sizeof(buff));
	}

	else {
		if (mode == 1) playString(stringData, 300, 100);
		if (mode == 2) playString(stringData, 300, 100);
	}
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
