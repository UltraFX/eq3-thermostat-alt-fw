#include "main.h"

static menu_t eState = MENU_INIT;

void ModelN_Menu(void)
{	
	static uint8_t bySleep = 1;
	static uint8_t byTimer = 0;
	static uint16_t	wCount = 0;
	static uint8_t byChoice = 0;
	static int8_t iEncValue = 0;
	static uint16_t	iTempSet = 0;
	
	Disp_Clear();
	Disp_Date(sProtocol.byWeekDay);
	Disp_Bargraph_number(sProtocol.bySignal);
				
	switch(eState)
	{
	case MENU_INIT:
	  ModelN_Init();
		eState = INIT_ENTRANCE;
		break;
	case MENU_CALIBRATE:			
		while(ModelN_Calib() != 0);
		eState = MENU_SHOW_TIME;
		break;
	case MENU_SHOW_TIME:
		Display_Time();
		bySleep = 1;
		break;
	case MENU_SHOW_TEMP_CUR:
		Display_Temperature(TEMP_CURRENT);
		bySleep = 1;
		break;
	case MENU_SHOW_TEMP_SET:
		Display_Temperature(TEMP_SET);
		bySleep = 1;
		break;
	case MENU_SHOW_TEMP_OUT:
		Display_Temperature(TEMP_OUTSIDE);
		bySleep = 1;
		break;
	case MENU_SHOW_HUMID:
		Display_Humidity();
		bySleep = 1;
		break;
	case MENU_SET_TEMP:
		if(iTempSet == 0)
		{
			iTempSet = sProtocol.iTempSet;
		}
		wCount++;
		Disp_Decpoint(2, 1);
		Disp_Number(2, iTempSet / 100);
		Disp_Number(3, (iTempSet % 100) / 10);
		Disp_Number(4, (iTempSet % 100) % 10);
		if(byChoice == 0)
		{				
			Disp_Symbol(SYMBOL_DEGREE, 1);
		}
		if(wCount >= MENU_BLINK)
		{			
			wCount = 0;				
			byChoice ^= 1;
		}

		iEncValue += UI_Encoder_Read();
		if(iEncValue <= -4)
		{
			UI_Timeout_Reset();
			if(iTempSet > 150)
			{
				iTempSet -= 5;
			}
			iEncValue = 0;
		}
	
		if(iEncValue >= 4)
		{
			UI_Timeout_Reset();
			if(iTempSet < 350)
			{
				iTempSet += 5;
			}
			iEncValue = 0;
		}
		
		if(UI_Buttons_Get(BUTTON1) == 1)
		{	
			sProtocol.iTempSet = iTempSet;
			iTempSet = 0;
			byRunning = 0;
			ModelN_Menu_Set_State(MENU_SHOW_TEMP_SET);
		}
		break;
	default: 
		eState = MENU_INIT;
		break;
	}
	
	if(bySleep == 1 && byRunning == 0)
	{	
		if(iTempSet != 0)
		{
			iTempSet = 0;
		}
		byTimer++;
		if(byTimer == MENU_CHANGE)
		{
			byTimer = 0;
			if(eState < MENU_SHOW_HUMID)
			{
				eState++;
			}
			else
			{
				eState = MENU_SHOW_TIME;
			}
			byTimer = 0;
		}
		
		RTC_SetWakeUpCounter(2047);
		RTC_WakeUpCmd(ENABLE);
		
		halt();
		
		RTC_WakeUpCmd(DISABLE);
	}
}

void ModelN_Menu_Set_State(menu_t eNewState)
{
	eState = eNewState;
}

menu_t ModelN_Menu_Get_State(void)
{
	return eState;
}