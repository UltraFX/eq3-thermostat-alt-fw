#include "main.h"

uint8_t bySegA[4][2] = 
{
	{LCD_RAMRegister_0, 0x80},
	{LCD_RAMRegister_0, 0x20},
	{LCD_RAMRegister_0, 0x08},
	{LCD_RAMRegister_0, 0x02}
};

uint8_t bySegB[4][2] = 
{
	{LCD_RAMRegister_4, 0x08},
	{LCD_RAMRegister_4, 0x02},
	{LCD_RAMRegister_3, 0x80},
	{LCD_RAMRegister_3, 0x20}
};

uint8_t bySegC[4][2] = 
{
	{LCD_RAMRegister_7, 0x80},
	{LCD_RAMRegister_7, 0x20},
	{LCD_RAMRegister_7, 0x08},
	{LCD_RAMRegister_7, 0x02}
};

uint8_t bySegD[4][2] = 
{
	{LCD_RAMRegister_11, 0x08},
	{LCD_RAMRegister_11, 0x02},
	{LCD_RAMRegister_10, 0x80},
	{LCD_RAMRegister_10, 0x20}
};

uint8_t bySegE[4][2] = 
{
	{LCD_RAMRegister_8, 0x02},
	{LCD_RAMRegister_7, 0x40},
	{LCD_RAMRegister_7, 0x10},
	{LCD_RAMRegister_7, 0x04}
};

uint8_t bySegF[4][2] = 
{
	{LCD_RAMRegister_1, 0x02},
	{LCD_RAMRegister_0, 0x40},
	{LCD_RAMRegister_0, 0x10},
	{LCD_RAMRegister_0, 0x04}
};

uint8_t bySegG[4][2] = 
{
	{LCD_RAMRegister_4, 0x20},
	{LCD_RAMRegister_4, 0x04},
	{LCD_RAMRegister_4, 0x01},
	{LCD_RAMRegister_3, 0x40}
};

uint8_t byDaySeg[7][2] = 
{
	{LCD_RAMRegister_4, 0x40},
	{LCD_RAMRegister_1, 0x04},
	{LCD_RAMRegister_11, 0x20},
	{LCD_RAMRegister_11, 0x10},
	{LCD_RAMRegister_8, 0x01},
	{LCD_RAMRegister_4, 0x10},
	{LCD_RAMRegister_1, 0x01}
};

uint8_t bySymbolSeg[][2] =
{
	{LCD_RAMRegister_0, 0x01}, 
	{LCD_RAMRegister_1, 0x08},
	{LCD_RAMRegister_3, 0x10},
	{LCD_RAMRegister_4, 0x80},
	{LCD_RAMRegister_7, 0x01},
	{LCD_RAMRegister_8, 0x04},
	{LCD_RAMRegister_8, 0x08},
	{LCD_RAMRegister_10, 0x10},
	{LCD_RAMRegister_11, 0x40},
	{LCD_RAMRegister_11, 0x80}
};

void Disp_Clear(void)
{
	uint8_t byBuf;
	
	for(byBuf = 0; byBuf < 13; byBuf++)
	{
		LCD->RAM[LCD_RAMRegister_0 + byBuf] = 0;
	}
}

