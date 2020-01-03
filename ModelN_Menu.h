#ifndef MODELN_MENU_H
#define MODELN_MENU_H

typedef enum {
	MENU_INIT,
	MENU_CALIBRATE,
	MENU_SHOW_TIME,
	MENU_SHOW_TEMP_CUR,
	MENU_SHOW_TEMP_SET,
	MENU_SHOW_TEMP_OUT,
	MENU_SHOW_HUMID,
	MENU_SET_TEMP
} menu_t;

/**	@brief Menu statemachine */
extern void ModelN_Menu(void);

/**	
	* @brief set new Menu state
	*
	* @param[in]	eNewState	State to enter
	*/
extern void ModelN_Menu_Set_State(menu_t eNewState);

/** 
	* @brief Get current Menu state
	*
	* @return Current Menu state
	*/
extern menu_t ModelN_Menu_Get_State(void);

#endif