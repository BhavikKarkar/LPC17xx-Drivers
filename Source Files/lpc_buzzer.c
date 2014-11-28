/******************************************************************//**
* @file		lpc_buzzer.c
* @brief	Contains all functions support for Buzzer firmware library
* 			on LPC17xx
* @version	1.0
* @date		25. Nov. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup BUZZER
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_buzzer.h"

/* Buzzer */
uint16_t timeout;     /* timeout for note */
uint16_t on_time;
uint16_t off_time;

uint16_t on_timer;
uint16_t off_timer;

Bool play_note;

/************************** LOCAL CONSTANTS *************************/
const uchar note[MELODY_LENGTH] =
{
  E5, SILENT_NOTE, E5, SILENT_NOTE, E5, SILENT_NOTE,

  C5, E5, G5, SILENT_NOTE, G4, SILENT_NOTE,

  C5, G4, SILENT_NOTE, E4, A4, B4, A_SHARP4, A4, G4,
  E5, G5, A5, F5, G5, SILENT_NOTE, E5, C5, D5, B4,

  C5, G4, SILENT_NOTE, E4, A4, B4, A_SHARP4, A4,G4,
  E5, G5, A5, F5, G5, SILENT_NOTE, E5, C5, D5, B4,

  SILENT_NOTE, G5, F_SHARP5, F5, D_SHARP5, E5, SILENT_NOTE,
  G_SHARP4, A4, C5, SILENT_NOTE, A4, C5, D5,

  SILENT_NOTE, G5, F_SHARP5, F5, D_SHARP5, E5, SILENT_NOTE,
  C6, SILENT_NOTE, C6, SILENT_NOTE, C6,

  SILENT_NOTE, G5, F_SHARP5, F5, D_SHARP5, E5, SILENT_NOTE,
  G_SHARP4, A4, C5, SILENT_NOTE, A4, C5, D5,

  SILENT_NOTE, D_SHARP5, SILENT_NOTE, D5, C5
};

const uint16_t duration[MELODY_LENGTH] =
{
  100, 25, 125, 125, 125, 125, 125, 250, 250, 250, 250, 250,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 125, 125, 125, 250, 125, 125, 200, 50, 100, 25, 500,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 250, 125, 375, 500
};


/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief       RIT interrupt handler sub-routine
 * @param[in]   None
 * @return      None
 **********************************************************************/
void RIT_IRQHandler(void)
{
    RIT_GetIntStatus(LPC_RIT); //call this to clear interrupt flag

    /* generate one note; freq + duration */
    if(play_note) /* Flag to play note */
    {
        if(timeout)  /* Note Duration */
        {
            if(on_timer)
            {
                GPIO_SetValue(BUZZER_PORT, BUZZER_PIN);    // Buzzer On
                --on_timer;
            }
            else if(off_timer)
            {
                GPIO_ClearValue(BUZZER_PORT, BUZZER_PIN);  // Buzzer Off
                --off_timer;
            }
            else
            {
                on_timer = on_time;     /* Reload on_time  */
                off_timer = off_time;   /* Reload off_time */
            }
            --timeout;  /*decrement timer */
        }
    }/* end of buzzer's code */
}

/************************** PUBLIC FUNCTIONS *************************/
/* Public Functions ----------------------------------------------------------- */
/** @addtogroup BUZZER_Public_Functions
 * @{
 */
 
/*-------------------------PUBLIC FUNCTIONS------------------------------*/
void Buzzer_Config(void)
{
    GPIO_SetDir(BUZZER_PORT, BUZZER_PIN, 1);        // Buzzer Output
    GPIO_ClearValue(BUZZER_PORT, BUZZER_PIN);       // Buzzer Off

    RIT_Config(LPC_RIT, 50, RIT_US);
}

/*********************************************************************//**
 * @brief	Set up timer 1 to play the desired frequency (in Hz) and for the the
 *          desired duration (in ms). Allowed frequencies are from 40 Hz to 5 kHz.
 * @param	freq : Frequency of the note
 * @param   dur  : Duration of the note
 * @return	None
 **********************************************************************/
void Play_Frequency(uint16_t freq, uint16_t dur)
{
	if(freq == SILENT_NOTE)
	{
		on_time = 0;               /* ON time of period */
		off_time = 0;              /* OFF time of period */

		timeout = dur * 20;        /* set timeout (duration); multiply by 20 if 20kHz */
	}
	else
	{
		on_time = freq / 2;        /* 50% duty cycle */
		off_time = freq - on_time;
		timeout = dur * 20;        /* set timeout (duration); multiply by 20 if 20kHz */
	}

	on_timer = on_time;          /* Load on_time  */
	off_timer = off_time;        /* Load off_time */
}


/*********************************************************************//**
 * @brief	This tune will be play at the end of the program
 * @param	None
 * @return	None
 **********************************************************************/
void Play_Melody(void)
{
	uchar i;
	uint16_t freq;
	uint16_t dur;

	play_note = ON;   /* start tune */

	for ( i = 0; i < MELODY_LENGTH; i++)
	{
		freq = note[i];
		dur = duration[i];
		Play_Frequency(freq, dur);
		while(timeout){ /* wait */};
	}
	play_note = OFF; /* stop tune */

	GPIO_ClearValue(BUZZER_PORT, BUZZER_PIN); // Buzzer Off
}


/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

