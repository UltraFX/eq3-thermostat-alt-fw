#include "main.h"
#include "thread_mode.h"

/* Variables for FIFO-Receivebuffer */
protocol_t			  sProt_rec[BUF_SIZE] = {0};
volatile uint8_t  byIn = 0; 
uint8_t           byOut = 0;
uint8_t*				  pbyProtData;

static uint8_t  byMode = MODE_THREAD;
static uint8_t* byCon_Init;
static uint8_t	byInit_Val = 1;
static uint8_t  byData_In = 0;

static RTC_InitTypeDef   RTC_InitStr;
static RTC_TimeTypeDef   RTC_TimeStr;
static RTC_DateTypeDef   RTC_DateStr;

static volatile uint8_t byPosition = 0;


uint8_t modeln_app_buf_available(void)
{
	return ((byIn - byOut) != 0);
}

protocol_t *modeln_app_get_buf(void)
{
	uint8_t byRet = byOut;
	if(byOut < 9)
	{
		byOut++;
	}
	else
	{
		byOut = 0;
	}
	return &sProt_rec[byRet];
}

static void prot_calc_crc(protocol_t *psProt)
{
  const uint8_t byGenerator = 0x1D;
  uint8_t byCRC = 0;
  uint8_t *sbyData = (uint8_t*)&psProt->byCmd;
  uint8_t byI, byK;

  /* Using the algorithm from http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html chapter 4.3 by Bastian Molkenthin */
  for(byI = 0; byI < sizeof(protocol_t)-1; byI++)
  {
      byCRC ^= sbyData[byI]; /* XOR-in the next input byte */

      for (byK = 0; byK < 8; byK++)
      {
          if ((byCRC & 0x80) != 0)
          {
              byCRC = (uint8_t)((byCRC << 1) ^ byGenerator);
          }
          else
          {
              byCRC <<= 1;
          }
      }
  }
  
  sProtocol.byCRC = byCRC;
}

void Calendar_Init(void)
{	
  RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
  RTC_InitStr.RTC_AsynchPrediv = 0x7F;
  RTC_InitStr.RTC_SynchPrediv = 0x00FF;
  RTC_Init(&RTC_InitStr);

  RTC_DateStructInit(&RTC_DateStr);
}

void modeln_app_receive_data(uint8_t byData)
{
	static uint8_t byDbg = 0;
	
	if(byPosition == 0)
	{
		pbyProtData = (uint8_t*)&sProt_rec[byIn];
	}
	
	*pbyProtData = byData;		
	pbyProtData++;
	byPosition++;
	
	if(byPosition == sizeof(protocol_t))
	{
		byRecAction = 0;
		byPosition = 0;
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		prot_calc_crc(&sProt_rec[byIn]);
		if(sProtocol.byCRC == sProt_rec[byIn].byCRC)
		{			
			byTrigger++;
			if(byIn < 9)
			{
				byIn++;
			}
			else
			{
				byIn = 0;
			}
		}	
	}
	//ModelN_Update_Data();
}

