#ifndef MODELN_DISPLAY_DRIVER
#define MODELN_DISPLAY_DRIVER

#define SYMBOL_DEGREE			1
#define SYMBOL_SUN				2
#define SYMBOL_PERCENT		3
#define SYMBOL_DOOR				4
#define SYMBOL_BATTERY		5
#define SYMBOL_VACATION		6
#define SYMBOL_MOON				7
#define SYMBOL_HOURGRID		8
#define SYMBOL_MANUAL			9
#define SYMBOL_AUTO				10

#define FLASH_ENABLE			1
#define FLASH_DISABLE			0

/** @brief Initialize LCD Display */
extern void Disp_Init(void);

/** @brief Let the display flash
	* 
	* @param[in]	byMode	FLASH_ENABLE or FLASH_DISABLE
	*/
extern void Disp_Flash(uint8_t byMode);

/** @brief Clear the whole Display */
extern void Disp_Clear(void);

/**
	* @brief Display a bar bitcoded
	*
	* @param[in]	dwNumber	bitcode (0x000001 - 0x8FFFFF)
	*/
extern void Disp_Bargraph(uint32_t dwNumber);

/**
	* @brief Display a bar with a certain number
	*
	* @param[in]	byNumber	Number of Bar (1 - 24)
	*/
extern void Disp_Bargraph_number(uint8_t byNumber);

/**
	* @brief Display a number on a digit
	*
	* @param[in]	bySegment	Digit number (1-4)
	* @param[in]	byNumber	Number to display (0 - 9)
	*/
extern void Disp_Number(uint8_t bySegment, uint8_t byNumber);

/**
	* @brief Display a character on a digit
	*
	* @param[in]	bySegment	Digit number (1-4)
	* @param[in]	byNumber	Character to display (C, A, L)
	*/
extern void Disp_Character(uint8_t bySegment, char cChar);

/**
	* @brief Display time grid for the bars at the top
	*
	* @param[in]	byState	Display On (1) or Off (0)
	*/
extern void Disp_Time_Grid(uint8_t byState);

/**
	* @brief Display double point for showing time
	*
	* @param[in]	byState	Display On (1) or Off (0)
	*/
extern void Disp_Doublepoint(uint8_t byState);

/**
	* @brief Display decimal point
	*
	* @param[in]	byDigit	1 or 2
	* @param[in]	byState	Display On (1) or Off (0)
	*/
extern void Disp_Decpoint(uint8_t byDigit, uint8_t byState);

/**
	* @brief Display day on the bottom
	*
	* @param[in]	byDate	All off = 0, or Day (1 - 7)
	*/
extern void Disp_Date(uint8_t byDate);

/**
	* @brief Display special symbol
	*
	* @param[in]	bySymbol	Symbol to display (defined in ModelN_Display_Driver.h)
	* @param[in]	byState		Display On (1) or Off (0)
	*/
extern void Disp_Symbol(uint8_t bySymbol, uint8_t byState);

#endif