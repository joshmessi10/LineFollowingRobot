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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
typedef enum{
	LED1_Toggle=0,
	LED2_Toggle,
	LED3_Toggle,
	LED4_Toggle,
	TIEMPO_CALIBRACION,
	TIEMPO_MOTOR1,
	MOTOR1_TOGGLE,
	TIEMPO_MOTOR2,
	MOTOR2_TOGGLE,
	VUELTAS,
	TTOTAL
}TIMERS;

volatile uint32_t timers[TTOTAL];

//MAQUINA DE ESTADOS DE LED1
typedef enum{
	LED_ON = 0,
	LED_OFF,
	LED_STANDBY
}ST_LED1;

//estado principal del seguidor de linea
typedef enum{
	ESPERA=0,
	CALIBRACION,//1
	MOTOR_DERECHO,//2
	MOTOR_IZQUIERDO,//3
	SEGUIDOR_LINEA,//4
}MODO_TRABAJO;

typedef enum{
	INICIA_VAL=0,
	CAPTURA_DATOS
}ST_CAL;

typedef enum{
	INICIA=0,
	PRUEBA
}ST_MOTOR;

typedef enum{
INICIA_C=0,
AT,
CIPMODE,
CIPSTART,
CIPSEND,
CONECTED
}ST_CONECT;

ST_CONECT st_conexion=INICIA;
ST_LED1 st_led1 = LED_STANDBY, st_led2 = LED_STANDBY, st_led3 = LED_STANDBY, st_led4 = LED_STANDBY;
MODO_TRABAJO modo_trabajo=ESPERA;
ST_CAL st_calibracion=INICIA_VAL;
ST_MOTOR st_motor1=INICIA, st_motor2=INICIA;
uint8_t pulsador1 = 0, pulsador2 = 0, pulsador3 = 0, pulsador4 = 0;
volatile uint8_t datoRecibido[64], bufW[16];
int32_t valorPonderado = 0;
int32_t sumaPonderada = 0;
int32_t mediaPonderada = 0;
uint32_t adc[7], max[7], min[7];
uint8_t enviar[32];
uint8_t error=0, mot1=0, mot2=0, vueltas=0, motPast1=0, motPast2=0;
int32_t posi = 0;
uint8_t led[5];
int arr[16][4] = { {0, 0, 0, 0},
		{0, 0, 0, 1},
		{0, 0, 1, 0},
		{0, 0, 1, 1},
		{0, 1, 0, 0},
		{0, 1, 0, 1},
		{0, 1, 1, 0},
		{0, 1, 1, 1},
		{1, 0, 0, 0},
		{1, 0, 0, 1},
		{1, 0, 1, 0},
		{1, 0, 1, 1},
		{1, 1, 0, 0},
		{1, 1, 0, 1},
		{1, 1, 1, 0},
		{1, 1, 1, 1}
      };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start_DMA(&hadc1, adc, 7);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  TIM3->CCR2=0;
  TIM3->CCR1=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	  //conexionHandler();
	  ledHandler();
	  //memset(led,0,5);
	  //for(int i=0; i<5;i++){

		 // led[i]=(vueltas>>i);
	  //}

	  cuentaHandler();
	  seguidorHandler();
	  //comunicacionHandler();
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_10B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 7;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8400-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 16800-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 100-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	uint8_t i;
		for(i=0; i< TTOTAL; i++){
			if(timers[i] != 0){
				timers[i]--;
			}
		}
}
void ledHandle1(){
	switch(st_led1){
			case LED_STANDBY:
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
				if(timers[TIEMPO_CALIBRACION] != 0){
				st_led1 = LED_OFF;
				timers[LED1_Toggle] = 0;
				}
				break;

			case LED_OFF:
				if(timers[LED1_Toggle] == 0){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
				st_led1 = LED_ON;
				timers[LED1_Toggle] = 500;
				}
				if(timers[TIEMPO_CALIBRACION] == 0)
					st_led1=LED_STANDBY;
				break;

			case LED_ON:
				if(timers[LED1_Toggle] == 0){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
				st_led1 = LED_OFF;
				timers[LED1_Toggle] = 500;
				}
				if(timers[TIEMPO_CALIBRACION] == 0)
					st_led1=LED_STANDBY;
				break;
		}
}
void ledHandle2(){
	switch(st_led2){

			case LED_STANDBY:
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
				if(timers[TIEMPO_MOTOR1] != 0){
				st_led2 = LED_OFF;
				timers[LED2_Toggle] = 0;
				}
				break;

			case LED_OFF:
				if(timers[LED2_Toggle] == 0){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
				st_led2 = LED_ON;
				timers[LED2_Toggle] = 500;
				}
				if(timers[TIEMPO_MOTOR1] == 0)
					st_led2=LED_STANDBY;
				break;

			case LED_ON:
				if(timers[LED2_Toggle] == 0){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
				st_led2 = LED_OFF;
				timers[LED2_Toggle] = 500;
				}
				if(timers[TIEMPO_MOTOR1] == 0)
					st_led2=LED_STANDBY;
				break;
		}
}
void ledHandle3(){
	switch(st_led3){

			case LED_STANDBY:
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
				if(timers[TIEMPO_MOTOR2] != 0){
				st_led3 = LED_OFF;
				timers[LED3_Toggle] = 0;
				}
				break;

			case LED_OFF:
				if(timers[LED3_Toggle] == 0){
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
				st_led3 = LED_ON;
				timers[LED3_Toggle] = 500;
				}
				if(timers[TIEMPO_MOTOR2] == 0)
					st_led3=LED_STANDBY;
				break;

			case LED_ON:
				if(timers[LED3_Toggle] == 0){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
				st_led3 = LED_OFF;
				timers[LED3_Toggle] = 500;
				}
				if(timers[TIEMPO_MOTOR2] == 0)
					st_led3=LED_STANDBY;
				break;
		}
}
void ledHandle4(){
	switch(st_led4){

			case LED_STANDBY:
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
				if(modo_trabajo==ESPERA){
				st_led4 = LED_OFF;
				timers[LED4_Toggle] = 0;
				}
				break;

			case LED_OFF:
				if(timers[LED4_Toggle] == 0){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
				st_led4 = LED_ON;
				timers[LED4_Toggle] = 200;
				}
				if(modo_trabajo!=ESPERA)
					st_led4=LED_STANDBY;
				break;

			case LED_ON:
				if(timers[LED4_Toggle] == 0){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,1);
				st_led4 = LED_OFF;
				timers[LED4_Toggle] = 200;
				}
				if(modo_trabajo!=ESPERA)
					st_led4=LED_STANDBY;
				break;
		}
}
void ledHandler(){
		  ledHandle1();
		  ledHandle2();
		  ledHandle3();
		  ledHandle4();
}

