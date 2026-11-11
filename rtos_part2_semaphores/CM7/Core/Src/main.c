/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "string.h"
#include "stdio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* DUAL_CORE_BOOT_SYNC_SEQUENCE: Define for dual core boot synchronization    */
/*                             demonstration code based on hardware semaphore */
/* This define is present in both CM7/CM4 projects                            */
/* To comment when developping/debugging on a single core                     */
#define DUAL_CORE_BOOT_SYNC_SEQUENCE

#if defined(DUAL_CORE_BOOT_SYNC_SEQUENCE)
#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif
#endif /* DUAL_CORE_BOOT_SYNC_SEQUENCE */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart3;

/* Definitions for LPT */
osThreadId_t LPTHandle;
const osThreadAttr_t LPT_attributes = {
  .name = "LPT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for MPT */
osThreadId_t MPTHandle;
const osThreadAttr_t MPT_attributes = {
  .name = "MPT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for HPT */
osThreadId_t HPTHandle;
const osThreadAttr_t HPT_attributes = {
  .name = "HPT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for VHP */
osThreadId_t VHPHandle;
const osThreadAttr_t VHP_attributes = {
  .name = "VHP",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for myBinarySem01 */
osSemaphoreId_t myBinarySem01Handle;
const osSemaphoreAttr_t myBinarySem01_attributes = {
  .name = "myBinarySem01"
};
/* Definitions for myCountingSem01 */
osSemaphoreId_t myCountingSem01Handle;
const osSemaphoreAttr_t myCountingSem01_attributes = {
  .name = "myCountingSem01"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
void START_LPT(void *argument);
void Start_MPT(void *argument);
void START_HPT(void *argument);
void START_VHP(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct {
	uint32_t timestamp;
	uint32_t event_id;
}message_q_t;
int _io_putchar(uint8_t ch){
	HAL_UART_Transmit(&huart3,&ch, 1, HAL_MAX_DELAY);
	return ch;
}
 int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
	  _io_putchar(*ptr++);
  }
  return len;
}

 int resources[3]={100,200,300};
 char *resourceowner[3]={"free","free","free"};
 int getResource(char *taskName){
	 for(int i=0;i<3;i++){
		 if(strcmp(resourceowner[i],"free")==0)
		 {
			 resourceowner[i]=taskName;
			 return i;
		 }
	 }
	 return -1;
 }
 void release_resource(int id){
	 resourceowner[id]="free";
 }
 void printResourceTable(void){
	 printf("\nResource Table \n");
	 for(int i=0;i<3;i++){
		 printf("resource %d -> %s\n",resources[i],resourceowner[i]);
	 }
	 printf("\n");

 }
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
//#if defined(DUAL_CORE_BOOT_SYNC_SEQUENCE)
//  int32_t timeout;
//#endif /* DUAL_CORE_BOOT_SYNC_SEQUENCE */
/* USER CODE END Boot_Mode_Sequence_0 */

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
//#if defined(DUAL_CORE_BOOT_SYNC_SEQUENCE)
//  /* Wait until CPU2 boots and enters in stop mode or timeout*/
//  timeout = 0xFFFF;
//  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
//  if ( timeout < 0 )
//  {
//  Error_Handler();
//  }
//#endif /* DUAL_CORE_BOOT_SYNC_SEQUENCE */
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
//#if defined(DUAL_CORE_BOOT_SYNC_SEQUENCE)
///* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
//HSEM notification */
///*HW semaphore Clock enable*/
//__HAL_RCC_HSEM_CLK_ENABLE();
///*Take HSEM */
//HAL_HSEM_FastTake(HSEM_ID_0);
///*Release HSEM in order to notify the CPU2(CM4)*/
//HAL_HSEM_Release(HSEM_ID_0,0);
///* wait until CPU2 wakes up from stop mode */
//timeout = 0xFFFF;
//while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
//if ( timeout < 0 )
//{
//Error_Handler();
//}
//#endif /* DUAL_CORE_BOOT_SYNC_SEQUENCE */
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myBinarySem01 */
  myBinarySem01Handle = osSemaphoreNew(1, 1, &myBinarySem01_attributes);

  /* creation of myCountingSem01 */
  myCountingSem01Handle = osSemaphoreNew(3, 3, &myCountingSem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LPT */
  LPTHandle = osThreadNew(START_LPT, NULL, &LPT_attributes);

  /* creation of MPT */
  MPTHandle = osThreadNew(Start_MPT, NULL, &MPT_attributes);

  /* creation of HPT */
  HPTHandle = osThreadNew(START_HPT, NULL, &HPT_attributes);

  /* creation of VHP */
  VHPHandle = osThreadNew(START_VHP, NULL, &VHP_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

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
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_START_LPT */
/**
  * @brief  Function implementing the LPT thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_START_LPT */
void START_LPT(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */

	uint32_t wait;
	int resid;
  for(;;)
  {
	  printf("LPT WAITING FOR RESOURCES \r\n");
	  osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);
	  resid=getResource("LPT");
	  printf("LPT ACCESSING RESOURCES%d \n",resources[resid]);
	  printResourceTable();
	  osDelay(2000);
	  printf("LPT FINISHED USING RESOURCES\n");
	  release_resource(resid);
	  osSemaphoreRelease(myCountingSem01Handle);
	  osDelay(1000);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Start_MPT */
/**
* @brief Function implementing the MPT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_MPT */
void Start_MPT(void *argument)
{
	int resid;
  /* USER CODE BEGIN Start_MPT */
  /* Infinite loop */
  for(;;)
  {
//   printf("MPT IS ACTIVE AND TRYING TO ACQUIRE SEMAPHORE\r\n");
//   osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
//   printf("SEMAPHORE ACQUIRED SUCCESSFULLY\r\n");
//   osSemaphoreRelease(myBinarySem01Handle);
//   printf("released semaphore and going to sleep\r\n");
//   osDelay(200);
	  /*second for iterating*/
//	  printf("IN MPT \r\n");
//	  wait=50000000;
//	  while(wait--);
//	  printf("MPT FINISHED\r\n");
//	  osDelay(500);
	  printf("MPT WAITING FOR RESOURCES \r\n");
	 	  osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);
	 	  resid=getResource("MPT");
	 	  printf("MPT ACCESSING RESOURCES%d \n",resources[resid]);
	 	  printResourceTable();
	 	  osDelay(2000);
	 	  printf("MPT FINISHED USING RESOURCES\n");
	 	  release_resource(resid);
	 	  osSemaphoreRelease(myCountingSem01Handle);
	 	  osDelay(1000);
  }
  /* USER CODE END Start_MPT */
}

/* USER CODE BEGIN Header_START_HPT */
/**
* @brief Function implementing the HPT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_START_HPT */
void START_HPT(void *argument)
{
	int resid;
  /* USER CODE BEGIN START_HPT */
  /* Infinite loop */
  for(;;)
  {

//	  printf("HPT IS ACTIVE AND TRYING TO ACQUIRE SEMAPHORE\r\n");
//	   osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
//	   printf("SEMAPHORE ACQUIRED SUCCESSFULLY\r\n");
//	   osSemaphoreRelease(myBinarySem01Handle);
//	   printf("released semaphore and going to sleep\r\n");
//    osDelay(200);
	  printf("HPT WAITING FOR RESOURCES \r\n");
	 	  osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);
	 	  resid=getResource("HPT");
	 	  printf("HPT ACCESSING RESOURCES%d \n",resources[resid]);
	 	  printResourceTable();
	 	  osDelay(2000);
	 	  printf("HPT FINISHED USING RESOURCES\n");
	 	  release_resource(resid);
	 	  osSemaphoreRelease(myCountingSem01Handle);
	 	  osDelay(1000);
  }
  /* USER CODE END START_HPT */
}

/* USER CODE BEGIN Header_START_VHP */
/**
* @brief Function implementing the VHP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_START_VHP */
void START_VHP(void *argument)
{
	int resid;
  /* USER CODE BEGIN START_VHP */
  /* Infinite loop */
  for(;;)
  {
	  printf("VHPT WAITING FOR RESOURCES \r\n");
	 	  osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);
	 	  resid=getResource("VHPT");
	 	  printf("VHPT ACCESSING RESOURCES%d \n",resources[resid]);
	 	  printResourceTable();
	 	  osDelay(2000);
	 	  printf("VHPT FINISHED USING RESOURCES\n");
	 	  release_resource(resid);
	 	  osSemaphoreRelease(myCountingSem01Handle);
	 	  osDelay(1000);
  }
  /* USER CODE END START_VHP */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
#ifdef USE_FULL_ASSERT
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
