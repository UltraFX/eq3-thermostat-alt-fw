#ifndef MAIN_H
#define MAIN_H

#include "stm8l15x.h"
#include "modeln_display_driver.h"
#include "ModelN_Control.h"
#include "ModelN_App.h"
#include "ModelN_Menu.h"
#include "user_interface.h"

#define	TEMP_CURRENT	0
#define	TEMP_SET			1
#define TEMP_OUTSIDE	2

#define	MENU_CHANGE		15	 /**< Timeout to switch to the next Display (in s) */
#define MENU_BLINK		100	 /**< Blinking interval  (multiple of 5 ms) */
#define BUT_TIMEOUT		3000 /**< Timeout when Button was pressed (multiple of 5 ms) */
#define UART_TIMEOUT	2000 /**< Timeout to wait for RX-Data when Interrupt fired (in ms) */

#define INIT_ENTRANCE	MENU_SHOW_TIME //MENU_CALIBRATE //MENU_SHOW_TIME

extern volatile uint32_t dwTick;
extern volatile uint8_t byTrigger;
extern volatile uint32_t dwUartTimeout;

extern void Motor_Command(uint8_t byCmd);
extern void APP_TIM_Int_Handler(void);
extern void delay_ms(uint32_t dwDelay);

#define PROT_NUM_BYTES	21

typedef struct
{
	uint8_t	byCmd;
	int8_t	biParam1;
	int8_t	biParam2;
	uint8_t	byYear;
	uint8_t	byMonth;
	uint8_t	byDay;
	uint8_t	byWeekDay;
	uint8_t	byHour;
	uint8_t	byMinute;
	int16_t	iTempSet;
	int16_t	iTempCurrent;
	int16_t	iTempOutside;
	uint8_t	byHumidity;
	uint8_t	bySignal;
	uint8_t	byBars1;
	uint8_t	byBars2;
	uint8_t	byBars3;
	uint8_t byCRC;
} porotocol_t;

extern porotocol_t sProtocol;

#endif