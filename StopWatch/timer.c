/**********************************************************************************
 * [FILE NAME]: timer.c
 *
 * [AUTHOR]: Toka Zakaria Mohamed Ramadan
 *
 * [DATE CREATED]: Oct 17, 2020
 *
 * [Description]: Function to initialize All Timers
 *
 ***********************************************************************************/

#include"timer.h"

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_Timer0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/




/**************************************************************************
 *                              Timer0
 * ************************************************************************/
ISR(TIMER0_OVF_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer0_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer0_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/**************************************************************************
 *                              Timer1
 * ************************************************************************/
ISR(TIMER1_OVF_vect)
{
	if(g_Timer1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer1_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_Timer1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer1_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/**************************************************************************
 *                              Timer2
 * ************************************************************************/
ISR(TIMER2_OVF_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer2_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_Timer2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer2_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
/*****************************************************************************************/



/***************************************************************************************************
 * [Function Name]: TIMER_init
 *
 * [Description]:  Function to Initialize Timer Driver
 *                 - Working in Interrupt Mode
 *                 - Choose Timer initial value
 *                 - Choose Timer_ID (Timer0, Timer1, Timer2)
 *                 - Choose Timer_Mode (OverFlow, Compare)
 *                 - Choose Timer compare match value if using CTC mode
 *                 - Choose Timer_Clock
 *
 *
 * [Args]:         Config_Ptr
 *
 * [In]            Config_Ptr: Pointer to Timer Configuration Structure
 *
 * [Out]           NONE
 *
 * [Returns]:      NONE
 ***************************************************************************************************/

void Timer_init(const Timer_ConfigType * Config_Ptr)
{

	switch(Config_Ptr->timer_ID)
	{

	case Timer0:

		/*Clear registers of Timer0 before accessing any of them*/
		TIMER0_CONTROL_REGIRSTER        &= 0X00;
		TIMER0_INITIAL_VALUE_REGISTER   &= 0X00;
		TIMER0_OUTPUT_COMPARE_REGISTER  &= 0X00;
		TIMER0_INTERRUPT_MASK_REGISTER  &= 0X00;

		/*
		 * Configure initial value for Timer0 to start count from it
		 * Anding with 0XFF to make sure the value won't exceed
		 * 255 as it is 8-bit Timer
		 */

		TIMER0_CONTROL_REGIRSTER = ( (Config_Ptr->timer_InitialValue) ) & 0XFF;

		/*
		 * Configure Clock Pre-scaler value for Timer0 in TCCR0 Register
		 * Anding with 0XF8 to make sure the first 3-bits in TCCR0 register=0
		 * ORing with the selected timer clock to include it in the first 3-bits
		 * and keep the other bits as they are
		 */

		TIMER0_CONTROL_REGIRSTER = (TIMER0_CONTROL_REGIRSTER  & TIMER0_CLEAR_CLOCK_BITS_VALUE) | (Config_Ptr->timer_clock);

		/*
		 * Configure compare output mode to work with normal port operation
		 * OC0 disconnected
		 * Clear COM00 bit in TCCR0 register
		 * Clear COM01 bit in TCCR0 register
		 */
		TIMER0_CONTROL_REGIRSTER  = CLEAR_BIT(TIMER0_CONTROL_REGIRSTER , TIMER0_COMPARE_OUTPUT_MODE_BIT0);
		TIMER0_CONTROL_REGIRSTER  = CLEAR_BIT(TIMER0_CONTROL_REGIRSTER , TIMER0_COMPARE_OUTPUT_MODE_BIT1);

		switch(Config_Ptr->timer_mode)
		{
		case Overflow:

			/*
			 * Configure wave form generation mode to work with normal mode
			 * Clear WGM00 bit in TCCR0 register
			 * Clear WGM01 bit in TCCR0 register
			 */
			TIMER0_CONTROL_REGIRSTER  = CLEAR_BIT(TIMER0_CONTROL_REGIRSTER , TIMER0_WAVE_FORM_GENERATION_BIT0);
			TIMER0_CONTROL_REGIRSTER  = CLEAR_BIT(TIMER0_CONTROL_REGIRSTER , TIMER0_WAVE_FORM_GENERATION_BIT1);

			/*
			 * Configure FOC0 bit in the TCCR0 register to be active
			 * As Timer0 is non-PWM
			 * Make FOC0 to be Active as it is overflow mode
			 */
			TIMER0_CONTROL_REGIRSTER = SET_BIT(TIMER0_CONTROL_REGIRSTER,TIMER0_FORCE_OUTPUT_COMPARE_BIT);


			/*
			 * Enable Timer0 overflow interrupt
			 * wait for: -Enable I-bit "mask bit" in the SREG register
			 *           -TOV0 bit in the TIFR register to be set
			 * Timer0 overflow mode is ready to work after that
			 */
			TIMER0_INTERRUPT_MASK_REGISTER = SET_BIT(TIMER0_INTERRUPT_MASK_REGISTER, TIMER0_OUTPUT_OVERFLOW_INTERRUPT);
			break;


		case Compare:


			/*
			 * Configure wave form generation mode to work with normal mode
			 * Clear WGM00 bit in TCCR0 register
			 * Clear WGM01 bit in TCCR0 register
			 */
			TIMER0_CONTROL_REGIRSTER = CLEAR_BIT(TIMER0_CONTROL_REGIRSTER, TIMER0_WAVE_FORM_GENERATION_BIT0);
			TIMER0_CONTROL_REGIRSTER = SET_BIT(TIMER0_CONTROL_REGIRSTER, TIMER0_WAVE_FORM_GENERATION_BIT1);

			/*
			 * Configure FOC0 bit in the TCCR0 register to be active
			 * As Timer0 is non-PWM
			 * Make FOC0 to be Active as it is compare mode
			 */
			TIMER0_CONTROL_REGIRSTER = SET_BIT(TIMER0_CONTROL_REGIRSTER, TIMER0_FORCE_OUTPUT_COMPARE_BIT);

			/*
			 * Configure Compare match value for Timer0 to start count from it
			 * Anding with 0XFF to make sure the value won't exceed
			 * 255 as it is 8-bit Timer
			 */
			TIMER0_OUTPUT_COMPARE_REGISTER = ((Config_Ptr->timer_compare_MatchValue)) & 0XFF;

			/*
			 * Enable Timer0 compare match interrupt
			 * wait for: -Enable I-bit "mask bit" in the SREG register
			 *           -OCF0 bit in the TIFR register to be set
			 * Timer0 compare match mode is ready to work after that
			 */
			TIMER0_INTERRUPT_MASK_REGISTER = SET_BIT(TIMER0_INTERRUPT_MASK_REGISTER, TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT);


			break;
		} /*End of internal switch case*/

		break; /*End ofTimer0*/


		case Timer1:

			/*Clear registers of Timer1 before accessing any of them*/
			TIMER1_CONTROL_REGIRSTER_A        &= 0X00;
			TIMER1_CONTROL_REGIRSTER_B        &= 0X00;
			TIMER1_INITIAL_VALUE_REGISTER     &= 0X00;
			TIMER1_OUTPUT_COMPARE_REGISTER_A  &= 0X00;
			TIMER1_INTERRUPT_MASK_REGISTER    &= 0X00;

			/*
			 * Configure initial value for Timer1 to start count from it
			 * Anding with 0XFF to make sure the value won't exceed
			 *  0XFFFF as it is 16-bit Timer
			 */

			TIMER1_INITIAL_VALUE_REGISTER = ( (Config_Ptr->timer_InitialValue) ) & 0XFFFF;

			/*
			 * Configure Clock Pre-scaler value for Timer0 in TCCR1B Register
			 * Anding with 0XF8 to make sure the first 3-bits in TCCR1B register=0
			 * ORing with the selected timer clock to include it in the first 3-bits
			 * and keep the other bits as they are
			 */

			TIMER1_CONTROL_REGIRSTER_B =  (TIMER1_CONTROL_REGIRSTER_B & 0XF8) | (Config_Ptr->timer_clock);

			/*
			 * Configure compare output mode to work with normal port operation
			 * OC1A/OC1B disconnected
			 * Clear COM1A0/COM1A1 bits in TCCR1A register
			 * Clear COM1B0/COM1B1 bits in TCCR1A register
			 */
			TIMER1_CONTROL_REGIRSTER_A = TIMER1_CONTROL_REGIRSTER_A & 0X0F;


			switch(Config_Ptr->timer_mode)
			{
			case Overflow:

				/*
				 * Configure wave form generation mode to work with normal mode
				 * Clear WGM10/WMG11 bit in TCCR1A register
				 * Clear WGM12/WGM13 bit in TCCR1B register
				 */
				TIMER1_CONTROL_REGIRSTER_A = TIMER1_CONTROL_REGIRSTER_A & 0XFC ;
				TIMER1_CONTROL_REGIRSTER_B = TIMER1_CONTROL_REGIRSTER_B & 0XE7 ;

				/*
				 * Configure FOC1A/FOC1B bit in the TCCR1 register to be active
				 * As Timer1 is non-PWM
				 * Make FOC1A/FOC1B to be Active as it is overflow mode
				 */
				TIMER1_CONTROL_REGIRSTER_A =  SET_BIT(TIMER1_CONTROL_REGIRSTER_A,TIMER1_FORCE_OUTPUT_COMPARE_BIT_A);
				TIMER1_CONTROL_REGIRSTER_A =  SET_BIT(TIMER1_CONTROL_REGIRSTER_A,TIMER1_FORCE_OUTPUT_COMPARE_BIT_B);


				/*
				 * Enable Timer1 overflow interrupt
				 * wait for: -Enable I-bit "mask bit" in the SREG register
				 *           -TOV1 bit in the TIFR register to be set
				 * Timer1 overflow mode is ready to work after that
				 */
				TIMER1_INTERRUPT_MASK_REGISTER = SET_BIT(TIMER1_INTERRUPT_MASK_REGISTER,TIMER1_OUTPUT_OVERFLOW_INTERRUPT);

				break;


			case Compare:


				/*
				 * Configure wave form generation mode to work with normal mode
				 * Clear WGM10/WGM11 bits in TCCR1A register
				 * Set WGM12 bit in TCCR1B register
				 * Clear WGM13 bit in TCCR1B register
				 */
				TIMER1_CONTROL_REGIRSTER_A = TIMER1_CONTROL_REGIRSTER_A & 0XFC ;
				TIMER1_CONTROL_REGIRSTER_B = SET_BIT(TIMER1_CONTROL_REGIRSTER_B, TIMER1_WAVE_FORM_GENERATION_BIT12);
				TIMER1_CONTROL_REGIRSTER_B = CLEAR_BIT(TIMER1_CONTROL_REGIRSTER_B, TIMER1_WAVE_FORM_GENERATION_BIT13);

				/*
				 * Configure FOC1A/FOC1B bit in the TCCR1 register to be active
				 * As Timer1 is non-PWM
				 * Make FOC1A/FOC1B to be Active as it is compare mode
				 */
				TIMER1_CONTROL_REGIRSTER_A =  SET_BIT(TIMER1_CONTROL_REGIRSTER_A,TIMER1_FORCE_OUTPUT_COMPARE_BIT_A);
				TIMER1_CONTROL_REGIRSTER_A =  SET_BIT(TIMER1_CONTROL_REGIRSTER_A,TIMER1_FORCE_OUTPUT_COMPARE_BIT_B);


				/*
				 * Configure Compare match value for Timer1 to start count from it
				 * Anding with 0XFFFF to make sure the value won't exceed
				 * OXFFFF as it is 16-bit Timer
				 */
				TIMER1_OUTPUT_COMPARE_REGISTER_A = ((Config_Ptr->timer_compare_MatchValue)) & 0XFFFF;

				/*
				 * Enable Timer1 compare match interrupt
				 * wait for: -Enable I-bit "mask bit" in the SREG register
				 *           -OCF1 bit in the TIFR register to be set
				 * Timer1 compare match mode is ready to work after that
				 */
				TIMER1_INTERRUPT_MASK_REGISTER = SET_BIT(TIMER1_INTERRUPT_MASK_REGISTER,TIMER1_OUTPUT_COMPARE_MATCH_INTERRUPT);


				break;
			} /*End of internal switch case*/



			break; /*End of Timer1*/



			case Timer2:

				/*Clear registers of Timer0 before accessing any of them*/
				TIMER2_CONTROL_REGIRSTER        &= 0X00;
				TIMER2_INITIAL_VALUE_REGISTER   &= 0X00;
				TIMER2_OUTPUT_COMPARE_REGISTER  &= 0X00;
				TIMER2_INTERRUPT_MASK_REGISTER  &= 0X00;
				/*************************************************************************/
				ASSR  &= 0X00;
				/*******************************************************************************/
				/*
				 * Configure initial value for Timer2 to start count from it
				 * Anding with 0XFF to make sure the value won't exceed
				 * 255 as it is 8-bit Timer
				 */

				TIMER2_INITIAL_VALUE_REGISTER = ( (Config_Ptr->timer_InitialValue) ) & 0XFF;

				/*
				 * Configure Clock Pre-scaler value for Timer0 in TCCR2 Register
				 * Anding with 0XF8 to make sure the first 3-bits in TCCR2 register=0
				 * ORing with the selected timer clock to include it in the first 3-bits
				 * and keep the other bits as they are
				 */

				TIMER2_CONTROL_REGIRSTER = (TIMER2_CONTROL_REGIRSTER  & TIMER2_CLEAR_CLOCK_BITS_VALUE) | (Config_Ptr->timer_clock);

				/*
				 * Configure compare output mode to work with normal port operation
				 * OC2 disconnected
				 * Clear COM20 bit in TCCR2 register
				 * Clear COM21 bit in TCCR2 register
				 */
				TIMER2_CONTROL_REGIRSTER  = CLEAR_BIT(TIMER2_CONTROL_REGIRSTER, TIMER2_COMPARE_OUTPUT_MODE_BIT0);
				TIMER2_CONTROL_REGIRSTER  = CLEAR_BIT(TIMER2_CONTROL_REGIRSTER, TIMER2_COMPARE_OUTPUT_MODE_BIT1);

				switch(Config_Ptr->timer_mode)
				{
				case Overflow:

					/*
					 * Configure wave form generation mode to work with normal mode
					 * Clear WGM00 bit in TCCR0 register
					 * Clear WGM01 bit in TCCR0 register
					 */
					TIMER2_CONTROL_REGIRSTER  = CLEAR_BIT(TIMER2_CONTROL_REGIRSTER , TIMER2_WAVE_FORM_GENERATION_BIT0);
					TIMER2_CONTROL_REGIRSTER  = CLEAR_BIT(TIMER2_CONTROL_REGIRSTER , TIMER2_WAVE_FORM_GENERATION_BIT1);

					/*
					 * Configure FOC2 bit in the TCCR2 register to be active
					 * As Timer2 is non-PWM
					 * Make FOC2 to be Active as it is overflow mode
					 */
					TIMER2_CONTROL_REGIRSTER =  SET_BIT(TIMER2_CONTROL_REGIRSTER ,TIMER2_FORCE_OUTPUT_COMPARE_BIT);


					/*
					 * Enable Timer0 overflow interrupt
					 * wait for: -Enable I-bit "mask bit" in the SREG register
					 *           -TOV0 bit in the TIFR register to be set
					 * Timer0 overflow mode is ready to work after that
					 */
					TIMER2_INTERRUPT_MASK_REGISTER = SET_BIT(TIMER2_INTERRUPT_MASK_REGISTER,TIMER2_OUTPUT_OVERFLOW_INTERRUPT);

					break;


				case Compare:


					/*
					 * Configure wave form generation mode to work with normal mode
					 * Clear WGM20 bit in TCCR2 register
					 * Clear WGM21 bit in TCCR2 register
					 */
					TIMER2_CONTROL_REGIRSTER  = CLEAR_BIT(TIMER2_CONTROL_REGIRSTER , TIMER2_WAVE_FORM_GENERATION_BIT0);
					TIMER2_CONTROL_REGIRSTER  = SET_BIT(TIMER2_CONTROL_REGIRSTER , TIMER2_WAVE_FORM_GENERATION_BIT1);

					/*
					 * Configure FOC2 bit in the TCCR2 register to be active
					 * As Timer2 is non-PWM
					 * Make FOC2 to be Active as it is compare mode
					 */
					TIMER2_CONTROL_REGIRSTER =  SET_BIT(TIMER2_CONTROL_REGIRSTER , TIMER2_FORCE_OUTPUT_COMPARE_BIT);

					/*
					 * Configure Compare match value for Timer2 to start count from it
					 * Anding with 0XFF to make sure the value won't exceed
					 * 255 as it is 8-bit Timer
					 */
					TIMER2_OUTPUT_COMPARE_REGISTER = ((Config_Ptr->timer_compare_MatchValue)) & 0XFF;

					/*
					 * Enable Timer2 compare match interrupt
					 * wait for: -Enable I-bit "mask bit" in the SREG register
					 *           -OCF2 bit in the TIFR register to be set
					 * Timer2 compare match mode is ready to work after that
					 */
					TIMER2_INTERRUPT_MASK_REGISTER = SET_BIT(TIMER2_INTERRUPT_MASK_REGISTER, TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT);


					break;
				} /*End of internal switch case*/


				break; /*End of Timer2*/

	}/*End of Super switch case*/

}/*End of the Timer_init*/


/***************************************************************************************************
 * [Function Name]: Timer_setCallBack
 *
 * [Description]:  Function to set the Call Back function address.
 *
 * [Args]:         a_Ptr, timer_type
 *
 * [In]            a_Ptr: -Pointer to function
 *                        -To use it to save receive the function call back name
 *                        -To store it in the global pointer to function to use it in
 *
 *                 timer_type: -Variable from type enum Timer_Mode
 *                             -To use it to choose the type of the timer
 *
 * [Out]           NONE
 *
 * [Returns]:      NONE
 ***************************************************************************************************/
void Timer_setCallBack(void(*a_ptr)(void), Timer_Type timer_type )
{

	switch(timer_type)
	{
	case Timer0:
		/* Save the address of the Call back function in a global variable */
		g_Timer0_callBackPtr = a_ptr;
		break;

	case Timer1:
		/* Save the address of the Call back function in a global variable */
		g_Timer1_callBackPtr = a_ptr;
		break;

	case Timer2:
		/* Save the address of the Call back function in a global variable */
		g_Timer2_callBackPtr = a_ptr;
		break;
	} /*End of the switch case*/

}/*End of the setCallBack function*/



/***************************************************************************************************
 * [Function Name]: Timer_setCallBack
 *
 * [Description]:  Function to stop the clock of the timer to stop incrementing
 *
 * [Args]:         timer_type
 *
 *
 * [In]            timer_type: -Variable from type enum Timer_Mode
 *                             -To use it to choose the type of the timer
 *                             -stop the required timer
 *
 * [Out]           NONE
 *
 * [Returns]:      NONE
 ***************************************************************************************************/


void Timer_stop(Timer_Type timer_type)
{

	switch(timer_type)
	{
	case Timer0:
		/*
		 * Clear the first 3-bits in TCCR0 register
		 * stop the clock of the timer
		 * timer0 will stop incrementing
		 */
		TIMER0_CONTROL_REGIRSTER = (TIMER0_CONTROL_REGIRSTER  & TIMER0_CLEAR_CLOCK_BITS_VALUE) ;
		break;

	case Timer1:

		/*
		 * Clear the first 3-bits in TCCR1B register
		 * stop the clock of the timer
		 * timer1 will stop incrementing
		 */
		TIMER1_CONTROL_REGIRSTER_B  =  (TIMER1_CONTROL_REGIRSTER_B  & 0XF8) ;
		break;


	case Timer2:
		/*
		 * Clear the first 3-bits in TCCR2 register
		 * stop the clock of the timer
		 * timer2 will stop incrementing
		 */
		TIMER2_CONTROL_REGIRSTER = (TIMER2_CONTROL_REGIRSTER  & TIMER2_CLEAR_CLOCK_BITS_VALUE) ;
		break;

	} /*End of the switch case*/


}/*End of the Timer_stop function*/


/***************************************************************************************************
 * [Function Name]: Timer_setCallBack
 *
 * [Description]:  Function to DeInit the timer to start again from beginning
 *
 * [Args]:         timer_type
 *
 *
 * [In]            timer_type: -Variable from type enum Timer_Mode
 *                             -To use it to choose the type of the timer
 *                             -DeInit the required timer
 *
 * [Out]           NONE
 *
 * [Returns]:      NONE
 ***************************************************************************************************/

void Timer_DeInit(Timer_Type timer_type)
{
	switch(timer_type)
	{
	case Timer0:

		/*Clear all register in Timer0*/
		TIMER0_CONTROL_REGIRSTER        &= 0X00;
		TIMER0_INITIAL_VALUE_REGISTER   &= 0X00;
		TIMER0_OUTPUT_COMPARE_REGISTER  &= 0X00;
		TIMER0_INTERRUPT_MASK_REGISTER  &= 0X00;
		break;

	case Timer1:

		/*Clear all register in Timer1*/
		TIMER1_CONTROL_REGIRSTER_A         &= 0X00;
		TIMER1_CONTROL_REGIRSTER_B         &= 0X00;
		TIMER1_INITIAL_VALUE_REGISTER      &= 0X00;
		TIMER1_OUTPUT_COMPARE_REGISTER_A   &= 0X00;
		TIMER1_INTERRUPT_MASK_REGISTER     &= 0X00;
		break;


	case Timer2:

		/*Clear all register in Timer2*/
		TIMER2_CONTROL_REGIRSTER        &= 0X00;
		TIMER2_INITIAL_VALUE_REGISTER   &= 0X00;
		TIMER2_OUTPUT_COMPARE_REGISTER  &= 0X00;
		TIMER2_INTERRUPT_MASK_REGISTER  &= 0X00;
		break;

	} /*End of the switch case*/


}/*end of the Timer_DeInit function*/
