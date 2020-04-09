#ifndef MAIN_H
#define MAIN_H

#include "stm8l15x.h"
#include "modeln_display_driver.h"
#include "ModelN_Control.h"
#include "ModelN_App.h"
#include "ModelN_Menu.h"
#include "user_interface.h"
#include "thread_mode.h"

#define	TEMP_CURRENT	0
#define	TEMP_SET			1
#define TEMP_OUTSIDE	2

#define	MENU_CHANGE		15	 /**< Timeout to switch to the next Display (in s) */
#define MENU_BLINK		100	 /**< Blinking interval  (multiple of 5 ms) */
#define BUT_TIMEOUT		600  /**< Timeout when Button was pressed (multiple of 5 ms) */
#define UART_TIMEOUT	2000 /**< Timeout to wait for RX-Data when Interrupt fired (in ms) */

#define INIT_ENTRANCE	MENU_SHOW_TIME //MENU_CALIBRATE //MENU_SHOW_TIME

extern volatile uint32_t dwTick;
extern volatile uint8_t byTrigger;
extern volatile uint32_t dwUartTimeout;
extern volatile uint8_t	byRecAction;

extern void Motor_Command(uint8_t byCmd);
extern void APP_TIM_Int_Handler(void);
extern void delay_ms(uint32_t dwDelay);

#define PROT_NUM_BYTES	21

extern protocol_t sProtocol;

#endif