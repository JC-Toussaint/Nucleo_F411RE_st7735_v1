/* USER CODE BEGIN Header */
/**
It is SPI but with a single bidirectional pin e.g.
LED backlight, sometimes called BL. (it might be LED anode or cathode)
SDA bidirectional data pin. Connect to MOSI
SCK clock. sometimes called SCLK, SCL, ...
A0 data/command GPIO OUTPUT pin. sometimes called DC, RS, ...
RESET reset pin. you must connect. (or pullup)
CS chip select

ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
Since the provided driver doesn't use read capability from LCD, only constraint
on write baudrate is considered.

 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
/* vim: set ai et ts=4 sw=4: */
#include <string.h>
#include "st7735.h"
#include "fonts.h"
#include "testimg.h"
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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void init() {
	ST7735_Init();

	const char ready[] = "Ready!\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t*)ready, sizeof(ready)-1, HAL_MAX_DELAY);
}

void loop() {
	// Check border
	ST7735_FillScreen(ST7735_BLACK);

	for(int x = 0; x < ST7735_WIDTH; x++) {
		ST7735_DrawPixel(x, 0, ST7735_RED);
		ST7735_DrawPixel(x, ST7735_HEIGHT-1, ST7735_RED);
	}

	for(int y = 0; y < ST7735_HEIGHT; y++) {
		ST7735_DrawPixel(0, y, ST7735_RED);
		ST7735_DrawPixel(ST7735_WIDTH-1, y, ST7735_RED);
	}

	HAL_Delay(1000);

	// Check fonts
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_WriteString(0, 0, "Font_7x10, red on black, lorem ipsum dolor sit amet", Font_7x10, ST7735_RED, ST7735_BLACK);
	ST7735_WriteString(0, 3*10, "Font_11x18, green, lorem ipsum", Font_11x18, ST7735_GREEN, ST7735_BLACK);
	ST7735_WriteString(0, 3*10+3*18, "Font_16x26", Font_16x26, ST7735_BLUE, ST7735_BLACK);
	HAL_Delay(2000);

	// Check colors
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_WriteString(0, 0, "BLACK", Font_11x18, ST7735_WHITE, ST7735_BLACK);
	HAL_Delay(500);

	ST7735_FillScreen(ST7735_BLUE);
	ST7735_WriteString(0, 0, "BLUE", Font_11x18, ST7735_BLACK, ST7735_BLUE);
	HAL_Delay(500);

	ST7735_FillScreen(ST7735_RED);
	ST7735_WriteString(0, 0, "RED", Font_11x18, ST7735_BLACK, ST7735_RED);
	HAL_Delay(500);

	ST7735_FillScreen(ST7735_GREEN);
	ST7735_WriteString(0, 0, "GREEN", Font_11x18, ST7735_BLACK, ST7735_GREEN);
	HAL_Delay(500);

	ST7735_FillScreen(ST7735_CYAN);
	ST7735_WriteString(0, 0, "CYAN", Font_11x18, ST7735_BLACK, ST7735_CYAN);
	HAL_Delay(500);

	ST7735_FillScreen(ST7735_MAGENTA);
	ST7735_WriteString(0, 0, "MAGENTA", Font_11x18, ST7735_BLACK, ST7735_MAGENTA);
	HAL_Delay(500);

	ST7735_FillScreen(ST7735_YELLOW);
	ST7735_WriteString(0, 0, "YELLOW", Font_11x18, ST7735_BLACK, ST7735_YELLOW);
	HAL_Delay(500);

	ST7735_FillScreen(ST7735_WHITE);
	ST7735_WriteString(0, 0, "WHITE", Font_11x18, ST7735_BLACK, ST7735_WHITE);
	HAL_Delay(500);

#ifdef ST7735_IS_128X128
	// Display test image 128x128
	ST7735_DrawImage(0, 0, ST7735_WIDTH, ST7735_HEIGHT, (uint16_t*)test_img);

	/*
    // Display test image 128x128 pixel by pixel
    for(int x = 0; x < ST7735_WIDTH; x++) {
        for(int y = 0; y < ST7735_HEIGHT; y++) {
            uint16_t color565 = test_img_128x128[y][x];
            // fix endiness
            color565 = ((color565 & 0xFF00) >> 8) | ((color565 & 0xFF) << 8);
            ST7735_DrawPixel(x, y, color565);
        }
    }
	 */
#else
	uint16_t data[1];
	for (uint16_t i=0; i<ST7735_WIDTH; i++){
		for (uint16_t j=0; j<ST7735_HEIGHT; j++){
			/*        FEDCBA9876543210 */
			data[0]=0b1111100000000000;
			uint16_t color565 = test_img[i][j];
			color565 = ((color565 & 0xFF00) >> 8) | ((color565 & 0xFF) << 8);
			ST7735_DrawPixel(ST7735_WIDTH-1-i, j, color565);
		}
	}
	HAL_Delay(15000);
#endif // ST7735_IS_128X128

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
	MX_SPI1_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	init();
	while (1)
	{
		loop();
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
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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
	RCC_OscInitStruct.PLL.PLLQ = 4;
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
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_1LINE;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

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
	huart2.Init.BaudRate = 9600;
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
	HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : ST7735_RES_Pin */
	GPIO_InitStruct.Pin = ST7735_RES_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ST7735_RES_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : ST7735_DC_Pin */
	GPIO_InitStruct.Pin = ST7735_DC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ST7735_DC_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : ST7735_CS_Pin */
	GPIO_InitStruct.Pin = ST7735_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ST7735_CS_GPIO_Port, &GPIO_InitStruct);

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
	while(1)
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
