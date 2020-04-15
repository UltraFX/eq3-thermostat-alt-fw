#include "main.h"

static uint8_t byThread_State = THREAD_INIT;
static uint8_t byThread_Init = 1;

/** @brief Display "Init" while initializing Thread */
static void thread_mode_disp_init(void); 

/** @brief Display "Conn" while connecting to Thread network */
static void thread_mode_disp_conn(void);

/** @brief Display "OK" */
static void thread_mode_disp_ok(void);

/** @brief Display "Err" */
static void thread_mode_disp_err(void);

/** @brief Display "Join" */
static void thread_mode_disp_join(void);

void thread_mode_init(void)
{
	thread_mode_set_state(THREAD_INIT);
	ModelN_Menu_Set_State(MENU_THREAD);
}

void thread_mode_handler(void)
{
	static uint8_t byState_old = 0xFF;
	
	if(byState_old != byThread_State)
	{
		switch(byThread_State)
		{
			case THREAD_INIT:
				byThread_Init = 1;
				thread_mode_disp_init();
				break;
			case THREAD_CONNECTING:
				byThread_Init = 1;
				thread_mode_disp_conn();
				break;
			case THREAD_NO_CON:
				break;
			case THREAD_JOINING:
				thread_mode_disp_join();
				break;
			case THREAD_OK:
				byThread_Init = 0;
				thread_mode_disp_ok();
				break;
			case THREAD_ERROR:
				thread_mode_disp_err();
				break;
			default:
				break;
		}
		
		byState_old = byThread_State;
	}
}

uint8_t thread_mode_get_state(void)
{
	return byThread_State;
}

void thread_mode_set_state(uint8_t byState)
{
	if(byState > THREAD_ERROR)
	{
		return;
	}
	
	byThread_State = byState;
}

uint8_t* thread_mode_get_init(void)
{
	return &byThread_Init;
}


static void thread_mode_disp_init(void)
{
	Disp_Clear();
	Disp_Character(1, 'I');
	Disp_Character(2, 'n');
	Disp_Character(3, 'i');
	Disp_Character(4, 't');
}

static void thread_mode_disp_conn(void)
{
	Disp_Clear();
	Disp_Character(1, 'C');
	Disp_Character(2, 'o');
	Disp_Character(3, 'n');
	Disp_Character(4, 'n');
}

static void thread_mode_disp_ok(void)
{
	Disp_Clear();
	Disp_Character(1, 'O');
	Disp_Character(2, 'k');
}

static void thread_mode_disp_err(void)
{
	Disp_Clear();
	Disp_Character(1, 'E');
	Disp_Character(2, 'r');
	Disp_Character(3, 'r');
}

static void thread_mode_disp_join(void)
{
	Disp_Clear();
	Disp_Character(1, 'J');
	Disp_Character(2, 'o');
	Disp_Character(3, 'i');
	Disp_Character(4, 'n');	
}