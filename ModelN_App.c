#include "main.h"

porotocol_t  sProt_rec;
uint8_t*		pbyProtData;


static void prot_calc_crc(void)
{
  uint16_t wCRC = 0;
  uint8_t *sbyData = (uint8_t*)&sProt_rec.byCmd;
  uint8_t byI;

  for(byI = 0; byI < 20; byI++)
  {
    wCRC += *sbyData;
    sbyData++;
  }
  
  sProtocol.byCRC = (uint8_t)wCRC & 0xFF;
}

static void copy_data(void)
{
	sProtocol.byCmd =  sProt_rec.byCmd;
  sProtocol.biParam1 =  sProt_rec.biParam1;
  sProtocol.biParam2 =  sProt_rec.biParam2;  
  sProtocol.byYear =  sProt_rec.byYear;
  sProtocol.byMonth =  sProt_rec.byMonth;
  sProtocol.byDay =  sProt_rec.byDay;
  sProtocol.byWeekDay =  sProt_rec.byWeekDay;
  sProtocol.byHour =  sProt_rec.byHour;
  sProtocol.byMinute =  sProt_rec.byMinute;
  sProtocol.iTempSet =  sProt_rec.iTempSet;
  sProtocol.iTempCurrent =  sProt_rec.iTempCurrent;
  sProtocol.iTempOutside =  sProt_rec.iTempOutside;
  sProtocol.byHumidity =  sProt_rec.byHumidity;
  sProtocol.bySignal =  sProt_rec.bySignal;
  sProtocol.byBars1 =  sProt_rec.byBars1;
  sProtocol.byBars2 =  sProt_rec.byBars2;
  sProtocol.byBars3 =  sProt_rec.byBars3;
	sProtocol.byCRC = sProt_rec.byCRC;
}

void ModelN_Update_Data(void)
{
	uint8_t byProtCnt = 0;	
	
	pbyProtData = (uint8_t*)&sProt_rec;
	for(byProtCnt = 0; byProtCnt < PROT_NUM_BYTES; byProtCnt++)
	{
		dwUartTimeout = UART_TIMEOUT;
		while((USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) && (dwUartTimeout > 0));
		if(dwUartTimeout == 0)
		{
			return;
		}
		else
		{
			*pbyProtData = USART_ReceiveData8(USART1);		
			pbyProtData++;
		}
	}	
	prot_calc_crc();
	if(sProtocol.byCRC == sProt_rec.byCRC)
	{
		copy_data();
	}
}

void ModelN_Send_Protocol(void)
{
	uint8_t byProtCnt = 0;
	
	pbyProtData = (uint8_t*)&sProt_rec; //sProtocol;
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		
	for(byProtCnt = 0; byProtCnt < PROT_NUM_BYTES; byProtCnt++)
	{
		USART_SendData8(USART1, *pbyProtData);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		pbyProtData++;
	}			
}

void ModelN_Cmd(void)
{
	switch(sProtocol.byCmd)
	{
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
			break;
		default: 	break;
	}
}

void ModelN_Init(void)
{
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
		wTemp = (uint16_t)iTemp;
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
