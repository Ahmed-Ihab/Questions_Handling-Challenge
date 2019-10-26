/*
 * CFile1.c
 *
 * Created: 26/10/2019 08:18:29 م
 *  Author: COMPUTER SHOW
 */ 
#include"questionHandl.h"

/***** QUESTIONS ******/
static uint8 Question[5][16]={"ascii of . = 46?","ascii of + = 43?","ascii of E = 70?", "ascii of N = 78?","ascii of B = 67?"};
/***** Right  answers  ******/
static uint8 Answer_of_Questions[5]={YES,YES,NO,YES,NO};


	
void Question_Handling_Task (void)
{
	/*question stages*/
	static Question_State_t Question_State = QUESTION1;
	/*application stages*/
	static APP_STATE_t app_state = PRINT_QUESTION;
	/*iterator throught the whole application*/
	static uint8 quest_iterator = 0;
	/*indicating number of correct answers*/
	static uint8 Correct_Answer=0;
	/*indicating correct answer*/
	static uint8 correct_answer_flag = 0;
	/*indicating wrong answer*/
	static uint8 wrong_answer_flag = 0;
	uint8 Answer= 6;	//any values except 1 , 2 , 3
	/*choices*/
	uint8 Display_choices[]="1.YES 2.NO 3.NXT";
	/*final score*/
	uint8 score[]="yr score:";
	/*displaying right or wrong */
	uint8 right[]="CORRECT ";
	uint8 wrong[]="WRONG ";
	/*indicating end of process*/
	uint8 static end_of_app = 0;

	switch(Question_State)

	{
		case QUESTION1:
		{
			switch(app_state)
			{
				/*printing question till ?*/
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
					/*printing available  choices*/
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

				/*get answer from user by keypad*/
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
							app_state = PRINT_QUESTION;
							Question_State=QUESTION2;
							//Do Nothing
						}
						else
						{
							//error not availble
						}
						
						//app_state = SHOW_CORRECT_OR_NOT;
						quest_iterator=0;
					}
					break;
				}
				/*informing user after every question with his progress*/
				case SHOW_CORRECT_OR_NOT:
				{
					if(correct_answer_flag == 1 )
					{
						
						
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
				/*printing question till ?*/
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
				
				/*printing available  choices*/
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

				/*get answer from user by keypad*/
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
							app_state = PRINT_QUESTION;
							Question_State=QUESTION3;
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
				/*informing user after every question with his progress*/
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
				/*printing question till ?*/
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
				
				/*printing available  choices*/
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

				/*get answer from user by keypad*/
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
							app_state = PRINT_QUESTION;
							Question_State=QUESTION4;
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
				/*informing user after every question with his progress*/
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
				/*printing question till ?*/
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
				
				/*printing available  choices*/
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

				/*get answer from user by keypad*/
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
							app_state = PRINT_QUESTION;
							Question_State=QUESTION5;
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
				/*informing user after every question with his progress*/
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
				/*printing question till ?*/
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
				
				/*printing available  choices*/
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

				/*get answer from user by keypad*/
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
							//app_state = SHOW_CORRECT_OR_NOT;
							Question_State = RESULT;
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
				/*informing user after every question with his progress*/
				case SHOW_CORRECT_OR_NOT:
				{
					if(end_of_app == 0)
					{
						LCD_Send_Byte( 0x01 ,0);
						end_of_app = 1;
					}
					else if(end_of_app == 1)
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
								LCD_Send_Byte( 0x01 ,0);
								quest_iterator = 0;
								correct_answer_flag = 0;
								Question_State = RESULT;
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
		/*Displaying th scores*/
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




