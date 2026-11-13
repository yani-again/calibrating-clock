/* USER CODE BEGIN Header */

// MSF SIGNAL SOURCES:
// - https://www.npl.co.uk/getattachment/2afd4c3b-fc8d-486c-b2b9-fc9a5a66394c/msf-time-date-code-2019.pdf
// - https://timetoolsltd.com/atomic-clocks/decoding-the-msf-60-radio-time-signal/

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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <assert.h>
#include "sn74.h"
#include "simple_stm32g0xx.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// SN74HC595N shift register definitions
#define SN74_SER_SRCLK_PORT  SIMPLE_GPIOA
#define SN74_RCLK_PORT       SIMPLE_GPIOA
#define SN74_SER_PIN         0U
#define SN74_SRCLK_PIN       1U

// longwave radio transceiver definitions
#define LW_PORT             'A'    // LWR = LongWave
#define LW_DATA_PIN         6U      // labelled 'T'  on module
#define LW_ENABLE_PIN       7U      // labelled 'P1' on module

// fonts
static const uint8_t FONTS[10] = {0xFC, 0x30, 0xDA, 0x7A, 0x36, 0x6E, 0xEE, 0x38, 0xFE, 0x7E};

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

/*
 * !!!!!!!!!!!!!!!!!!!!!
 * !!!!! IMPORTANT !!!!!
 * !!!!!!!!!!!!!!!!!!!!!
 * PA0: SER
 * PA1: SRCLK
 * PA2-5: RCLK
 */
// IDs for which SN74 register corresponds to which which LED
typedef enum
{
    SN74_pin_hourTens = 2,
    SN74_pin_hourOnes = 3,
    SN74_pin_minTens  = 4,
    SN74_pin_minOnes  = 5
} SN47_PinIDs;

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
    /* USER CODE BEGIN 2 */

    // LSE
    CC_LSE_Init();

    // GPIO
    CC_GPIO_Init();

    // default screen to 23:59 at startup/reset
    CC_setTime(2, 3, 5, 9);

    // EXTI and NVIC configure
    simple_exti_config_t exti_config;
    exti_config.port = LW_PORT;
    exti_config.pin = LW_DATA_PIN;
    exti_config.fallingTrigger_enable = true;
    exti_config.risingTrigger_enable = true;
    exti_config.interrupt_enable = true;
    exti_config.event_enable = false;

    simple_exti_configure(&exti_config);

    // using this module: 
    // https://www.xtals.co.uk/product-page/wwvb-60khz-single-frequency-modules-radio-time-signal-receiver
    //
    // pin connections and their uses:
    // - G: GND
    // - V: 3V3
    // - T: data line (data received by MCU)
    // - P1: on/off switch (logic low = receiver running)

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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void CC_GPIO_Init(void)
{
    // PA0 = SER
    // PA1 = SRCLK
    // PA2-5 = RCLK
    simple_gpio_config_t gpio_config_leds;

    gpio_config_leds.mode = simple_gpio_mode_output;
    gpio_config_leds.outputType = simple_gpio_out_pushpull;
    gpio_config_leds.pupd = simple_gpio_pupd_none;
    gpio_config_leds.speed = simple_gpio_speed_vlow;

    // SER config
    gpio_config_leds.pin = SN74_SER_PIN;
    simple_gpio_configure(SN74_SER_SRCLK_PORT, &gpio_config_leds);

    // SRCLK config
    gpio_config_leds.pin = SN74_SRCLK_PIN;
    simple_gpio_configure(SN74_SER_SRCLK_PORT, &gpio_config_leds);

    // RCLK config
    // hour tens
    gpio_config_leds.pin = SN74_pin_hourTens;
    simple_gpio_configure(SN74_RCLK_PORT, &gpio_config_leds);
    // hour ones
    gpio_config_leds.pin = SN74_pin_hourOnes;
    simple_gpio_configure(SN74_RCLK_PORT, &gpio_config_leds);
    // minute tens
    gpio_config_leds.pin = SN74_pin_minTens;
    simple_gpio_configure(SN74_RCLK_PORT, &gpio_config_leds);
    // minute ones
    gpio_config_leds.pin = SN74_pin_minOnes;
    simple_gpio_configure(SN74_RCLK_PORT, &gpio_config_leds);

    return;
}

void CC_LSE_Init(void)
{
    simple_clock_lseConfig_t lseConfig;
    lseConfig.bypassEnable = false;
    lseConfig.enable = true;
    simple_clock_configureLse(&lseConfig);
}

void CC_setTime(uint8_t ht, uint8_t ho, uint8_t mt, uint8_t mo)
{
    assert(ht <= 2);
    assert(ho <= 9);
    assert(mt <= 5);
    assert(mo <= 9);

    sn74_write(SN74_SER_SRCLK_PORT, SN74_SER_PIN, SN74_SRCLK_PIN, FONTS[ht]);
    sn74_latch(SN74_RCLK_PORT, SN74_pin_hourTens);
    sn74_write(SN74_SER_SRCLK_PORT, SN74_SER_PIN, SN74_SRCLK_PIN, FONTS[ho]);
    sn74_latch(SN74_RCLK_PORT, SN74_pin_hourOnes);
    sn74_write(SN74_SER_SRCLK_PORT, SN74_SER_PIN, SN74_SRCLK_PIN, FONTS[mt]);
    sn74_latch(SN74_RCLK_PORT, SN74_pin_minTens);
    sn74_write(SN74_SER_SRCLK_PORT, SN74_SER_PIN, SN74_SRCLK_PIN, FONTS[mo]);
    sn74_latch(SN74_RCLK_PORT, SN74_pin_minOnes);
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
     eg: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
