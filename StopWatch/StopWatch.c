/**********************************************************************************
 * [FILE NAME]:  StopWatch.c
 *
 * [AUTHOR]: Toka Zakaria Mohamed Ramadan
 *
 * [DATE CREATED]: Oct 17, 2020
 *
 * [Description]:StopWatch based on ATmega16 microcontroller and
 *               six 7-Segments connected to microcontroller using only 10 GPIO
 *               pins "4 pins to 7-Segment and 6 pins for Transistor for selection",
 *               also using 3 buttons for stop, resume ,and reset options
 *
 ***********************************************************************************/

#include"application_functions.h"

/******************************************************************************************
 *                                      Global Variables
 *****************************************************************************************/

/*
 * Variable to check if the interrupt of TIMER1 happen or not
 * global to use it in external function
 */
uint8 g_tick=0;
/*
 * Variable to increment the value of the seconds
 *global to use it in external function
 *
 */
uint8 g_seconds=0;
/*
 * Variable to increment the value of the minutes
 * global to use it in external function
 *
 */
uint8 g_minutes=0;

/*
 * Variable to increment the value of the hours
 * global to use it in external function
 *
 */
uint8 g_hours=0;


/*
 * initialize the configure structure of the timer
 * choosing value in order to form the structure
 * global to us it in external functions
 */

Timer_ConfigType timer= { INITIAL_VALUE, COMPARE_VALUE, Timer1, F_CPU_1024,Compare};


/******************************************************************************************
 *                                   START OF THE PROGRAM
 *****************************************************************************************/

