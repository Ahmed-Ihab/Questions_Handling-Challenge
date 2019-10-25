/*
 * main.c
 *
 *  Created on: Oct 22, 2019
 *      Author: Aziz & Ahmed Ehab
 */


#include "OS.h"
#include<util/delay.h>
#include "LCD.h"
#include "Keypad.h"
#include "DIO.h"

void Display_Update (void); 
void Question_Handling_Task (void);


uint8 Thumb_UP [7] = { 0x18 , 0x18 , 0x1F , 0x1F , 0x1F , 0x1E , 0x1C};
uint8 Thumb_Down [7] = { 0x07 , 0x0F , 0x1F , 0x1F , 0x1F , 0x03 , 0x03};


#define YES  '1'
#define NO   '2'
#define SKIP '3'

//ST_Task_Info Task_1_Info={LCD_4Bits_Initialization,2,PERIODIC,Ready,1};
ST_Task_Info Task_2_Info={LCD_Update,26,PERIODIC,Ready,0};
//ST_Task_Info Task_3_Info={Display_Update,20,PERIODIC,Ready,0};
ST_Task_Info Task_4_Info={keypad_Update,30,PERIODIC,Ready,1};
ST_Task_Info Task_5_Info={Question_Handling_Task,50,PERIODIC,Ready,1};

uint8 Question[5][16]={"what ur name?","what's ur age?"," ", " ",""};

uint8 Answer_of_Questions[5]={YES,NO,NO,YES,NO};


typedef enum   { QUESTION1=0, QUESTION2 ,QUESTION3,QUESTION4,QUESTION5,RESULT} Question_State_t;
	
void Question_Handling_Task (void)
{
	
	static Question_State_t Question_State = QUESTION1;
	
	static uint8 i = 0;
	static uint8 Correct_Answer=0;
	static uint8 Answer_Display_Flag = 0;
	uint8 Answer= 6;	//any values except 1 , 2 , 3
	uint8 Display_Ans[]="   YES   NO  ";
	
	switch(Question_State)
	{
		case QUESTION1:
		{	
			if(Answer_Display_Flag==0)
			{
				if(Question[QUESTION1][i] != '?')
				{
					LCD_Data_G[0][i]= Question[0][i];
					i++;				
				}	
				
				else // Question has been completely printed so it's time to display the answer
				{
					i=0;
					Answer_Display_Flag=1;
				}			
			}
	
			if(Answer_Display_Flag == 1)
			{
				
				if(Display_Ans[i] != '\0')
				{
					LCD_Data_G[1][i] = Display_Ans[i];
					i++;
				}
				
				else  // Answer Statement has been completely printed so it's time to display choose your answer
				{
					static uint8 Answer=0;
					Keypad_Get_Data_from_Buffer(&Answer);
					
					if(Answer > 0) 
					
					{
						Question_State = QUESTION2;
						Press_Key_Flag = 0;
						i=0;
						Answer_Display_Flag=0;
					}
				}		
			}
				
			break;
				
		}
	
		case QUESTION2:
		{
			if(Answer_Display_Flag==0)				// wait your Question to be completely displayed in the first line then print the choices
			{
				if(Question[QUESTION2][i] != '?')
				{
					LCD_Data_G[0][i]= Question[QUESTION2][i];
					i++;
				}
				
				else // Question has been completely printed so it's time to display the answer choices
				{
					if(Display_Ans[i] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
					{
						LCD_Data_G[1][i] = Display_Ans[i];
						i++;
					}
					
					else								 // Answer Statement has been completely printed so it's time to choose your answer
					{
						i=0;
						Answer_Display_Flag=1;
					}
					
				}
			}
			
		  else if(Answer_Display_Flag == 1)			// it's time to print the answer choices in the second line
			{
					if(	Keypad_Get_Data_from_Buffer(&Answer) == 1)
					{
						if(Answer == Answer_of_Questions[QUESTION2] )
						{
							Correct_Answer++;
							
							LCD_Data_G[0][1]='y';
							
						
							LCD_Send_Byte( 0x01 ,0);
							//LCD_Send_Byte(0,1);		
								
						}
						
						else if (Answer == SKIP)
						{
							LCD_Send_Byte( 0x01 ,0);
							//Do Nothing
						}
						
						else if(Answer != Answer_of_Questions[QUESTION2])
						{
							LCD_Data_G[0][1]='n';
							LCD_Send_Byte( 0x01 ,0);
							LCD_Send_Byte(1,1);
						}
						
						
						
						
						Question_State = QUESTION3;
						Press_Key_Flag = 0;
						i=0;
						Answer_Display_Flag=0;
						
					}
				
			  }
			
			break;
		}
		
		case QUESTION3:
		{
			{
				Question_State = QUESTION4;
			}
			break;
		}
		case QUESTION4:
		{
			{
				Question_State = QUESTION5;
			}
			break;
		}
		case QUESTION5:
		{
			{
				Question_State = RESULT;
			}
			break;
		}
		
		default:
		{
			
		
			break;
		}

		
	}

	
}


/*
void Display_Update (void)
{
	static uint8 i =0;
	static uint8 j =12;
	uint8 Question[]="What's ur Name ?????";
	
	while( Question[i] != '\0')
	{
			LCD_Data_G [0][i]=Question[i];	
			i++;
	}
	
	//LCD_Control_Cursor(0,1,22);
		
	//LCD_Send_Byte(0,1);
	
	
	//LCD_Control_Cursor(1,0,24);
	//LCD_Send_Byte(1,1);
	
	
	Char_G =j;				// Char_G is used to position the Key which get from keypad
	LCD_Keypad_Update();

}
*/


int main()
{
	// Testing Ports
	DDRD = 0xFF;
	DDRB = 0xFF;
	
	// Initialization Section 
	LCD_4Bits_Initialization();
	DIO_init();
	TIMER_init();
	Keypad_Init();
	
	
	LCD_Create_Character(0,Thumb_UP);
	LCD_Create_Character(1,Thumb_Down);
	//LCD_Send_Byte(1,Thumb_UP);

	
	OS_Init(&OS_cnfg);
	

	//OS_Create_Task(&Task_1_Info);
	OS_Create_Task(&Task_2_Info);
	//OS_Create_Task(&Task_3_Info);
	OS_Create_Task(&Task_4_Info);
	OS_Create_Task(&Task_5_Info);
	
	
	OS_Run();
	

	return 0;

}

