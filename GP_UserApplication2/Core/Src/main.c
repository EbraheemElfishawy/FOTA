/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
uint8_t RxData[100];
uint32_t volatile flag=0;
char *p;
uint32_t volatile SeriaRecieve2;
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
 UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void blink(void);
void blink2(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    //HAL_UART_Receive(&huart3,&SeriaRecieve2, 1,1000 );

    if(flag==1)
    {
  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
  	  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);

  	  HAL_UART_Receive(&huart3,&SeriaRecieve2, 1,1000 );
  	  if (SeriaRecieve2 == 0)
  	  	{
  	  		blink2();
  	  	}
  	  	else if (SeriaRecieve2 == 1)
  	  	{
  	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
  	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
  	  		bootloader_jump_to_user_app1();
  	  	}
  	  	else if (SeriaRecieve2 == 2)
  	  	{
  	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);// Turn OFF User LED to indicate that we left Bootloader
  	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);

  	  		bootloader_jump_to_user_app2();
  	  	}
  	  	else if (SeriaRecieve2 == 4)
  	  	{
  	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, 1);// Turn OFF User LED to indicate that we left Bootloader
  	  		bootloader_jump_to_bootloader();
  	  	}
  	  flag=0;
  	  __HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);

    }
  }
    }
    /* USER CODE END 3 */

  void blink(void)
  {
  	uint32_t current_tick1;
  	current_tick1= HAL_GetTick();
  	for(uint8_t i=0; i<5; i++)
  	{
  		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
  		current_tick1 = HAL_GetTick();
  		while(HAL_GetTick() <= (current_tick1+100));
  		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
  		current_tick1 = HAL_GetTick();
  		while(HAL_GetTick() <= (current_tick1+500));
  	}
  }
  void blink2(void)
  {
  	//uint32_t current_tick1;
  	//current_tick1= HAL_GetTick();
  	for(uint8_t i=0; i<10; i++)
  	{
  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
  		//current_tick1 = HAL_GetTick();
  		//while(HAL_GetTick() <= (current_tick1+50));
  		for(uint32_t i=0;i<10000000;i=i+32);
  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
  		for(uint32_t i=0;i<10000000;i=i+32);

  		//current_tick1 = HAL_GetTick();
  		//while(HAL_GetTick() <= (current_tick1+50));
  	}
  }

  /*code to jump to user application
   *Here we are assuming FLASH_SECTOR2_BASE_ADDRESS
   *is where the user application is stored
   */
  void bootloader_jump_to_user_app1(void)
  {
     //just a function pointer to hold the address of the reset handler of the user app.
      void (*app_reset_handler)(void);
      printmsg("BL_DEBUG_MSG:bootloader_jump_to_user_app\n");
      // 1. configure the MSP by reading the value from the base address of the sector 2
      uint32_t msp_value = *(volatile uint32_t *)FLASH_SECTOR2_BASE_ADDRESS;
      printmsg("BL_DEBUG_MSG:MSP value : %#x\n",msp_value);
      //This function comes from CMSIS.
      __set_MSP(msp_value);
      //SCB->VTOR = FLASH_SECTOR2_BASE_ADDRESS;
      /* 2. Now fetch the reset handler address of the user application
       * from the location FLASH_SECTOR2_BASE_ADDRESS+4
       */
      uint32_t resethandler_address = *(volatile uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS + 4);
      app_reset_handler = (void*) resethandler_address;
      printmsg("BL_DEBUG_MSG: app reset handler addr : %#x\n",app_reset_handler);
      //3. jump to reset handler of the user application
      app_reset_handler();

  }
  void bootloader_jump_to_user_app2(void)
  {

     //just a function pointer to hold the address of the reset handler of the user app.
      void (*app_reset_handler)(void);
      printmsg("BL_DEBUG_MSG:bootloader_jump_to_user_app\n");
      // 1. configure the MSP by reading the value from the base address of the sector 5
      uint32_t msp_value = *(volatile uint32_t *)FLASH_SECTOR5_BASE_ADDRESS;
      printmsg("BL_DEBUG_MSG:MSP value : %#x\n",msp_value);
      //This function comes from CMSIS.
      __set_MSP(msp_value);
      //SCB->VTOR = FLASH_SECTOR5_BASE_ADDRESS;
      /* 2. Now fetch the reset handler address of the user application
       * from the location FLASH_SECTOR5_BASE_ADDRESS+4
       */
      uint32_t resethandler_address = *(volatile uint32_t *) (FLASH_SECTOR5_BASE_ADDRESS + 4);
      app_reset_handler = (void*) resethandler_address;
      printmsg("BL_DEBUG_MSG: app reset handler addr : %#x\n",app_reset_handler);
      //3. jump to reset handler of the user application
      app_reset_handler();
  }
  void bootloader_jump_to_bootloader(void)
  {

     //just a function pointer to hold the address of the reset handler of the user app.
      void (*app_reset_handler)(void);
      printmsg("BL_DEBUG_MSG:bootloader_jump_to_user_app\n");
      // 1. configure the MSP by reading the value from the base address of the sector 0
      uint32_t msp_value = *(volatile uint32_t *)FLASH_SECTOR0_BASE_ADDRESS;
      printmsg("BL_DEBUG_MSG:MSP value : %#x\n",msp_value);
      //This function comes from CMSIS.
      __set_MSP(msp_value);
      //SCB->VTOR = FLASH_SECTOR0_BASE_ADDRESS;
      /* 2. Now fetch the reset handler address of the user application
       * from the location FLASH_SECTOR0_BASE_ADDRESS+4
       */
      uint32_t resethandler_address = *(volatile uint32_t *) (FLASH_SECTOR0_BASE_ADDRESS + 4);
      app_reset_handler = (void*) resethandler_address;
      printmsg("BL_DEBUG_MSG: app reset handler addr : %#x\n",app_reset_handler);
      //3. jump to reset handler of the user application
      app_reset_handler();
  }
  /* prints formatted string to console over UART */
   void printmsg(char *format,...)
   {
  	#ifdef BL_DEBUG_MSG_EN
  	 char str[80];
  	 /*Extract the the argument list using VA apis */
  	 va_list args;
  	 va_start(args, format);
  	 vsprintf(str, format,args);
  	 HAL_UART_Transmit(D_UART,(uint8_t *)str, strlen(str),HAL_MAX_DELAY);
  	 va_end(args);
  	#endif
   }

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
  __HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
