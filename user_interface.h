#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#define BUTTON0	1
#define BUTTON1	4
#define BUTTON2	8

extern volatile uint8_t byButtonActive;

/** @brief Initialize GPIOs for Buttons and Rotary Encoder */
extern void UI_Buttons_Init(void);

extern uint8_t ui_calc_weekday(uint16_t wYear, uint8_t byMonth, uint8_t byDay);

/** @brief UI Handler to handle UI things */
extern void UI_Handler(void);

/** 
	* @brief Interrupt callback for Buttons
	*
	* @param[in]	byButton	Button which was pressed
	*/
extern void UI_Int_Handler(uint8_t byButton);

/** Reset the timeout, when a button was pressed */
extern void UI_Timeout_Reset(void);

/** 
	* @brief Read out encoder position 
	*
	* @return Encoder value
	*/
extern int8_t UI_Encoder_Read(void);

/**
	* @brief Get specific Button Status
	*
	* @param[in]	byButton	The button to be checked (e.g. BUTTON0)
	* @return			1 = Button pressed, 0 = Button not pressed
	*/
extern uint8_t UI_Buttons_Get(uint8_t byButton);

/** @brief Display "Init" after booting */
extern void UI_Disp_Init(void);

#endif