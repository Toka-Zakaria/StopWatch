/**********************************************************************************
 * [FILE NAME]:  application_functions.c
 *
 * [AUTHOR]: Toka Zakaria Mohamed Ramadan
 *
 * [DATE CREATED]: Oct 17, 2020
 *
 * [Description]: File to contain all functions of the application.
 ***********************************************************************************/


#include"application_functions.h"

/***************************************************************************************************
 * [Function Name]: Reset_stopWatch
 *
 * [Description]:  Function to reset the stopWatch to restart
 *                 the stopWatch to begin from 0 again
 *
 * [Args]:         NONE
 *
 *
 * [In]            NONE
 *
 * [Out]           NONE
 *
 * [Returns]:      NONE
 ***************************************************************************************************/
void Reset_stopWatch(void)
{

	/*
	 * Clear all variables of the StopWatch to
	 * return increment from the begin
	 */
	g_seconds=0;
	g_minutes=0;
	g_hours=0;
}/*end of Reset_stopWatch function*/

/***************************************************************************************************
 * [Function Name]: Stop_stopWatch
 *
 * [Description]:  Function to stop the stopWatch to freeze
 *                 on the value which stopped at
 *
 * [Args]:         NONE
 *
 *
 * [In]            NONE
 *
 * [Out]           NONE
 *
 * [Returns]:      NONE
 ***************************************************************************************************/
void Stop_stopWatch(void)
{
	/*
	 * call the function of stop timer
	 * to clear the bits of the clock timer
	 */
	Timer_stop(Timer1);
}/*end of Stop_stopWatch function*/

/***************************************************************************************************
 * [Function Name]: Resume_stopWatch
 *
 * [Description]:  Function to force the stopWatch to work
 *                 from the final value it stopped at
 *
 * [Args]:         NONE
 *
 *
 * [In]            NONE
 *
 * [Out]           NONE
 *
 * [Returns]:      NONE
 ***************************************************************************************************/
void Resume_stopWatch(void)
{
	/*
	 * start to begin the timer again
	 */
	Timer_init(&timer);

}/*end of Resume_stopWatch function*/
/***************************************************************************************************
 * [Function Name]: DetectSecond
 *
 * [Description]:  Function to increment the a variable which
 *                 act as an interrupt occur
 *
 * [Args]:         NONE
 *
 *
 * [In]            NONE
 *
 * [Out]           NONE
 *
 * [Returns]:      NONE
 ***************************************************************************************************/
void DetectSecond(void)
{
	/*
	 * Increment a variable to check how many times the ISR happen
	 */
	g_tick++;

}/*end of DetectSecond function*/