void Disp_Bargraph_number(uint8_t byNumber)
{
	switch(byNumber)
	{	
	case 24: LCD->RAM[LCD_RAMRegister_12] |=  0x20;
					//break;
	case 23: LCD->RAM[LCD_RAMRegister_9] |=  0x02;
					//break;	
  case 22: LCD->RAM[LCD_RAMRegister_5] |=  0x20;
					//break;
	case 21: LCD->RAM[LCD_RAMRegister_2] |=  0x02;
					//break;
  case 20: LCD->RAM[LCD_RAMRegister_2] |=  0x01;
					//break;
  case 19: LCD->RAM[LCD_RAMRegister_5] |=  0x10;
					//break;
  case 18: LCD->RAM[LCD_RAMRegister_9] |=  0x01;
					//break;
	case 17: LCD->RAM[LCD_RAMRegister_12] |=  0x10;
					//break;
	case 16: LCD->RAM[LCD_RAMRegister_12] |=  0x08;
					//break;	
  case 15: LCD->RAM[LCD_RAMRegister_8] |=  0x80;
					//break;
	case 14: LCD->RAM[LCD_RAMRegister_5] |=  0x08;
					//break;
	case 13: LCD->RAM[LCD_RAMRegister_1] |=  0x80;
				//break;	
  case 12: LCD->RAM[LCD_RAMRegister_1] |=  0x40;
					//break;	
	case 11: LCD->RAM[LCD_RAMRegister_5] |=  0x04;
					//break;
	case 10: LCD->RAM[LCD_RAMRegister_8] |=  0x40;
					//break;
	case 9: LCD->RAM[LCD_RAMRegister_12] |=  0x04;
					//break;
	case 8: LCD->RAM[LCD_RAMRegister_12] |=  0x02;
					//break;
	case 7: LCD->RAM[LCD_RAMRegister_8] |=  0x20;
					//break;
	case 6: LCD->RAM[LCD_RAMRegister_5] |=  0x02;
					//break;
	case 5: LCD->RAM[LCD_RAMRegister_1] |=  0x20;
					//break;
	case 4: LCD->RAM[LCD_RAMRegister_1] |=  0x10;
					//break;
	case 3: LCD->RAM[LCD_RAMRegister_5] |=  0x01;
					//break;
	case 2: LCD->RAM[LCD_RAMRegister_8] |=  0x10;
					//break;
	case 1: LCD->RAM[LCD_RAMRegister_8] |=  0x10;
					break;				
	default: break;
	}
}

void Disp_Init(void)
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

	Disp_Flash(FLASH_DISABLE);
  LCD_Cmd(ENABLE); /*!< Enable LCD peripheral */
}

void Disp_Flash(uint8_t byMode)
{
	if(byMode == 0)
	{
		LCD_BlinkConfig(LCD_BlinkMode_Off, LCD_BlinkFrequency_Div512); //LCD_BlinkMode_TypeDef LCD_BlinkMode, LCD_BlinkFrequency_TypeDef LCD_BlinkFrequency
	}
	else
	{
		LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM, LCD_BlinkFrequency_Div512);
	}
}