static void copy_data(protocol_t *psProt)
{
	uint8_t byBitCode = psProt->cParam1;
	
	sProtocol.byCmd =  				psProt->byCmd;
  sProtocol.cParam1 =  			psProt->cParam1;
  sProtocol.cParam2 =  			psProt->cParam2;  
  
	if(byBitCode & BC_DATE)
	{
		sProtocol.byYear =  			psProt->byYear;
		sProtocol.byMonth = 	 		psProt->byMonth;
		sProtocol.byDay =  				psProt->byDay;
		
		sProtocol.byWeekDay =  	  ui_calc_weekday(sProtocol.byYear, sProtocol.byMonth, sProtocol.byDay);
		
		RTC_DateStr.RTC_WeekDay = (RTC_Weekday_TypeDef)sProtocol.byWeekDay;
		RTC_DateStr.RTC_Date = sProtocol.byDay;
		RTC_DateStr.RTC_Month = (RTC_Month_TypeDef)sProtocol.byMonth;
		RTC_DateStr.RTC_Year = sProtocol.byYear;
		RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);
	}
	
	if(byBitCode & BC_TIME)
	{
		sProtocol.byHour =  			psProt->byHour;
		sProtocol.byMinute =  		psProt->byMinute;
		RTC_TimeStructInit(&RTC_TimeStr);
		RTC_TimeStr.RTC_Hours   = sProtocol.byHour;
		RTC_TimeStr.RTC_Minutes = sProtocol.byMinute;
		RTC_TimeStr.RTC_Seconds = 00;
		RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);
	}
	
	if(byBitCode & BC_CUR_TEMP)
	{
		sProtocol.iTempCurrent =  psProt->iTempCurrent;
	}
	
	if(byBitCode & BC_SET_TEMP)
	{
		sProtocol.iTempSet =  		psProt->iTempSet;
	}
  
	if(byBitCode & BC_OUT_TEMP)
	{
		sProtocol.iTempOutside =  psProt->iTempOutside;
	}
	
	if(byBitCode & BC_HUMID)
	{
		sProtocol.byHumidity =  	psProt->byHumidity;
	}
	
	if(byBitCode & BC_SIGNAL)
	{
		sProtocol.bySignal =  		psProt->bySignal;
	}
	
	if(byBitCode & BC_BARS)
	{
		sProtocol.byBars1 =  			psProt->byBars1;
		sProtocol.byBars2 =  			psProt->byBars2;
		sProtocol.byBars3 =  			psProt->byBars3;
	}
	
	sProtocol.byCRC =					psProt->byCRC;
}

static void copy_cmd(void)
{
	sProtocol.byCmd =  sProt_rec[byIn].byCmd;
  sProtocol.cParam1 =  sProt_rec[byIn].cParam1;
  sProtocol.cParam2 =  sProt_rec[byIn].cParam2;  
	sProtocol.byCRC = sProt_rec[byIn].byCRC;	
}

void ModelN_Send_Protocol(void)
{
	uint8_t byProtCnt = 0;
	uint8_t *pbySend;
	
	prot_calc_crc(&sProtocol);
	pbySend = (uint8_t*)&sProtocol;
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		
	for(byProtCnt = 0; byProtCnt < PROT_NUM_BYTES; byProtCnt++)
	{
		USART_SendData8(USART1, *pbySend);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		pbySend++;
	}			
}

void ModelN_Cmd(protocol_t *psProt)
{
	switch(psProt->byCmd)
	{
		case CMD_MODE:
			switch(psProt->cParam1)
			{
			case MODE_THREAD:
				byMode = MODE_THREAD;
				thread_mode_init();
				ModelN_Menu_Set_State(MENU_THREAD);
				
				sProtocol.byCmd =  CMD_ACK;
				break;
			/* TODO: Implement other modes (WiFi, CAN, BT, ...) */
			default:
				sProtocol.byCmd =  CMD_ERR;
				return;
			}
			break;
		case CMD_DATA:
				copy_data(psProt);
				byData_In = 1;
				sProtocol.byCmd =  CMD_ACK;
				break;
		case CMD_STATE:
				switch(byMode)
				{
					case MODE_THREAD:						
						thread_mode_set_state((uint8_t)psProt->cParam1);
						byCon_Init = thread_mode_get_init();
						if(thread_mode_get_init() != 0)
						{
							ModelN_Menu_Set_State(MENU_THREAD);
						}
						
						sProtocol.byCmd =  CMD_ACK;
						break;
					/* TODO: Implement other modes (WiFi, CAN, BT, ...) */
					default:
					  sProtocol.byCmd =  CMD_ERR;
						break;
				}
				break;
		case CMD_MOTOR:
			sProtocol.byCmd =  CMD_ACK;
			break;
		case CMD_CALIBRATE:
			sProtocol.byCmd =  CMD_ACK;
			while(ModelN_Calib() != 0);
			break;
			/*
		case 'f':
		case 'F': byRunning = 1; byDirection = DIR_CLOSE; ModelN_Motor_Cmd(MOTOR_CMD_FORWARD);
							break;
		case 's':
		case 'S': byRunning = 0; ModelN_Motor_Cmd(MOTOR_CMD_STOP);
							break;
		case 'b':
		case 'B': byRunning = 1; byDirection = DIR_OPEN; ModelN_Motor_Cmd(MOTOR_CMD_BACKWARD);
							break;
		case'c':
		case 'C': 
			while(ModelN_Calib() != 0);
			break;
		case 'P':
			ModelN_Set_Pos((uint8_t)sProtocol.biParam1);
			break;*/
		default: 	
			sProtocol.byCmd =  CMD_ERR;
			break;
	}
}