void rutinaCalibracion(){
	switch(st_calibracion){
	case INICIA_VAL:
		for(int i=0;i<7;i++){
			max[i]=0;
			min[i]=1023;
		}
	    st_calibracion=CAPTURA_DATOS;
	    timers[TIEMPO_CALIBRACION]=15000;
		break;
	case CAPTURA_DATOS:
		for(int i=0;i<7;i++){
			if(max[i]<adc[i])
				max[i]=adc[i];
			if(min[i]>adc[i])
				min[i]=adc[i];
		}
		enviarQT(adc[0],adc[1],adc[2],adc[3],adc[4],adc[5],1);//seguidor
		enviarQT(max[0],max[1],max[2],max[3],max[4],max[5],2);//max
		enviarQT(min[0],min[1],min[2],min[3],min[4],min[5],3);//min
		if(timers[TIEMPO_CALIBRACION]==0){
			st_calibracion=INICIA_VAL;
			modo_trabajo=ESPERA;
		}
		break;
	}
}

void motorDerecho(){
	switch(st_motor1){
	case INICIA:
		TIM3->CCR1=0;
		st_motor1=PRUEBA;
		timers[TIEMPO_MOTOR1]=10000;
			break;
	case PRUEBA:
		if(timers[MOTOR1_TOGGLE]==0){
		TIM3->CCR1+=10;
		if((TIM3->CCR1)>100)
			TIM3->CCR1=0;
		timers[MOTOR1_TOGGLE]=500;
		}
		if(timers[TIEMPO_MOTOR1]==0){
			st_motor1=INICIA;
			modo_trabajo=ESPERA;
			TIM3->CCR1=0;
		}
		break;
	}
}

