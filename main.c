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

porotocol_t sProtocol;

//static uint8_t byDirection = DIR_OPEN;

RTC_InitTypeDef   RTC_InitStr;
RTC_TimeTypeDef   RTC_TimeStr;
RTC_DateTypeDef   RTC_DateStr;
RTC_AlarmTypeDef  RTC_AlarmStr;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void delay_ms(uint32_t dwDelay)
{
	uint32_t dwEnd = dwTick + dwDelay;
	while(dwTick < dwEnd);
}

void Calendar_Init(void)
{
  RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
  RTC_InitStr.RTC_AsynchPrediv = 0x7F;
  RTC_InitStr.RTC_SynchPrediv = 0x00FF;
  RTC_Init(&RTC_InitStr);

  RTC_DateStructInit(&RTC_DateStr);
  RTC_DateStr.RTC_WeekDay = (RTC_Weekday_TypeDef)sProtocol.byWeekDay;
  RTC_DateStr.RTC_Date = sProtocol.byDay;
  RTC_DateStr.RTC_Month = (RTC_Month_TypeDef)sProtocol.byMonth;
  RTC_DateStr.RTC_Year = sProtocol.byYear;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);

  RTC_TimeStructInit(&RTC_TimeStr);
  RTC_TimeStr.RTC_Hours   = sProtocol.byHour;
  RTC_TimeStr.RTC_Minutes = sProtocol.byMinute;
  RTC_TimeStr.RTC_Seconds = 00;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);
}

void APP_LCD_Init(void)
{
	/* Enable LCD clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
	
	/* Initialize the LCD */
  LCD_Init(LCD_Prescaler_2, LCD_Divider_18, LCD_Duty_1_4,
           LCD_Bias_1_3, LCD_VoltageSource_Internal);

  /* Mask register*/
  LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0x03);
  LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0x00);

  LCD_ContrastConfig(LCD_Contrast_Level_4);
  LCD_DeadTimeConfig(LCD_DeadTime_0);
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

  LCD_Cmd(ENABLE); /*!< Enable LCD peripheral */
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
	
	/* Protocol Interrupt */
	GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_In_FL_IT);
	EXTI_SetPinSensitivity((EXTI_Pin_TypeDef)GPIO_Pin_7, EXTI_Trigger_Falling);
	
	/* Remap UART Pins */
	SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, ENABLE);
	
	GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_2, ENABLE);
	GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_3, ENABLE);
	
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

/** @brief  Main program.*/
void main(void)
{	
	/* Select HSI as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  /* system clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)
  {}
	
	/* Enable external low speed crystal for RTC */
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
	
	//RTC_RatioCmd(ENABLE);
	//Calendar_Init();
		
	/* Configure WakeUp via RTC CLock */
	RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
	
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	
	/* motor control */
	GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow);
	GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);
	GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
	GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
	
	GPIO_SetBits(GPIOD, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
  
  /* Initialize Peripherals */	
	APP_LCD_Init();
	APP_UART_Init();
	APP_Timer_Init();
	APP_ADC_Init();
	UI_Buttons_Init();	
	ModelN_Timer_Init();

	/* Enable general interrupts */
  enableInterrupts();
		
	CLK_HaltConfig(CLK_Halt_SlowWakeup, ENABLE);	
		
	ADC_SoftwareStartConv(ADC1);
	
	byTrigger = 0;
	
  /* Infinite loop */
  while (1)
  {		
	  /* Trigger fires, when UART Transmission takes place */
		if(byTrigger == 1)
		{
			byTrigger = 0;
			
			ModelN_Update_Data();				
			ModelN_Cmd();
			//ModelN_Send_Protocol();
    }
		
		ModelN_Send_Protocol();
		UI_Handler();
		delay_ms(5);
		ModelN_Menu();			
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
