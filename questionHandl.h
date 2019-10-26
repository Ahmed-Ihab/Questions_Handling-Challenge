/*
 * IncFile1.h
 *
 * Created: 26/10/2019 08:18:52 م
 *  Author: COMPUTER SHOW
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include "OS.h"
#include "LCD.h"
#include "Keypad.h"
#include "DIO.h"
#include "Timer.h"


#define YES  '1'
#define NO   '2'
#define SKIP '3'


typedef enum   { QUESTION1=0, QUESTION2 ,QUESTION3,QUESTION4,QUESTION5,RESULT} Question_State_t;
typedef enum   { PRINT_QUESTION=0, PRINT_CHOICES ,GET_ANSWER,SHOW_CORRECT_OR_NOT} APP_STATE_t;
	
void Question_Handling_Task (void);


#endif /* INCFILE1_H_ */