void motorIzquierdo(){
	switch(st_motor2){
	case INICIA:
			TIM3->CCR2=0;
			st_motor2=PRUEBA;
			timers[TIEMPO_MOTOR2]=10000;
				break;
		case PRUEBA:
			if(timers[MOTOR2_TOGGLE]==0){
			TIM3->CCR2+=10;
			if((TIM3->CCR2)>100)
				TIM3->CCR2=0;
			timers[MOTOR2_TOGGLE]=500;
			}
			if(timers[TIEMPO_MOTOR2]==0){
				st_motor2=INICIA;
				modo_trabajo=ESPERA;
				TIM3->CCR2=0;
			}
			break;
	}
}

int32_t rutinaPosicion(){
	int32_t valorPonderado=0, mediaPonderada=0;
	char negativo=0;
	for(int i=0;i<5;i++){
			if(max[i]<adc[i])
				max[i]=adc[i];
			if(min[i]>adc[i])
				min[i]=adc[i];
		}

	uint32_t adc0=adc[0]*(1023/(max[0]-min[0]))-(1023/(max[0]-min[0]))*min[0];
	uint32_t adc1=adc[1]*(1023/(max[1]-min[1]))-(1023/(max[1]-min[1]))*min[1];
	uint32_t adc2=adc[2]*(1023/(max[2]-min[2]))-(1023/(max[2]-min[2]))*min[2];
	uint32_t adc3=adc[3]*(1023/(max[3]-min[3]))-(1023/(max[3]-min[3]))*min[3];
	uint32_t adc4=adc[4]*(1023/(max[4]-min[4]))-(1023/(max[4]-min[4]))*min[4];

	valorPonderado = (int32_t)  (-2*adc0)+(-1*adc1)+(0*adc2)+(1*adc3)+(2*adc4);

	sumaPonderada = (int32_t) adc0+adc1+adc2+adc3+adc4;
		  if(valorPonderado < 0){
			  valorPonderado = valorPonderado * -1;
			  negativo = 1;
		  }

		  mediaPonderada = (int32_t)  75*valorPonderado/sumaPonderada;
		  if(negativo == 1){
			  mediaPonderada = mediaPonderada * -1;
		  }
	return mediaPonderada;
}

void seguidorLinea(){
	posi = rutinaPosicion();
	motPast1=mot1; motPast2=mot2;
	if(posi<0){
			if(posi<-10){
				mot1=10;
				mot2=60;
				}
			else if(posi<-20){
				mot1=5;
				mot2=60;
				}
			else if(adc[0]>500 && adc[1]>500 && adc[2]>500 && adc[3]>500 && adc[4]>500){
				if(motPast1<motPast2){
					mot1=2;
					mot2=60;
				}
				else {
					mot1=60;
					mot2=2;
				}
			}
			else{
				mot1=60;
				mot2=60;
			}
		}
		else{
			if(posi>10){
				mot1=60;
				mot2=10;
			}
			else if(posi>20){
				mot1=60;
				mot2=5;
			}
			else if(adc[0]>500 && adc[1]>500 && adc[2]>500 && adc[3]>500 && adc[4]>500){
				if(motPast1<motPast2){
					mot1=2;
					mot2=60;
				}
				else {
					mot1=60;
					mot2=2;
				}
			}
			else{
				mot1=60;
				mot2=60;
			}
		}
	if((timers[VUELTAS]==0)&&(adc[0]<500 && adc[1]<500 && adc[2]<500 && adc[3]<500 && adc[4]<500)){
		timers[VUELTAS]=3000;
		vueltas++;
		if(vueltas == 16){
			modo_trabajo = ESPERA;
		}
	}
	TIM3->CCR1=mot1;
	TIM3->CCR2=mot2;
}

void seguidorHandler(){
	switch(modo_trabajo){
	case ESPERA:

		TIM3->CCR1=0;
		TIM3->CCR2=0;
		break;
	case CALIBRACION:
		rutinaCalibracion();
		break;
	case MOTOR_DERECHO:
		motorDerecho();
		break;
	case MOTOR_IZQUIERDO:
		motorIzquierdo();
		break;
	case SEGUIDOR_LINEA:
		seguidorLinea();
		break;
	}
}

