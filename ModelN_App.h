#ifndef MODELN_APP_H
#define MODELN_APP_H

/** @brief Update data from UART */
extern void ModelN_Update_Data(void);

/** @brief Send current protocol Data via UART */
extern void ModelN_Send_Protocol(void);

/** @brief Run a command from Protocol */
extern void ModelN_Cmd(void);

/** At first start, initialize data protocol with data */
extern void ModelN_Init(void);

/** @brief Display current time*/
extern void Display_Time(void);

/**
	* @brief display current temperature
	*
	* @param[in]	byTemp	TEMP_CURRENT: current room temperature
  *											TEMP_GOAL:	set temperature to reach
	*											TEMP_OUTSIDE: Outside temperature
	*/
extern void Display_Temperature(uint8_t byTemp);

/** @brief Display Humidity */
extern void Display_Humidity(void);

#endif