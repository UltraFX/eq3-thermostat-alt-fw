#ifndef THREAD_MODE_H
#define THREAD_MODE_H

enum thread_state
{
	THREAD_INIT 				= 0x01,
	THREAD_CONNECTING 	= 0x02,
	THREAD_NO_CON 			= 0x03,
	THREAD_JOINING 			= 0x04,
	THREAD_OK 					= 0x05,
	THREAD_ERROR   			= 0xFF
};

/** @brief Initialize Thread mode */
extern void thread_mode_init(void);

/** @brief Handle thread statemachine until connection is established */
extern void thread_mode_handler(void);

/** 
	* @brief Set new thread state
	*
	* @param[in]	byState		State to switch to	
	*/
extern void thread_mode_set_state(uint8_t byState);

/**
	* @brief Return pointer to initialization-variable to get initialization state of Thread network
	*/
extern uint8_t* thread_mode_get_init(void);

/** @brief Get current state */
extern uint8_t thread_mode_get_state(void);

#endif /* THREAD_MODE_H */