int main(void)
{
	/******************************************************************************************
	 *                                      INITIALIZATION
	 *****************************************************************************************/
	/*
	 * Configure first 4 pins in PORTC as output pins (Data Lines)
	 */
	SEVEN_SEGMENTS_DIRECTION_DATA_LINE_PORT |= 0X0F;
	/*
	 * configure first 6 pins in PORTA As output pins(control lines)
	 */
	SEVEN_SEGMENTS_DIRECTION_CONTROL_LINE_PORT |= 0X3F;
	/*
	 * initial value 5 volt to display all segments in
	 * the same time for the first time
	 */
	SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT |= 0X3F;
	/*
	 * Initial value for the begin of the 7 segment
	 *all segments = 0 in the first
	 */
	SEVEN_SEGMENTS_DATA_DATA_LINE_PORT = SEVEN_SEGMENTS_DATA_DATA_LINE_PORT & 0XF0;

	/*
	 * Enable i-bit in the SREG register
	 */
	SREG |= (1<<7);

	/*
	 * initialize the configure structure of external
	 * interrup0 choosing value in order to
	 * form the structure.
	 */
	External_Interrupt_ConfigType reset= {INTERRUPT0, Falling};
	/*
	 * initialize the configure structure of external
	 * interrup1 choosing value in order to
	 * form the structure.
	 */
	External_Interrupt_ConfigType stop= {INTERRUPT1, Raising};
	/*
	 * initialize the configure structure of external
	 * interrup2 choosing value in order to
	 * form the structure.
	 */
	External_Interrupt_ConfigType resume= {INTERRUPT2, Falling};

	/*
	 * call the function Timer_setCallBack and give it
	 * the address of function DetectSecond to force the timer ISR
	 * to call this function every interrupt occur, "every one second"
	 */
	Timer_setCallBack(DetectSecond, Timer1);

	/*
	 * Start the timer to count
	 * give it the address of the declared structure
	 * to put its values in the initialization function
	 * in the entire registers of the timer
	 */
	Timer_init(&timer);

	/*
	 * call the function Interrupt_setCallBack and give it
	 * the address of function Reset_stopWatch to force the timer ISR
	 * to call this function every interrupt occur
	 */
	Interrupt_setCallBack(Reset_stopWatch, INTERRUPT0);
	/*
	 * Start the interrupt to detect edges
	 * give it the address of the declared structure
	 * to put its values in the initialization function
	 * in the entire registers of interrupt1
	 */
	External_Interrupt_init(&reset);

	/*
	 * call the function Interrupt_setCallBack and give it
	 * the address of function Reset_stopWatch to force the timer ISR
	 * to call this function every interrupt occur
	 */
	Interrupt_setCallBack(Stop_stopWatch, INTERRUPT1);
	/*
	 * Start the interrupt to detect edges
	 * give it the address of the declared structure
	 * to put its values in the initialization function
	 * in the entire registers of interrupt1
	 */
	External_Interrupt_init(&stop);

	/*
	 * call the function Interrupt_setCallBack and give it
	 * the address of function Reset_stopWatch to force the timer ISR
	 * to call this function every interrupt occur
	 */
	Interrupt_setCallBack(Resume_stopWatch, INTERRUPT2);
	/*
	 * Start the interrupt to detect edges
	 * give it the address of the declared structure
	 * to put its values in the initialization function
	 * in the entire registers of interrupt1
	 */
	External_Interrupt_init(&resume);

	/******************************************************************************************
	 *                                  EVERYTIME APPLICATION
	 *                                       SUPER LOOP
	 *****************************************************************************************/
	while(1)
	{

		/*
		 * Check if the interrupt of the Timer1 happen or not
		 */
		if (g_tick == 1)
		{
			/*
			 * Increment the seconds of the StopWatch
			 */
			g_seconds++;
			/*
			 * Check if 1 minute passed or not if yes increment
			 *minutes1 and start the seconds to increment
			 * from 0 again
			 */
			if(g_seconds == MAXIMUM_SECONDS)
			{

				/*
				 * clear the variable of seconds to begin from start again
				 * indicate that 1 minute passed
				 */
				g_seconds= INITIAL_COUNT;
				/*
				 * Increment variable of minutes to start appear on
				 * the 7 segments, incrementing every 60 seconds
				 */
				g_minutes++;

			}/*End of seconds condition*/


			/*
			 * Check if 1 hour passed or not if yes increment hours 1
			 * and start the Minutes & Seconds to increment from 0 again
			 */
			if(g_minutes == MAXIMUM_MINUTES)
			{

				/*
				 * clear the variable of seconds to begin from start again
				 * indicate that 1 minute passed
				 */
				g_seconds = INITIAL_COUNT;
				/*
				 * clear the variable of minutes to begin from start again
				 * indicate that 1 hour passed
				 */
				g_minutes = INITIAL_COUNT;
				/*
				 * Increment variable of hours to start appear on
				 * the 7 segments, incrementing every 60 minutes
				 */
				g_hours++;

			}/*end of minutes condition*/


			/*
			 * Check if 1 hour passed or not if yes increment hours 1
			 *and start the Minutes & Seconds hours to increment from 0 again
			 *and this condition is true every 1000 melly seconds
			 */

			if(g_hours == MAXIMUM_HOUR)
			{

				/*
				 * clear the variable of seconds to begin from start again
				 * indicate that 1 minute passed
				 */
				g_seconds = INITIAL_COUNT;
				/*
				 * clear the variable of minutes to begin from start again
				 * indicate that 1 hour passed
				 */
				g_minutes = INITIAL_COUNT;
				/*
				 * clear the variable of minutes to begin from start again
				 * indicate that 1 day passed
				 */
				g_hours = INITIAL_COUNT;
			}

			/*
			 * Return the value of tick to be 0 again to be
			 * ready for the next
			 */
			g_tick = INITIAL_COUNT;

		}/*end of first if statement*/


		/******************************************************************************************
		 *                            SEVEN_SEGMENTS MULTIPLIXING
		 *****************************************************************************************/
		/*
		 * Description of these block code: The Timer interrupt happen every
		 * one second, the duration of every iteration of the loop
		 * is 30 melly second and 1 second has 1000 melly seconds, so the
		 * tick will increment every 1000 melly second but tick stay 0
		 * every 1000/30 iteration so we have to save the values of digits
		 * while incrementing these iteration so if the tick != 1,and
		 * this will happen every 30 melly second iteration until it reaches
		 * 100 melly seconds, so if the condition false we have to save the
		 * values on the digits until the next increment
		 */

		/****************************************************************************************/

		/******************************************************************************************
		 *                                   Display Seconds
		 *****************************************************************************************/

		/*
		 * Display first segment only,turn off the others, display the data
		 */
		SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT = (SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT & 0XC0) | SEGMENT1;
		SEVEN_SEGMENTS_DATA_DATA_LINE_PORT = (SEVEN_SEGMENTS_DATA_DATA_LINE_PORT & 0XF0) | ( (g_seconds%10) & 0X0F);
		/*
		 * wait 1 ms to give it chance to appear on 7segemnt
		 */
		_delay_ms(1);

		/*
		 * Display second segment only,turn off the others, display the data
		 */
		SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT = (SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT & 0XC0) | SEGMENT2;
		SEVEN_SEGMENTS_DATA_DATA_LINE_PORT = (SEVEN_SEGMENTS_DATA_DATA_LINE_PORT & 0XF0) | ( (g_seconds/10) & 0X0F);
		/*
		 * wait 1 ms to give it chance to appear on 7segemnt
		 */
		_delay_ms(1);

		/******************************************************************************************
		 *                                   Display Minutes
		 *****************************************************************************************/

		/*
		 * Display third segment only,turn off the others, display the data
		 */
		SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT = (SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT & 0XC0) | SEGMENT3;
		SEVEN_SEGMENTS_DATA_DATA_LINE_PORT = (SEVEN_SEGMENTS_DATA_DATA_LINE_PORT & 0XF0) | ( (g_minutes%10) & 0X0F);
		/*
		 * wait 1 ms to give it chance to appear on 7segemnt
		 */
		_delay_ms(1);


		/*
		 * Display fourth segment only,turn off the others, display the data
		 */
		SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT = (SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT & 0XC0) | SEGMENT4;
		SEVEN_SEGMENTS_DATA_DATA_LINE_PORT = (SEVEN_SEGMENTS_DATA_DATA_LINE_PORT & 0XF0) | ( (g_minutes/10) & 0X0F);
		/*
		 * wait 1 ms to give it chance to appear on 7segemnt
		 */
		_delay_ms(1);



		/******************************************************************************************
		 *                                   Display Hours
		 *****************************************************************************************/

		/*
		 * Display fifth segment only,turn off the others, display the data
		 */
		SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT = (SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT & 0XC0) | SEGMENT5;
		SEVEN_SEGMENTS_DATA_DATA_LINE_PORT = (SEVEN_SEGMENTS_DATA_DATA_LINE_PORT & 0XF0) | ( (g_hours%10) & 0X0F);
		/*
		 * wait 1 ms to give it chance to appear on 7segemnt
		 */
		_delay_ms(1);

		/*
		 * Display sixth segment only,turn off the others, display the data
		 */
		SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT = (SEVEN_SEGMENTS_CONTROL_CONTROL_LINE_PORT & 0XC0) | SEGMENT6;
		SEVEN_SEGMENTS_DATA_DATA_LINE_PORT = (SEVEN_SEGMENTS_DATA_DATA_LINE_PORT & 0XF0) | ( (g_hours/10) & 0X0F);
		/*
		 * wait 1 ms to give it chance to appear on 7segemnt
		 */
		_delay_ms(1);



	}/*end of Super loop*/

}/*end of main*/

