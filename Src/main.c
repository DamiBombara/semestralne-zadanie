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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MAX_LED 8
#define USE_BRIGHTNESS 1

uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4];
int datasentflag = 0;

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	datasentflag=1;
}

void Set_LED (int LEDnum, int Red, int Green, int Blue)

{
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = Green;
	LED_Data[LEDnum][2] = Red;
	LED_Data[LEDnum][3] = Blue;
}

#define PI 3.14159265

void Set_Brightness (int brightness)  // 0-45
{
	if (brightness > 45) brightness = 45;
	for (int i=0; i<MAX_LED; i++)
	{
		LED_Mod[i][0] = LED_Data[i][0];
		for (int j=1; j<4; j++)
		{
			float angle = 90-brightness;  // in degrees
			angle = angle*PI / 180;  // in rad
			LED_Mod[i][j] = (LED_Data[i][j])/(tan(angle));
		}
	}
}

uint16_t pwmData[(24*MAX_LED)+50];

void WS2812_Send (void)
{
	uint32_t indx=0;
	uint32_t color;

	for (int i= 0; i<MAX_LED; i++)
	{
		color = ((LED_Mod[i][1]<<16) | (LED_Mod[i][2]<<8) | (LED_Mod[i][3]));		// 24bit
		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				pwmData[indx] = 7;  // 2/3 of 10
			}

			else pwmData[indx] = 3;  // 1/3 of 10
			indx++;
		}

	}

	for (int i=0; i<50; i++)				// reset
	{
		pwmData[indx] = 0;
		indx++;
	}

	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);
	while (!datasentflag){};
	datasentflag = 0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
char buffer[] = "Funguje komunikacia\n\r" ;
char mod1[] = "chmod 1";
char mod2[] = "chmod 2";
char easterEgg[] = "eazy-e";
uint8_t stringData[35];
uint8_t mode;

char tx_data[1000];
void SystemClock_Config(void);

void proccesDmaData(uint8_t* data, uint16_t len, uint16_t pos);

int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init1();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  Set_LED(0, 255, 0,0);
  Set_Brightness(45);
  WS2812_Send();

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
  	                    "• o - B7  \r\n\n"
  						"Easter egg pri mode 1: Kto zalozil znamu skupinu N.W.A? (Napoveda: je to fakt 'eazy')\r\n\n");

      USART2_PutBuffer(tx_data, sizeof(tx_data));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_TIM1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void proccesDmaData(uint8_t* data, uint16_t len, uint16_t pos){

  if(mode == 1){
	  uint8_t j = 0;
	  	for (uint8_t i = 0; i < len; i++) {
	  		if (*(data+i) == '\n' || *(data+i) == '\r') continue;
	  		stringData[j] = (*(data+i));
	  		j++;
	  	}
	  	stringData[j] = '\0';


    if(strcmp("chmod",stringData) == 0){
      mode = 2;
    }
    else{
      playString(stringData, 300, 100);
    }



    //USART2_PutBuffer(stringData, sizeof(stringData));

  }

  else if(mode == 2){
      char tone = (char) data[0];

      /*playTone(returnFreguency(tone), 300);

      if(tone == 'c'){
        mode = 1;
      }*/

    if(isPianoKey(tone)){
    	startTone(returnFreguency(tone));
    }
    else if(tone == STOP_SIGNAL){
    	stopTone(tone);
    }

    else if(tone == CHANGE_KEY){
      mode = 1;
    }


  }
}
/* USER CODE END 4 */

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
