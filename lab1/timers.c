#include "timer.h"
#include "LEDs.h"

#include <avr/interrupt.h>

// GLOBALS
extern uint32_t G_green_ticks;
extern uint32_t G_yellow_ticks;
extern uint32_t G_ms_ticks;

extern uint16_t G_red_period;
extern uint16_t G_green_period;
extern uint16_t G_yellow_period;

extern uint16_t G_release_red;

void init_timers() {
	// -------------------------  RED --------------------------------------//

	// Set mode to CTC
	// WGM2 = 0
	// WGM1 = 1
	// WGM0 = 0
	TCCR0A &= ~(0 << WGM00);
	TCCR0A |= (1 << WGM01);
	TCCR0B &= ~(0 << WGM02);
	
	//Set the scaler to 256
	// CS00 = 0
	// CS01 = 0
	// CS02 = 1
	TCCR0B &= ~(0 << CS00);
	TCCR0B &= ~(0 << CS01);
	TCCR0B |= (1 << CS02);
	
	//Set Top
	OCR0A = 78;

	//Enable output compare match interrupt on timer 0A
	TIMSK0 |= (1 << OCIE0A);

	// Initialize counter
	G_ms_ticks = 0;

	//--------------------------- YELLOW ----------------------------------//

	// WGM30 = 0
	// WGM31 = 0
	// WGM32 = 1
	// WGM33 = 0
	TCCR3A &= ~(0 << WGM00);
	TCCR3A &= ~(0 << WGM31);
	TCCR3B |= (1 << WGM32);
	TCCR3B &= ~(0 << WGM33);
	
	// Using pre-scaler 1024
	// CS30 = 1
	// CS31 = 0
	// CS32 = 1
	TCCR3B |= (1 << CS30);
	TCCR3B &= ~(0 << CS31);
	TCCR3B |= (1 << CS32);
	
	// Set TOP
	OCR3A = 195;

	//Enable output compare match interrupt on timer 3A
	TIMSK3 |= (1 << OCIE3A);

	G_yellow_ticks = 0;

	//--------------------------- GREEN ----------------------------------//
	
	// Set mode to CTC
	TCCR1A &= ~(0 << WGM10);
	TCCR1A &= ~(0 << WGM11);
	TCCR1B |= (1 << WGM12);
	TCCR1B &= ~(0 << WGM13);

	//Set to toggle
	TCCR1A |= (1 << COM1A0);
	TCCR1A &= ~(0 << COM1A1);
	
	//Set the scaler to 1026
	// CS00 = 1
	// CS01 = 0
	// CS02 = 1
	TCCR1B |= (1 << CS10);
	TCCR1B &= ~(0 << CS11);
	TCCR1B |= (1 << CS12);

	//Start G_green_period at 1000 to avoid divide by 0
	G_green_period = 1000;

	OCR1A = (uint16_t)(20000000 / 1024) / (1000 / G_green_period);

	//Set comp match so we can count toggles
	OCR1B = 1;
	TIMSK1 |= (1 << OCIE1B);
}

ISR(TIMER0_COMPA_vect) {

	// Increment ticks
	G_ms_ticks++;

	// if time to toggle the RED LED, set flag to release
	if ( ( G_ms_ticks % G_red_period ) == 0 )
		G_release_red = 1;
}