void Disp_Bargraph(uint32_t dwNumber)
{
	LCD->RAM[LCD_RAMRegister_12] = (dwNumber | 0x000001)?LCD->RAM[LCD_RAMRegister_12] | 0x01:LCD->RAM[LCD_RAMRegister_12] & ~0x01; 
	LCD->RAM[LCD_RAMRegister_8] =  (dwNumber | 0x000002)?LCD->RAM[LCD_RAMRegister_12] | 0x10:LCD->RAM[LCD_RAMRegister_12] & ~0x10; 
	LCD->RAM[LCD_RAMRegister_5] =  (dwNumber | 0x000004)?LCD->RAM[LCD_RAMRegister_12] | 0x01:LCD->RAM[LCD_RAMRegister_12] & ~0x01; 
	LCD->RAM[LCD_RAMRegister_1] =  (dwNumber | 0x000008)?LCD->RAM[LCD_RAMRegister_12] | 0x10:LCD->RAM[LCD_RAMRegister_12] & ~0x10; 
	LCD->RAM[LCD_RAMRegister_1] =  (dwNumber | 0x000010)?LCD->RAM[LCD_RAMRegister_12] | 0x20:LCD->RAM[LCD_RAMRegister_12] & ~0x20; 
	LCD->RAM[LCD_RAMRegister_5] =  (dwNumber | 0x000020)?LCD->RAM[LCD_RAMRegister_12] | 0x02:LCD->RAM[LCD_RAMRegister_12] & ~0x02; 
	LCD->RAM[LCD_RAMRegister_8] =  (dwNumber | 0x000040)?LCD->RAM[LCD_RAMRegister_12] | 0x20:LCD->RAM[LCD_RAMRegister_12] & ~0x20; 
	LCD->RAM[LCD_RAMRegister_12] = (dwNumber | 0x000080)?LCD->RAM[LCD_RAMRegister_12] | 0x02:LCD->RAM[LCD_RAMRegister_12] & ~0x02; 
	LCD->RAM[LCD_RAMRegister_12] = (dwNumber | 0x000100)?LCD->RAM[LCD_RAMRegister_12] | 0x04:LCD->RAM[LCD_RAMRegister_12] & ~0x04; 
	LCD->RAM[LCD_RAMRegister_8] =  (dwNumber | 0x000200)?LCD->RAM[LCD_RAMRegister_12] | 0x40:LCD->RAM[LCD_RAMRegister_12] & ~0x40; 
	LCD->RAM[LCD_RAMRegister_5] =  (dwNumber | 0x000400)?LCD->RAM[LCD_RAMRegister_12] | 0x04:LCD->RAM[LCD_RAMRegister_12] & ~0x04; 
	LCD->RAM[LCD_RAMRegister_1] =  (dwNumber | 0x000800)?LCD->RAM[LCD_RAMRegister_12] | 0x40:LCD->RAM[LCD_RAMRegister_12] & ~0x40; 
	LCD->RAM[LCD_RAMRegister_1] =  (dwNumber | 0x001000)?LCD->RAM[LCD_RAMRegister_12] | 0x80:LCD->RAM[LCD_RAMRegister_12] & ~0x80; 
	LCD->RAM[LCD_RAMRegister_5] =  (dwNumber | 0x002000)?LCD->RAM[LCD_RAMRegister_12] | 0x08:LCD->RAM[LCD_RAMRegister_12] & ~0x08; 
	LCD->RAM[LCD_RAMRegister_8] =  (dwNumber | 0x004000)?LCD->RAM[LCD_RAMRegister_12] | 0x20:LCD->RAM[LCD_RAMRegister_12] & ~0x20; 
	LCD->RAM[LCD_RAMRegister_12] = (dwNumber | 0x008000)?LCD->RAM[LCD_RAMRegister_12] | 0x08:LCD->RAM[LCD_RAMRegister_12] & ~0x08; 
	LCD->RAM[LCD_RAMRegister_12] = (dwNumber | 0x010000)?LCD->RAM[LCD_RAMRegister_12] | 0x10:LCD->RAM[LCD_RAMRegister_12] & ~0x10; 
	LCD->RAM[LCD_RAMRegister_9] =  (dwNumber | 0x020000)?LCD->RAM[LCD_RAMRegister_12] | 0x01:LCD->RAM[LCD_RAMRegister_12] & ~0x01; 
	LCD->RAM[LCD_RAMRegister_5] =  (dwNumber | 0x040000)?LCD->RAM[LCD_RAMRegister_12] | 0x10:LCD->RAM[LCD_RAMRegister_12] & ~0x10; 
	LCD->RAM[LCD_RAMRegister_2] =  (dwNumber | 0x080000)?LCD->RAM[LCD_RAMRegister_12] | 0x01:LCD->RAM[LCD_RAMRegister_12] & ~0x01; 
	LCD->RAM[LCD_RAMRegister_2] =  (dwNumber | 0x100000)?LCD->RAM[LCD_RAMRegister_12] | 0x02:LCD->RAM[LCD_RAMRegister_12] & ~0x02; 
	LCD->RAM[LCD_RAMRegister_5] =  (dwNumber | 0x200000)?LCD->RAM[LCD_RAMRegister_12] | 0x20:LCD->RAM[LCD_RAMRegister_12] & ~0x20; 
	LCD->RAM[LCD_RAMRegister_9] =  (dwNumber | 0x400000)?LCD->RAM[LCD_RAMRegister_12] | 0x02:LCD->RAM[LCD_RAMRegister_12] & ~0x02; 
	LCD->RAM[LCD_RAMRegister_12] = (dwNumber | 0x800000)?LCD->RAM[LCD_RAMRegister_12] | 0x20:LCD->RAM[LCD_RAMRegister_12] & ~0x20; 
}

