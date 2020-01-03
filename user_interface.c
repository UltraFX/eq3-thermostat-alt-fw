#include "main.h"

static int8_t iLastVal = 0;
static int8_t iEnc_delta = 0;
static volatile uint8_t byButtons = 0;
volatile uint8_t byButtonActive = 0;
volatile uint16_t	wTimeout = BUT_TIMEOUT;

struct
{
	unsigned Key0:1;
	unsigned Key1:1;
	unsigned Key2:1;
} sKeys;

struct
{
	unsigned Key0:1;
	unsigned Key1:1;
	unsigned Key2:1;
} sState;

static const int16_t iaTable[16] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};

/** @brief read Encoder pins and determine new position */
static void UI_Encoder_Check(void);

/**
	* @brief Debounce Button pushes
	*
	* @return Debounced Button status (8-Bit coded)
	*/
static uint8_t UI_Buttons_Debounce(void);

/**
	* @brief Get debounced 8-bit coded variable and check the chance of 
			     the individual buttons
	*/
static void UI_Buttons_Check(void);

/**
	* @brief Read Button states from GPIOs and store them in a 8-Bit coded variable
	*/
static uint8_t UI_Buttons_Read(void);

void UI_Buttons_Init(void)
{
	/* Buttons 1-3 */
	GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_IT);
	GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_In_PU_IT);
	GPIO_Init(GPIOA, GPIO_Pin_0, GPIO_Mode_In_PU_IT);
	
	EXTI_SetPinSensitivity((EXTI_Pin_TypeDef)GPIO_Pin_0, EXTI_Trigger_Falling);
	EXTI_SetPinSensitivity((EXTI_Pin_TypeDef)GPIO_Pin_2, EXTI_Trigger_Falling);
	EXTI_SetPinSensitivity((EXTI_Pin_TypeDef)GPIO_Pin_4, EXTI_Trigger_Falling);
			
	/* Rotary Encoder */
	GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_In_FL_No_IT);
}

void UI_Int_Handler(uint8_t byButton)
{	
	wTimeout = BUT_TIMEOUT;
	byButtons = UI_Buttons_Debounce();
	byRunning = 1;
}

void UI_Handler(void)
{
	static int8_t iEncValue = 0;
	static uint8_t byNumber = 0, byNumberOld = 0;
	
	wTimeout--;
	if(wTimeout == 0)
	{
		ModelN_Menu_Set_State(MENU_SHOW_TIME);
		byRunning = 0;
	}
	
	UI_Encoder_Check();	
	
	byButtons = UI_Buttons_Debounce();
	UI_Buttons_Check();			
			
	
	if(UI_Buttons_Get(BUTTON0) == 1)
	{
		if(ModelN_Menu_Get_State() < MENU_SHOW_HUMID)
		{
			ModelN_Menu_Set_State(ModelN_Menu_Get_State()+1);
		}
		else
		{
			ModelN_Menu_Set_State(MENU_SHOW_TIME);
		}
	}

	/* Middle Button -> When pressed, go to temperature setting menu */
	if(ModelN_Menu_Get_State() != MENU_SET_TEMP)
	{
		if(UI_Buttons_Get(BUTTON1) == 1)
		{
			ModelN_Menu_Set_State(MENU_SET_TEMP);
		}
	}
	
	if(UI_Buttons_Get(BUTTON2) == 1)
	{
	}
	
	if(byNumber != byNumberOld)
	{
		Disp_Clear();
		Disp_Number(1, byNumber/100);
		Disp_Number(2, (byNumber%100)/10);
		Disp_Number(3, (byNumber%100)%10);
		byNumberOld = byNumber;
	}
}

void UI_Timeout_Reset(void)
{
	wTimeout = BUT_TIMEOUT;
}

/* ENCODER *******************************************************************/
static void UI_Encoder_Check(void)
{
	/* read encoder state every 1 ms (tick interrupt) */
	iLastVal = (iLastVal << 2) & 0x0F;
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0) != 0)
	{
		iLastVal |= 2;
	}

	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) != 0)
	{
		iLastVal |= 1;
	}
	iEnc_delta += iaTable[iLastVal];
}

int8_t UI_Encoder_Read(void)
{
	int8_t iRet;

	iRet = iEnc_delta;
	iEnc_delta = 0;

	return iRet;
}

/* BUTTONS *******************************************************************/
uint8_t UI_Buttons_Get(uint8_t byButton)
{
	uint8_t byRet = 0;

	switch(byButton)
	{
	case BUTTON0:
		if(sKeys.Key0 == 1)
		{
			byRet = 1;
			sKeys.Key0 = 0;
		}
		break;
	case BUTTON1:
		if(sKeys.Key1 == 1)
		{
			byRet = 1;
			sKeys.Key1 = 0;
		}
		break;
	case BUTTON2:
		if(sKeys.Key2 == 1)
		{
			byRet = 1;
			sKeys.Key2 = 0;
		}
		break;
	default:
		break;
	}

	return byRet;
}

static uint8_t UI_Buttons_Debounce(void)
{
	static uint8_t byaBut[3] = {0};
	static uint8_t byCnt = 0;
	static uint8_t byRet;

	if(byCnt == 3)
	{
		byRet = (byaBut[0] & byaBut[1]) |
						(byaBut[1] & byaBut[2]) |
						(byaBut[2] & byaBut[0]);
		byCnt = 0;
	}
	else
	{
		byaBut[byCnt++] = UI_Buttons_Read();
	}

	return byRet;
}

static void UI_Buttons_Check(void)
{
	if(byButtons & 0x04)
	{
		if(sState.Key0 == 0 && sKeys.Key0 == 0)
		{
			sKeys.Key0 = 1;
			sState.Key0 = 1;
		}
	}
	else
	{
		sState.Key0 = 0;
	}

	if(byButtons & 0x10)
		{
			if(sState.Key1 == 0 && sKeys.Key1 == 0)
			{
				sKeys.Key1 = 1;
				sState.Key1 = 1;
			}
		}
		else
		{
			sState.Key1 = 0;
		}

	if(byButtons & 0x01)
		{
			if(sState.Key2 == 0 && sKeys.Key2 == 0)
			{
				sKeys.Key2 = 1;
				sState.Key2 = 1;
			}
		}
		else
		{
			sState.Key2 = 0;
		}
}

static uint8_t UI_Buttons_Read(void)
{
	uint8_t byVal;

	byVal = ((~GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) & GPIO_Pin_2));

	byVal |= ((~GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) & GPIO_Pin_4));

	byVal |= ((~GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) & GPIO_Pin_0));

	return byVal;
}