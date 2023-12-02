#include "uart.h"
#include "pit.h"
#include "gpio.h"

extern uint32_t timer_value;
extern uint8_t led_state;


void updateLEDs(){
		switch(led_state){
			case 0:
				digitalWrite(RED_LED_PIN, HIGH, 'b');
				digitalWrite(BLUE_LED_PIN, HIGH, 'd');
				digitalWrite(GREEN_LED_PIN, LOW, 'b');
				break;
			case 1:
				digitalWrite(RED_LED_PIN, LOW, 'b');
				digitalWrite(BLUE_LED_PIN, LOW, 'd');
				digitalWrite(GREEN_LED_PIN, HIGH, 'b');
				break;
			case 2:
				digitalWrite(RED_LED_PIN, LOW, 'b');
				digitalWrite(BLUE_LED_PIN, HIGH, 'd');
				digitalWrite(GREEN_LED_PIN, LOW, 'b');
				break;
			case 3:
				digitalWrite(RED_LED_PIN, HIGH, 'b');
				digitalWrite(BLUE_LED_PIN, HIGH, 'd');
				digitalWrite(GREEN_LED_PIN, HIGH, 'b');
				break;
			default:
				break;
		}
}

/* Configurarea LED-urilor */
void initLeds()
{
	// Semnal de ceas pentru PORT B
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	// Semnal de ceas pentru PORT D
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	// Configurarea pinului PTB18 (RED LED) ca GPIO
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurarea pinului PTB19 (GREEN LED) ca GPIO
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurarea pinului PTD1 (GREEN LED) ca GPIO
	PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(1);

	setPin(RED_LED_PIN, OUTPUT, 'b');
	setPin(GREEN_LED_PIN, OUTPUT, 'b');
	setPin(BLUE_LED_PIN, OUTPUT, 'd');
	
	digitalWrite(RED_LED_PIN, HIGH, 'b');
	digitalWrite(GREEN_LED_PIN, HIGH, 'b');
	digitalWrite(BLUE_LED_PIN, HIGH, 'd');
	
}

int main(void) {
	
	// Baudrate 14.4kbps, Oversampling rate 16
	UART0_Initialize(14400, 16);
	
	initLeds();
	PIT_Init();
	ADC0_Init();
	while(1) {
	
	}
	
	return 0;
}