void ModelN_Init(void)
{
	byCon_Init = &byInit_Val;
	sProtocol.byMonth = 1;
	sProtocol.byDay = 1;
	sProtocol.byWeekDay = 1;
	sProtocol.byHour = 12;
	sProtocol.byMinute = 0;
	sProtocol.iTempSet = 210;
	sProtocol.iTempCurrent = 200;
	sProtocol.iTempOutside = 0;
	sProtocol.byHumidity = 50;
	sProtocol.bySignal = 1;
	
	Calendar_Init();
}

uint8_t ModelN_Data_Received(void)
{
	return byData_In;
}

void Display_Time(void)
{
	Disp_Doublepoint(1);
	Disp_Number(1, sProtocol.byHour / 10);
	Disp_Number(2, sProtocol.byHour % 10);
	
	Disp_Number(3, sProtocol.byMinute / 10);
	Disp_Number(4, sProtocol.byMinute % 10);
}

void Display_Temperature(uint8_t byTemp)
{
	int16_t iTemp;
	uint16_t wTemp;
	uint8_t	byChar = ' ';
	
	switch(byTemp)
	{
		case TEMP_CURRENT:
			iTemp = sProtocol.iTempCurrent;
			Disp_Character(1, 'c');
			break;
		case TEMP_SET:
			iTemp = sProtocol.iTempSet;
			//Disp_Number(1, 5);
			Disp_Character(1, ' ');
			break;				
		case TEMP_OUTSIDE:
			iTemp = sProtocol.iTempOutside;
			Disp_Character(1, 'o');
			break;
	}
	
	if(iTemp < 0)
	{
		Disp_Character(1, '-');
		iTemp = -iTemp;
	}
	wTemp = (uint16_t)iTemp;
	
	Disp_Decpoint(2, 1);
	
	if((wTemp / 100) != 0)
	{
		Disp_Number(2, wTemp / 100);
	}	
	else
	{
		Disp_Character(2, ' ');
	}
	Disp_Number(3, (wTemp % 100) / 10);
	Disp_Number(4, (wTemp % 100) % 10);
	Disp_Symbol(SYMBOL_DEGREE, 1);
}

void Display_Humidity(void)
{
	uint8_t byHum = sProtocol.byHumidity;
	
	if((byHum / 100) != 0)
	{
		Disp_Number(2, byHum / 100);
	}	
	else
	{
		Disp_Character(2, ' ');
	}
	
	if((byHum / 10) != 0)
	{
		Disp_Number(3, byHum / 10);
	}	
	else
	{
		Disp_Character(3, ' ');
	}
	
	Disp_Number(4, (byHum % 100) % 10);
	Disp_Symbol(SYMBOL_PERCENT, 1);
}

uint8_t ModelN_get_init_state(void)
{
	return *byCon_Init;
}

void modeln_app_update_time(void)
{
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
	sProtocol.byHour = RTC_TimeStr.RTC_Hours;
	sProtocol.byMinute = RTC_TimeStr.RTC_Minutes;
}