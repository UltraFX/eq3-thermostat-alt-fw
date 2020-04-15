#ifndef MODELN_APP_H
#define MODELN_APP_H

#define BUF_SIZE	10

#define BC_DATE			0x01
#define BC_TIME			0x02
#define BC_CUR_TEMP	0x04
#define BC_SET_TEMP	0x08
#define BC_OUT_TEMP	0x10
#define BC_HUMID		0x20
#define BC_SIGNAL		0x40
#define BC_BARS			0x80

typedef struct
{
	uint8_t	byCmd;
	int8_t	cParam1;
	int8_t	cParam2;
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
} protocol_t;

enum command
{
	CMD_DISPLAY	  = 0x01,
	CMD_DATA      = 0x02,
	CMD_MODE			=	0x10,
	CMD_STATE			= 0x11,
	CMD_MOTOR 		= 0x20,
	CMD_CALIBRATE	= 0x21,
	
	CMD_ACK		    = 0xF0,
	CMD_ERR				= 0xFF
};

enum mode
{
	MODE_THREAD	= 0x01,
	MODE_BT			= 0x02,
	MODE_CAN		= 0x03,
	MODE_WIFI   = 0x04
};

extern void modeln_app_receive_data(uint8_t byData);

/** @brief Return if received data is available */
extern uint8_t modeln_app_buf_available(void);

/** @brief Return current protocol buffer */
extern protocol_t *modeln_app_get_buf(void);

/** @brief Send current protocol Data via UART */
extern void ModelN_Send_Protocol(void);

/** @brief Run a command from Protocol */
extern void ModelN_Cmd(protocol_t *psProt);

/** At first start, initialize data protocol with data */
extern void ModelN_Init(void);

/** Return status of data reception */
extern uint8_t ModelN_Data_Received(void);

/** @brief Display current time*/
extern void Display_Time(void);

/**
	* @brief display current temperature
	*
	* @param[in]	byTemp	TEMP_CURRENT: current room temperature
  *											TEMP_SET:	set temperature to reach
	*											TEMP_OUTSIDE: Outside temperature
	*/
extern void Display_Temperature(uint8_t byTemp);

/** @brief Display Humidity */
extern void Display_Humidity(void);

/** @brief Return initialization state, independent of networking mode */
extern uint8_t ModelN_get_init_state(void);

/** @brief update current time to display from RTC */
extern void modeln_app_update_time(void);

#endif