void Disp_Number(uint8_t bySegment, uint8_t byNumber)
{
	uint8_t bySeg = bySegment - 1;
	
	if((bySegment < 1) || (bySegment > 4) || (byNumber > 9))
	{
		return;
	}
	
	switch(byNumber)
	{
	case 0:	LCD->RAM[(bySegA[bySeg][0])] |= bySegA[bySeg][1];
					LCD->RAM[(bySegB[bySeg][0])] |= bySegB[bySeg][1];
					LCD->RAM[(bySegC[bySeg][0])] |= bySegC[bySeg][1];
					LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
					LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
					LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
					LCD->RAM[bySegG[bySeg][0]] &= ~bySegG[bySeg][1];
					break;
	case 1:	LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
					LCD->RAM[bySegB[bySeg][0]] |= bySegB[bySeg][1];
					LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
					LCD->RAM[bySegD[bySeg][0]] &= ~bySegD[bySeg][1];
					LCD->RAM[bySegE[bySeg][0]] &= ~bySegE[bySeg][1];
					LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
					LCD->RAM[bySegG[bySeg][0]] &= ~bySegG[bySeg][1];
					break;
	case 2:	LCD->RAM[bySegA[bySeg][0]] |= bySegA[bySeg][1];
					LCD->RAM[bySegB[bySeg][0]] |= bySegB[bySeg][1];
					LCD->RAM[bySegC[bySeg][0]] &= ~bySegC[bySeg][1];
					LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
					LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
					LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
					LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
					break;
	case 3:	LCD->RAM[bySegA[bySeg][0]] |= bySegA[bySeg][1];
					LCD->RAM[bySegB[bySeg][0]] |= bySegB[bySeg][1];
					LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
					LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
					LCD->RAM[bySegE[bySeg][0]] &= ~bySegE[bySeg][1];
					LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
					LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
					break;
	case 4:	LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
					LCD->RAM[bySegB[bySeg][0]] |= bySegB[bySeg][1];
					LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
					LCD->RAM[bySegD[bySeg][0]] &= ~bySegD[bySeg][1];
					LCD->RAM[bySegE[bySeg][0]] &= ~bySegE[bySeg][1];
					LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
					LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
					break;
	case 5:	LCD->RAM[bySegA[bySeg][0]] |= bySegA[bySeg][1];
					LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
					LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
					LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
					LCD->RAM[bySegE[bySeg][0]] &= ~bySegE[bySeg][1];
					LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
					LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
					break;
	case 6:	LCD->RAM[bySegA[bySeg][0]] |= bySegA[bySeg][1];
					LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
					LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
					LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
					LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
					LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
					LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
					break;
	case 7:	LCD->RAM[bySegA[bySeg][0]] |= bySegA[bySeg][1];
					LCD->RAM[bySegB[bySeg][0]] |= bySegB[bySeg][1];
					LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
					LCD->RAM[bySegD[bySeg][0]] &= ~bySegD[bySeg][1];
					LCD->RAM[bySegE[bySeg][0]] &= ~bySegE[bySeg][1];
					LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
					LCD->RAM[bySegG[bySeg][0]] &= ~bySegG[bySeg][1];
					break;
	case 8:	LCD->RAM[bySegA[bySeg][0]] |= bySegA[bySeg][1];
					LCD->RAM[bySegB[bySeg][0]] |= bySegB[bySeg][1];
					LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
					LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
					LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
					LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
					LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
					break;
	case 9:	LCD->RAM[bySegA[bySeg][0]] |= bySegA[bySeg][1];
					LCD->RAM[bySegB[bySeg][0]] |= bySegB[bySeg][1];
					LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
					LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
					LCD->RAM[bySegE[bySeg][0]] &= ~bySegE[bySeg][1];
					LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
					LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
					break;				
	}
}

