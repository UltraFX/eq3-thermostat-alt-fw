/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define TIM1_PERIOD                  199
#define TIM1_PRESCALER               10
#define TIM1_REPTETION_COUNTER       0

#define	ADC_MOTOR_CHANNEL						 ADC_Channel_24	/**< ADC Channel of the Motor */

/* Private variables ---------------------------------------------------------*/
volatile uint32_t dwTick = 0;
volatile uint8_t byTrigger = 0;
volatile uint32_t dwUartTimeout = 0;
volatile uint8_t	byRecAction = 0;

protocol_t sProtocol;

//static uint8_t byDirection = DIR_OPEN;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void delay_ms(uint32_t dwDelay)
{
	uint32_t dwEnd = dwTick;
	while(dwTick - dwEnd < dwDelay);
}

void APP_LCD_Init(void)
{
	Disp_Init();
}

void APP_ADC_Init(void)
{
		/* Enable ADC1 clock */
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

	/* Initialize and configure ADC1 */
	ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);

	/* ADC channel used for IDD measurement */
	ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_192Cycles);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Disable SchmittTrigger for ADC_Channel, to save power */
	//ADC_SchmittTriggerConfig(ADC1, ADC_MOTOR_CHANNEL, DISABLE);

	/* Enable ADC1 Channel used for IDD measurement */
	ADC_ChannelCmd(ADC1, ADC_MOTOR_CHANNEL, ENABLE);

	/* Start ADC1 Conversion using Software trigger*/
      
}

void APP_UART_Init(void)
{
	CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
	
	GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);
	
	/* Remap UART Pins */
	SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, ENABLE);
	
	GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_2, ENABLE);
	GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_3, ENABLE);
	
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	USART_Init(USART1, (uint32_t)38400, USART_WordLength_8b, 
						 USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));						 
						 
	USART_Cmd(USART1, ENABLE);
}

void APP_Timer_Init(void)
{
	/* Setup Timer 1 for 1 ms ticks */
	
	/* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
	
	/* Time Base configuration */
  TIM1_TimeBaseInit(TIM1_PRESCALER, TIM1_CounterMode_Up, TIM1_PERIOD, TIM1_REPTETION_COUNTER);
	
	TIM1_ITConfig(TIM1_IT_Update, ENABLE);

	TIM1_Cmd(ENABLE);
}

void APP_TIM_Int_Handler(void)
{
	dwTick++;	
	if(dwUartTimeout > 0)
	{
		dwUartTimeout--;
	}
}

static uint8_t byPin = 0;

/** @brief  Main program.*/
void main(void)
{	
	static uint8_t byDbg = 0;

	/* Select HSI as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  /* system clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)
  {}
	
	/* Enable external low speed crystal for RTC */
	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
	
	//RTC_RatioCmd(ENABLE);
	//Calendar_Init();
		
	/* Configure WakeUp via RTC CLock */
	RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div4);
	
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	
	/* motor control */
	GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow);
	GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);
	GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
	GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
	
	GPIO_SetBits(GPIOD, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	APP_UART_Init();
	APP_Timer_Init();
	
  /* Initialize Peripherals */	
	APP_LCD_Init();
	APP_ADC_Init();
	UI_Buttons_Init();	
	ModelN_Timer_Init();
	
	/* Enable general interrupts */
  enableInterrupts();
	
	delay_ms(50);
	while(byPin == 0)
	{
		byPin = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7);
	}
		
	/* Configure Halt/Sleep-Mode */
	CLK_HaltConfig(CLK_Halt_SlowWakeup, ENABLE);	
		
	/* Do first conversion of ADC */
	ADC_SoftwareStartConv(ADC1);
	
	byTrigger = 0;
	
	/* Send initial byte to signal Host-Controller to startup */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData8(USART1, 'X');
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	
	/* Protocol Interrupt */
	GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_In_FL_IT);
	EXTI_SetPinSensitivity((EXTI_Pin_TypeDef)GPIO_Pin_7, EXTI_Trigger_Falling);
	
  /* Infinite loop */
  while (1)
  {		
	  /* Trigger fires, when UART Transmission takes place */	
		while(modeln_app_buf_available())
		{
			//Disp_Number(3, byDbg++);
			
			ModelN_Cmd(modeln_app_get_buf());
			ModelN_Send_Protocol();
		}		
		
		ModelN_Menu();
		UI_Handler();
		delay_ms(5);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
