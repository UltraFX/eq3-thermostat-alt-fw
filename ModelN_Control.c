#include "main.h"

static uint16_t		wMaxPos = 0;
static uint16_t 	wCurPos = 0;

volatile uint16_t	wPos = 0;
uint8_t 					byRunning = 0;
uint8_t						byDirection = DIR_OPEN;

uint8_t ModelN_Get_EndPos(void);

void ModelN_Timer_Handler(void)
{
	if(byRunning != 0)
	{
		if(byDirection == DIR_OPEN)
		{
			wPos++;
		}
		else
		{
			if(wPos > 0)
			{
				wPos--;
			}
		}
	}
}

void ModelN_Timer_Init(void)
{	
	/* Enable TIM2 clock */
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
	
	/* Time Base configuration */
	TIM2_TimeBaseInit(TIM2_PRESCALER, TIM2_CounterMode_Up, TIM2_PERIOD);
		
	//TIM1_ITConfig(TIM2_IT_Update, ENABLE);
	
	TIM2_Cmd(ENABLE);
}

uint8_t ModelN_Calib(void)
{
	uint8_t 	byRet = 1;
	uint8_t 	byAvg = 0;
	uint8_t   byCntRounds = 0;	
	uint32_t	dwAvg = 0;
	
	byRunning = 1;
	byDirection = DIR_CLOSE;
	
	Disp_Clear();
	Disp_Character(1, 'C');
	Disp_Character(2, 'A');
	Disp_Character(3, 'L');
	
	ModelN_Motor_Cmd(MOTOR_CMD_FORWARD);
	
	while(byRunning)
	{
		delay_ms(1);	
					
		if(ModelN_Get_EndPos())
		{
			byAvg++;
			if(byAvg < 20)
			{
				continue;
			}
			
			byRunning = 0;
			byAvg = 0;					
			
			if(byDirection == DIR_OPEN)
			{						
				dwAvg += wPos;
				byDirection = DIR_CLOSE;
				byRunning = 1;
				ModelN_Motor_Cmd(MOTOR_CMD_FORWARD);								
		  }
		  else
			{	
				wPos = 0;
				byCntRounds++;
				
				if(byCntRounds == 3)
				{
					ModelN_Motor_Cmd(MOTOR_CMD_STOP);
					byRunning = 0;
					byCntRounds = 0;
					wMaxPos = dwAvg / 2;
					byRet = 0;
				}
				else
				{					
					byDirection = DIR_OPEN;
					byRunning = 1;
					ModelN_Motor_Cmd(MOTOR_CMD_BACKWARD);
				}
			}			
		}
	}
	
	return byRet;
}

void ModelN_Set_Pos(uint8_t byPercent)
{
	uint8_t byBuf[6];
	uint8_t byAvg = 20;
	
	float fNewPos = (float)wMaxPos * ((float)byPercent / 100.0);
	uint16_t wNewPos = (uint16_t)fNewPos;
	
	wCurPos = wPos;
	
	if(wCurPos < wNewPos)
	{		
		byDirection = DIR_OPEN;
		ModelN_Motor_Cmd(MOTOR_CMD_BACKWARD);
		byRunning = 1;
		
		while((wPos < wNewPos));// && !(ModelN_Get_EndPos()));		
	}
	else
	{		
		byDirection = DIR_CLOSE;		
		ModelN_Motor_Cmd(MOTOR_CMD_FORWARD);
		byRunning = 1;
		
		while((wPos > wNewPos)); //&& !(ModelN_Get_EndPos()));
	}
	
	byRunning = 0;
	ModelN_Motor_Cmd(MOTOR_CMD_STOP);
}

void ModelN_Timer_Direction(uint8_t byDirection)
{
	if(byDirection == DIR_OPEN)
	{
		/* Select the Counter Mode */
  TIM2->CR1 &= (uint8_t)((uint8_t)(~TIM_CR1_CMS)) & ((uint8_t)(~TIM_CR1_DIR));
  TIM2->CR1 |= (uint8_t)(TIM2_CounterMode_Up);
	}
	else
	{
		/* Select the Counter Mode */
		TIM2->CR1 &= (uint8_t)((uint8_t)(~TIM_CR1_CMS)) & ((uint8_t)(~TIM_CR1_DIR));
		TIM2->CR1 |= (uint8_t)(TIM2_CounterMode_Up);
	}
}

void ModelN_Timer_Control(uint8_t byControl)
{
	if(byControl == CONTROL_STOP)
	{
		TIM2->CR1 &= (uint8_t)(~TIM_CR1_CEN);
	}
	else
	{
		TIM2->CR1 |= TIM_CR1_CEN;
	}
}

void ModelN_Timer_Reset(void)
{
	TIM2_SetCounter(0);
}

uint16_t ModelN_Timer_Read(void)
{
	return TIM2_GetCounter();
}

void ModelN_Motor_Cmd(uint8_t byCmd)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	switch(byCmd)
	{
		case MOTOR_CMD_STOP: 
			//GPIO_SetBits(GPIOD, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
			break;
		case MOTOR_CMD_BACKWARD: 
			GPIO_ResetBits(GPIOD, GPIO_Pin_4|GPIO_Pin_5);
			break;
		case MOTOR_CMD_FORWARD:
		  GPIO_ResetBits(GPIOD, GPIO_Pin_6|GPIO_Pin_7);		
			break;
		default: //GPIO_SetBits(GPIOD, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		         break;
	}
}

uint8_t ModelN_Get_EndPos(void)
{
	uint8_t byEnd = 0;
	uint16_t 	wADCData = 0;
	
	ADC_SoftwareStartConv(ADC1);
	
	/* Wait until End-Of-Convertion */
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);

	/* Get conversion value */
	wADCData = ADC_GetConversionValue(ADC1);
	
	if(wADCData < 2025)
	{
		byEnd = 1;
	}
	
	return byEnd;
}