void enviarQT(uint32_t adc1, uint32_t adc2, uint32_t adc3, uint32_t adc4, uint32_t adc5, uint32_t adc6, uint8_t tipoDato){//PROTOCOLO (en bytes): 0. inicio(0x01), 1. tamaño (datos), 2 - (n+1).datos, n+2. checksum, n+3. final(0x02)
	uint8_t i=0;
	memset(enviar,0,32);
	enviar[0]=1;
	enviar[1]=8;
	//inicio datos:
	enviar[2]=adc1/4;
	enviar[3]=adc2/4;
	enviar[4]=adc3/4;
	enviar[5]=adc4/4;
	enviar[6]=adc5/4;
	enviar[7]=tipoDato;
	enviar[8]=adc6;
	//fin datos.
	for(i=0;i<9;i++){
		enviar[9]+=enviar[i];
	}
	enviar[10]=2;
	CDC_Transmit_FS(enviar,strlen(enviar));
	HAL_UART_Transmit(&huart1, enviar, strlen(enviar), 1);
}
void llegaDato(uint8_t* Buf, uint32_t Len){//PROTOCOLO (en bytes): 0. inicio(0x01), 1. tamaño (datos), 2 - (n+1).datos, n+2. checksum, n+3. final(0x02)
	memset(datoRecibido,'\0',256);
		error=0;
		if(Buf[0]!=0x01){
			error=1;
		}
		else{
			uint8_t i, checksum=Buf[0]+Buf[1];
			for(i=0;i<Len-4;i++){//Buf[1]
				datoRecibido[i]=Buf[i+2];
				checksum+=Buf[i+2];
			}
			if(checksum!=Buf[Len-2]){//Buf[1]+2
				error=1;
			}
			if(Buf[Len-1]!=0x02){//Buf[1]+3
				error=1;
			}
		}
		if(error!=1){//PROTOCOLO V2: byte 1 de dato recibido es modo de trabajo
				if((modo_trabajo==SEGUIDOR_LINEA)&&(datoRecibido[0]==4)){
					modo_trabajo=ESPERA;
				}
				else{
					modo_trabajo=datoRecibido[0];
				}
			    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
			  }
			  else{
				  CDC_Transmit_FS("NO",strlen("NO"));
				  HAL_UART_Transmit(&huart1, "NO", strlen("NO"), 1);
				  //modo_trabajo=datoRecibido[0];
			  }
}

void cuentaHandler(){
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,arr[vueltas][0]);
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,arr[vueltas][1]);
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,arr[vueltas][2]);
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,arr[vueltas][3]);
}

/*void conexionHandler(){
uint8_t envi[32];
uint8_t recib[32];
	switch(st_conexion){
		case INICIA_C:
			envi="AT+CIPSTATUS";
			HAL_UART_Transmit(&huart1, envi, strlen(envi), 1);
            HAL_UART_Receive(&huart1, recib, 32, 2);
			if(recib!="STATUS:0"){
				st_conexion=AT;
			}
			break;
		case AT:
			envi="AT";
			HAL_UART_Transmit(&huart1, envi, strlen(envi), 1);
            HAL_UART_Receive(&huart1, recib, 32, 2);
			if(recib=="OK"){
				st_conexion=CIPMODE;
			}
			break;
		case CIPMODE:
			envi="AT+CIPMODE=1";
			HAL_UART_Transmit(&huart1, envi, strlen(envi), 1);
            HAL_UART_Receive(&huart1, recib, 32, 2);
			if(recib=="OK"){
				st_conexion=CIPSTART;
			}
			break;
		case CIPSTART:
			envi="AT+CIPSTART=\"TCP\",\"192.168.1.1\",1000";
			HAL_UART_Transmit(&huart1, envi, strlen(envi), 1);
            HAL_UART_Receive(&huart1, recib, 32, 2);
			if(recib=="OK"){
				st_conexion=CIPSEND;
			}
			break;
		case CIPSEND:
			envi="AT+CIPSEND";
			HAL_UART_Transmit(&huart1, envi, strlen(envi), 1);
            HAL_UART_Receive(&huart1, recib, 32, 2);
			if(recib==">"){
				st_conexion=CONECTED;
			}
			break;
		case CONECTED:
			  memset(bufW,0,32);
			  HAL_UART_Receive(&huart1, bufW, 32, 1);
			  if(bufW[2]!=0)
			  llegaDato(bufW,5);
			break;
	}
}*/

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
