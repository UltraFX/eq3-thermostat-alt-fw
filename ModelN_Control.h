#ifndef MODEL_N_CONTROL_H
#define MODEL_N_CONTROL_H

#define DIR_CLOSE	0
#define DIR_OPEN	1

#define CONTROL_STOP	0
#define CONTROL_START	1

#define MOTOR_CMD_STOP			0
#define MOTOR_CMD_FORWARD		1
#define MOTOR_CMD_BACKWARD	2

#define TIM2_PERIOD                  65535
#define TIM2_PRESCALER               TIM2_Prescaler_128
#define TIM2_REPTETION_COUNTER       0

extern volatile uint16_t	wPos;
extern uint8_t 					  byRunning;
extern uint8_t						byDirection;

extern void ModelN_Timer_Handler(void);
extern void ModelN_Timer_Init(void);
extern void ModelN_Timer_Direction(uint8_t byDirection);
extern void ModelN_Timer_Control(uint8_t byControl);
extern void ModelN_Timer_Reset(void);
extern uint16_t ModelN_Timer_Read(void);

extern uint8_t ModelN_Calib(void);
extern void ModelN_Set_Pos(uint8_t byPercent);

extern void ModelN_Motor_Cmd(uint8_t byCmd);

#endif