void Disp_Character(uint8_t bySegment, char cChar)
{
	uint8_t bySeg = bySegment - 1;
	
	if((bySegment < 1) || (bySegment > 4))
	{
		return;
	}
	
	switch(cChar)
	{
	case 'A':	
	case 'a':	LCD->RAM[bySegA[bySeg][0]] |= bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] |= bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] &= ~bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
						break;
	case 'C':
	case 'c': LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] &= ~bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
						break;
	case 'E':	
	case 'e':	LCD->RAM[bySegA[bySeg][0]] |= bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] &= ~bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
						break;							
	case 'I':	
	case 'i':	LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] &= ~bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] &= ~bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] &= ~bySegG[bySeg][1];
						break;	
  case 'J':	
	case 'j':	LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] |= bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] &= ~bySegG[bySeg][1];
						break;						
	case 'K':	
	case 'k':	LCD->RAM[bySegA[bySeg][0]] |= bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] &= ~bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
						break;						
	case 'L':	
	case 'l': LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] &= ~bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] &= ~bySegG[bySeg][1];
						break;
	case 'N':
	case 'n': LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] &= ~bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
						break;
	case 'O':
	case 'o': LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] |= bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
						break;
	case 'R':
	case 'r': LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] &= ~bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] &= ~bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
						break;
	case 'T':
	case 't': LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] &= ~bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] |= bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] |= bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] |= bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
						break;
  case '-': LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] &= ~bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] &= ~bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] &= ~bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] |= bySegG[bySeg][1];
						break;
	case ' ': LCD->RAM[bySegA[bySeg][0]] &= ~bySegA[bySeg][1];
						LCD->RAM[bySegB[bySeg][0]] &= ~bySegB[bySeg][1];
						LCD->RAM[bySegC[bySeg][0]] &= ~bySegC[bySeg][1];
						LCD->RAM[bySegD[bySeg][0]] &= ~bySegD[bySeg][1];
						LCD->RAM[bySegE[bySeg][0]] &= ~bySegE[bySeg][1];
						LCD->RAM[bySegF[bySeg][0]] &= ~bySegF[bySeg][1];
						LCD->RAM[bySegG[bySeg][0]] &= ~bySegG[bySeg][1];
						break;						
	default:
		break;
	}	
}

void Disp_Time_Grid(uint8_t byState)
{
	if(byState != 0)
	{
		LCD->RAM[LCD_RAMRegister_10] |= 0x10;
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_10] &= ~0x10;
	}
}

void Disp_Doublepoint(uint8_t byState)
{
	if(byState != 0)
	{
		LCD->RAM[LCD_RAMRegister_11] |= 0x04;
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_11] &= ~0x04;
	}
}

void Disp_Decpoint(uint8_t byDigit, uint8_t byState)
{
	if((byDigit != 1) && (byDigit != 2))
	{
		return;
	}
	
	if(byState != 0)
	{
		if(byDigit == 1)
		{
			LCD->RAM[LCD_RAMRegister_11] |= 0x01;
		}
		else
		{
			LCD->RAM[LCD_RAMRegister_10] |= 0x40;
		}		
	}
	else
	{
		if(byDigit == 1)
		{
			LCD->RAM[LCD_RAMRegister_11] &= ~0x01;
		}
		else
		{
			LCD->RAM[LCD_RAMRegister_10] &= ~0x40;
		}	
	}
}

void Disp_Date(uint8_t byDate)
{
	uint8_t byCnt;
	uint8_t byNum = byDate - 1;
	
	if(byDate > 7)
	{
		return;
	}
	
	for(byCnt = 0; byCnt < 7; byCnt++)
	{
		LCD->RAM[byDaySeg[byCnt][0]] &= ~byDaySeg[byCnt][1];
	}
	LCD->RAM[byDaySeg[byNum][0]] |= byDaySeg[byNum][1];
}

void Disp_Symbol(uint8_t bySymbol, uint8_t byState)
{
	uint8_t bySym = bySymbol - 1;
	uint8_t byCnt;
	
	if(bySymbol > 10)
	{
		return;
	}
	
	if(bySymbol == 0)
	{
		for(byCnt = 0; byCnt < 10; byCnt++)
		{
			LCD->RAM[bySymbolSeg[byCnt][0]] &= ~bySymbolSeg[byCnt][1];
		}
	}
	else
	{
		if(byState != 0)
		{
			LCD->RAM[bySymbolSeg[bySym][0]] |= bySymbolSeg[bySym][1];
		}
		else
		{
			LCD->RAM[bySymbolSeg[bySym][0]] &= ~bySymbolSeg[bySym][1];
		}		
	}
}