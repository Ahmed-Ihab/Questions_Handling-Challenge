/*
 * main.c
 *
 *  Created on: Oct 22, 2019
 *      Author: Aziz & Ahmed Ehab
 */


#include "OS.h"
#include "LCD.h"
#include "Keypad.h"
#include "DIO.h"
#include "Timer.h"



#define YES  '1'
#define NO   '2'
#define SKIP '3'


void Question_Handling_Task (void);
void Task_Init (void);

uint8 Question[5][16]={"ascii of . = 46?","ascii of + = 43?","ascii of E = 70?", "ascii of N = 78?","ascii of B = 67?"};
uint8 Answer_of_Questions[5]={YES,YES,NO,YES,NO};

typedef enum   { QUESTION1=0, QUESTION2 ,QUESTION3,QUESTION4,QUESTION5,RESULT} Question_State_t;
typedef enum   { PRINT_QUESTION=0, PRINT_CHOICES ,GET_ANSWER,SHOW_CORRECT_OR_NOT} APP_STATE_t;
	
uint8 Thumb_UP [7] = { 0x18 , 0x18 , 0x1F , 0x1F , 0x1F , 0x1E , 0x1C};
uint8 Thumb_Down [7] = { 0x07 , 0x0F , 0x1F , 0x1F , 0x1F , 0x03 , 0x03};


ST_Task_Info Task_1_Info={Task_Init,1,PERIODIC,Ready,3};
ST_Task_Info Task_2_Info={LCD_Update,20,PERIODIC,Ready,0};
ST_Task_Info Task_3_Info={keypad_Update,50,PERIODIC,Ready,1};
ST_Task_Info Task_4_Info={Question_Handling_Task,100,PERIODIC,Ready,2};


void Task_Init(void)
{
	static uint8 Init_Done_flag=0;
	static uint8 LCD_init_Done_Flag = 0;
	Function_Process_t Function_Process;

	if(Init_Done_flag == 0)
	{
		DIO_init();
		TIMER_init();
		Keypad_Init();
		Init_Done_flag = 1;
		
		return;
	}
	
	
	else if(Init_Done_flag == 1)
	{
		if(LCD_init_Done_Flag == 0)
		{
			Function_Process = LCD_4Bits_Initialization();
			
			if(Function_Process == FINISHED)
			{
				
				LCD_init_Done_Flag = 1;
				return;
			}
			else
			{
				//Do_NOTHING
			}
			
		}
		
		else
		{
			OS_Delete_Task(&Task_1_Info);
			//Do_NOTHING
		}
		
	}
	
			
}



