/*
 * CFile1.c
 *
 * Created: 26/10/2019 08:15:53 م
 *  Author: COMPUTER SHOW
 */ 
#include"questionHandl.h"

void Task_Init (void);


	
uint8 Thumb_UP [7] = { 0x18 , 0x18 , 0x1F , 0x1F , 0x1F , 0x1E , 0x1C};
uint8 Thumb_Down [7] = { 0x07 , 0x0F , 0x1F , 0x1F , 0x1F , 0x03 , 0x03};

/*task structure to initialize the modules of the system*/
ST_Task_Info Task_1_Info={Task_Init,1,PERIODIC,Ready,3};
/*task structure to Update the LCD periodically */
ST_Task_Info Task_2_Info={LCD_Update,20,PERIODIC,Ready,0};
/*task structure to Update the KEYPAD periodically */
ST_Task_Info Task_3_Info={keypad_Update,50,PERIODIC,Ready,1};
/*task structure to call the main application periodically */
ST_Task_Info Task_4_Info={Question_Handling_Task,100,PERIODIC,Ready,2};


void Task_Init(void)
{
	static uint8 Init_Done_flag=0;
	static uint8 LCD_init_Done_Flag = 0;
	Function_Process_t Function_Process;
	/*modules that init only once*/
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
		/*LCD init takes multiple stages*/
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
			/*delete the init task after init-ing all the modules*/
			OS_Delete_Task(&Task_1_Info);
			//Do_NOTHING
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
		/*init-ing the OS*/
		OS_Init(&OS_cnfg);
		/*creating tasks*/
		OS_Create_Task(&Task_1_Info);
		OS_Create_Task(&Task_2_Info);
		OS_Create_Task(&Task_3_Info);
		OS_Create_Task(&Task_4_Info);
		/*calling OS to organizing tasks*/
		OS_Run();

		return 0;
}