void Question_Handling_Task (void)
{
	static Question_State_t Question_State = QUESTION1;
	static APP_STATE_t app_state = PRINT_QUESTION;
	static uint8 quest_iterator = 0;
	static uint8 Correct_Answer=0;
	static uint8 correct_answer_flag = 0;
	static uint8 wrong_answer_flag = 0;
	uint8 Answer= 6;	//any values except 1 , 2 , 3
	uint8 Display_choices[]="1.YES 2.NO 3.SKP";
	uint8 score[]="yr score:";
	uint8 right[]="CORRECT ";
	uint8 wrong[]="WRONG ";
	uint8 static fini = 0;

	switch(Question_State)

	{
	case QUESTION1:
	{
		switch(app_state)
		{
			
			case PRINT_QUESTION:
			{
				if(Question[QUESTION1][quest_iterator] != '?')
				{
					LCD_Data_G[0][quest_iterator]= Question[QUESTION1][quest_iterator];
					quest_iterator++;
				}
				else
				{
					app_state = PRINT_CHOICES;
					quest_iterator = 0;
				}

				break;
			}
			
			
			case PRINT_CHOICES:
			{
				if(Display_choices[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
				{
					LCD_Data_G[1][quest_iterator] = Display_choices[quest_iterator];
					quest_iterator++;
				}

				else								 // Answer Statement has been completely printed so it's time to choose your answer
				{
					quest_iterator=0;
					app_state = GET_ANSWER;
				}

				break;
			}


			case GET_ANSWER:
			{
				if(	Keypad_Get_Data_from_Buffer(&Answer) )
				{
					if(Answer == Answer_of_Questions[QUESTION1] )
					{
						Correct_Answer++;
						correct_answer_flag = 1;
						wrong_answer_flag = 0;
						LCD_Send_Byte( 0x01 ,0);
						app_state = SHOW_CORRECT_OR_NOT;
						
					}

					else if(Answer != Answer_of_Questions[QUESTION1] && Answer != SKIP)
					{
						//clear screen
						LCD_Send_Byte( 0x01 ,0);
						wrong_answer_flag = 1;
						correct_answer_flag = 0;
						app_state = SHOW_CORRECT_OR_NOT;
					}
					else if (Answer == SKIP)
					{
						//clear screen
						LCD_Send_Byte( 0x01 ,0);
						app_state = SHOW_CORRECT_OR_NOT;
						//Do Nothing
					}
					else
					{
						//error not availble
					}
					
					app_state = SHOW_CORRECT_OR_NOT;
					quest_iterator=0;
				}
				break;
			}
			case SHOW_CORRECT_OR_NOT:
			{
				if(correct_answer_flag == 1 )
				{
					//Correct_Answer++;
					
						if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
						{
						LCD_Data_G[0][quest_iterator] = right[quest_iterator];
						quest_iterator++;
						}
						else
						{
						quest_iterator = 0;
						correct_answer_flag = 0;
						//LCD_Send_Byte( 0x01 ,0);
						app_state = PRINT_QUESTION;
						Question_State = QUESTION2;
						}
					
					
				}
				else if (wrong_answer_flag == 1 )
				{
					if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
					{
						LCD_Data_G[0][quest_iterator] = wrong[quest_iterator];
						quest_iterator++;
					}
					else
					{
						quest_iterator = 0;
						wrong_answer_flag = 0;
						//LCD_Send_Byte( 0x01 ,0);
						app_state = PRINT_QUESTION;
						Question_State = QUESTION2;
					}	
				}
				break;
			}
		}
		break;
	}
	case QUESTION2:
		{
			switch(app_state)
			{
				
				case PRINT_QUESTION:
				{
					if(Question[QUESTION2][quest_iterator] != '?')
					{
						//LCD_4Bits_Print_Character(1,quest_iterator,Question[QUESTION1][quest_iterator]);
						LCD_Data_G[0][quest_iterator]= Question[QUESTION2][quest_iterator];
						quest_iterator++;
					}
					else
					{
						app_state = PRINT_CHOICES;
						quest_iterator = 0;
					}

					break;
				}
				
				
				case PRINT_CHOICES:
				{
					if(Display_choices[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
					{
						LCD_Data_G[1][quest_iterator] = Display_choices[quest_iterator];
						quest_iterator++;
					}

					else								 // Answer Statement has been completely printed so it's time to choose your answer
					{
						quest_iterator=0;
						app_state = GET_ANSWER;
					}

					break;
				}


				case GET_ANSWER:
				{
					if(	Keypad_Get_Data_from_Buffer(&Answer) )
					{
						if(Answer == Answer_of_Questions[QUESTION2] )
						{
							Correct_Answer++;
							correct_answer_flag = 1;
							LCD_Send_Byte( 0x01 ,0);
							app_state = SHOW_CORRECT_OR_NOT;
							
						}

						else if(Answer != Answer_of_Questions[QUESTION2] && Answer != SKIP)
						{
							//clear screen
							LCD_Send_Byte( 0x01 ,0);

							wrong_answer_flag = 1;
							app_state = SHOW_CORRECT_OR_NOT;

						}
						else if (Answer == SKIP)
						{
							//clear screen
							LCD_Send_Byte( 0x01 ,0);
							app_state = SHOW_CORRECT_OR_NOT;
							//Do Nothing
						}
						else
						{
							//error not availble
						}
						
						
						quest_iterator=0;
					}
					break;
				}
				case SHOW_CORRECT_OR_NOT:
				{
					if(correct_answer_flag == 1 )
					{
						
						//LCD_Send_Byte( 0x01 ,0);
						if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
						{
							LCD_Data_G[0][quest_iterator] = right[quest_iterator];
							quest_iterator++;
							}
							else
							{
							quest_iterator = 0;
							//LCD_Send_Byte( 0x01 ,0);
							correct_answer_flag = 0;
							app_state = PRINT_QUESTION;
							Question_State = QUESTION3;
						}
						
						
					}
					else if (wrong_answer_flag == 1 )
					{
						if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
						{
							LCD_Data_G[0][quest_iterator] = wrong[quest_iterator];
							quest_iterator++;
						}
						else
						{
							quest_iterator = 0;
							wrong_answer_flag = 0;
							app_state = PRINT_QUESTION;
							Question_State = QUESTION3;
						}
					}
					break;
				}
				
			}
			break;
		}
		case QUESTION3:
		{
			switch(app_state)
			{
				
				case PRINT_QUESTION:
				{
					if(Question[QUESTION3][quest_iterator] != '?')
					{
						LCD_Data_G[0][quest_iterator]= Question[QUESTION3][quest_iterator];
						quest_iterator++;
					}
					else
					{
						app_state = PRINT_CHOICES;
						quest_iterator = 0;
					}

					break;
				}
				
				
				case PRINT_CHOICES:
				{
					if(Display_choices[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
					{
						LCD_Data_G[1][quest_iterator] = Display_choices[quest_iterator];
						quest_iterator++;
					}

					else								 // Answer Statement has been completely printed so it's time to choose your answer
					{
						quest_iterator=0;
						app_state = GET_ANSWER;
					}

					break;
				}


				case GET_ANSWER:
				{
					if(	Keypad_Get_Data_from_Buffer(&Answer) )
					{
						if(Answer == Answer_of_Questions[QUESTION3] )
						{
							Correct_Answer++;
							correct_answer_flag = 1;
							wrong_answer_flag = 0;
							LCD_Send_Byte( 0x01 ,0);
							app_state = SHOW_CORRECT_OR_NOT;
							
						}

						else if(Answer != Answer_of_Questions[QUESTION3] && Answer != SKIP)
						{
							//clear screen
							LCD_Send_Byte( 0x01 ,0);

							wrong_answer_flag = 1;
							correct_answer_flag = 0;
							app_state = SHOW_CORRECT_OR_NOT;

						}
						else if (Answer == SKIP)
						{
							//clear screen
							LCD_Send_Byte( 0x01 ,0);
							app_state = SHOW_CORRECT_OR_NOT;
							//Do Nothing
						}
						else
						{
							//error not availble
						}
						quest_iterator=0;
					}
					break;
				}
				case SHOW_CORRECT_OR_NOT:
				{
					if(correct_answer_flag == 1 )
					{
						
						//LCD_Send_Byte( 0x01 ,0);
						if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
						{
							LCD_Data_G[0][quest_iterator] = right[quest_iterator];
							quest_iterator++;
							}
							else
							{
							quest_iterator = 0;
							correct_answer_flag = 0;
							app_state = PRINT_QUESTION;
							Question_State = QUESTION4;
						    }
						
						
					   }
					else if (wrong_answer_flag == 1 )
					{
						if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
						{
							LCD_Data_G[0][quest_iterator] = wrong[quest_iterator];
							quest_iterator++;
						}
						else
						{
							quest_iterator = 0;
							wrong_answer_flag = 0;
							app_state = PRINT_QUESTION;
							Question_State = QUESTION4;
						}	
					}
					break;
				}
			}
			break;
		}
	
	
		case QUESTION4:
		{
			switch(app_state)
			{
				
				case PRINT_QUESTION:
				{
					if(Question[QUESTION4][quest_iterator] != '?')
					{
						
						LCD_Data_G[0][quest_iterator]= Question[QUESTION4][quest_iterator];
						quest_iterator++;
					}
					else
					{
						app_state = PRINT_CHOICES;
						quest_iterator = 0;
					}

					break;
				}
				
				
				case PRINT_CHOICES:
				{
					if(Display_choices[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
					{
						//LCD_4Bits_Print_Character(2,quest_iterator,Display_choices[quest_iterator]);
						LCD_Data_G[1][quest_iterator] = Display_choices[quest_iterator];
						quest_iterator++;
					}

					else								 // Answer Statement has been completely printed so it's time to choose your answer
					{
						quest_iterator=0;
						app_state = GET_ANSWER;
					}

					break;
				}


				case GET_ANSWER:
				{
					if(	Keypad_Get_Data_from_Buffer(&Answer) )
					{
						if(Answer == Answer_of_Questions[QUESTION4] )
						{
							Correct_Answer++;
							correct_answer_flag = 1;
							LCD_Send_Byte( 0x01 ,0);
							app_state = SHOW_CORRECT_OR_NOT;
							
						}

						else if(Answer != Answer_of_Questions[QUESTION4] && Answer != SKIP)
						{
							//clear screen
							LCD_Send_Byte( 0x01 ,0);

							wrong_answer_flag = 1;
							app_state = SHOW_CORRECT_OR_NOT;

						}
						else if (Answer == SKIP)
						{
							//clear screen
							LCD_Send_Byte( 0x01 ,0);
							app_state = SHOW_CORRECT_OR_NOT;
							//Do Nothing
						}
						else
						{
							//error not availble
						}
						
						
						quest_iterator=0;
					}
					break;
				}
				case SHOW_CORRECT_OR_NOT:
				{
					if(correct_answer_flag == 1 )
					{
						
						//LCD_Send_Byte( 0x01 ,0);
						if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
						{
							LCD_Data_G[0][quest_iterator] = right[quest_iterator];
							quest_iterator++;
							}
							else
							{
							quest_iterator = 0;
							correct_answer_flag = 0;
							app_state = PRINT_QUESTION;
							Question_State = QUESTION5;
						    }
						
						
					}
					else if (wrong_answer_flag == 1 )
					{
						if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
						{
							LCD_Data_G[0][quest_iterator] = wrong[quest_iterator];
							quest_iterator++;
						}
						else{
							quest_iterator = 0;
							wrong_answer_flag = 0;
							app_state = PRINT_QUESTION;
							Question_State = QUESTION5;
							
						}	
					}
					break;
				}
			}
			break;
		}
	
	
		case QUESTION5:
		{
			switch(app_state)
			{
				
				case PRINT_QUESTION:
				{
					if(Question[QUESTION5][quest_iterator] != '?')
					{
						//LCD_4Bits_Print_Character(1,quest_iterator,Question[QUESTION1][quest_iterator]);
						LCD_Data_G[0][quest_iterator]= Question[QUESTION5][quest_iterator];
						quest_iterator++;
					}
					else
					{
						//LCD_Send_Byte( 0x01 ,0);
						app_state = PRINT_CHOICES;
						quest_iterator = 0;
					}

					break;
				}
				
				
				case PRINT_CHOICES:
				{
					if(Display_choices[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
					{
						//LCD_4Bits_Print_Character(2,quest_iterator,Display_choices[quest_iterator]);
						LCD_Data_G[1][quest_iterator] = Display_choices[quest_iterator];
						quest_iterator++;
					}

					else								 // Answer Statement has been completely printed so it's time to choose your answer
					{
						//LCD_Send_Byte( 0x01 ,0);
						quest_iterator=0;
						app_state = GET_ANSWER;
					}

					break;
				}


				case GET_ANSWER:
				{
					if(	Keypad_Get_Data_from_Buffer(&Answer) )
					{
						if(Answer == Answer_of_Questions[QUESTION5] )
						{
							Correct_Answer++;
							correct_answer_flag = 1;
							LCD_Send_Byte( 0x01 ,0);
							app_state = SHOW_CORRECT_OR_NOT;
							
						}

						else if(Answer != Answer_of_Questions[QUESTION5] && Answer != SKIP)
						{
							//clear screen
							LCD_Send_Byte( 0x01 ,0);

							wrong_answer_flag = 1;
							app_state = SHOW_CORRECT_OR_NOT;

						}
						else if (Answer == SKIP)
						{
							//clear screen
							LCD_Send_Byte( 0x01 ,0);
							app_state = SHOW_CORRECT_OR_NOT;
							//Do Nothing
						}
						else
						{
							//error not availble
						}
						
						app_state = SHOW_CORRECT_OR_NOT;
						quest_iterator=0;
					}
					break;
				}
				case SHOW_CORRECT_OR_NOT:
				{
					if(fini == 0)
					{
						LCD_Send_Byte( 0x01 ,0);
						fini = 1;
					}
					else if(fini = 1)
					{
						if(correct_answer_flag = 1 )
						{
							
							//LCD_Send_Byte( 0x01 ,0);
							if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
							{
								LCD_Data_G[0][quest_iterator] = right[quest_iterator];
								quest_iterator++;
							}
							else
							{
								LCD_Send_Byte( 0x01 ,0);
								quest_iterator = 0;
								correct_answer_flag = 0;
								Question_State = RESULT;
							}
						}
						else if (wrong_answer_flag = 1 )
						{
							if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
							{
								LCD_Data_G[0][quest_iterator] = wrong[quest_iterator];
								quest_iterator++;
							}
							else
							{
								LCD_Send_Byte( 0x01 ,0);
								quest_iterator = 0;
								wrong_answer_flag = 0;
								Question_State = RESULT;
							}
						}

					}
					break;
				}
				
			}
			break;
		}
	case RESULT:
	{
		if(score[quest_iterator] != '\0')			// wait your answer choices to be completely displayed in the second line then select your answer through keypad
		{
			LCD_Data_G[0][quest_iterator] = score[quest_iterator];
			quest_iterator++;
		}
		else
		{
			//LCD_Send_Byte(0x01,0);
			LCD_Data_G[0][10]  = Correct_Answer + '0';
		}
		break;
	}
	}

}






int main()	
{
		// Testing Ports
		DDRD = 0xFF;
		DDRB = 0xFF;
		/*
		LCD_Create_Character(0,Thumb_UP);
		LCD_Create_Character(1,Thumb_Down);
		//LCD_Send_Byte(1,Thumb_UP);
		*/
		OS_Init(&OS_cnfg);
		OS_Create_Task(&Task_1_Info);
		OS_Create_Task(&Task_2_Info);
		OS_Create_Task(&Task_3_Info);
		OS_Create_Task(&Task_4_Info);
		OS_Run();

		return 0;
}

