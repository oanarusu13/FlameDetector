#include "uart.h"
#include "pit.h"
#include "gpio.h"
#include "adc.h"
#include "clock.h"
#include "pwm.h"

extern uint32_t timer_value;
extern int8_t led_state;

extern volatile uint8_t flag_50ms;
extern volatile uint8_t flag_100ms;
extern volatile uint8_t flag_500ms;
extern volatile uint8_t flag_1s;
extern volatile uint8_t flag_5s;

extern volatile int angle_left;
extern volatile int angle_right;

extern volatile int8_t manual_servo;

void updateLEDs(){
	
	if (led_state<0)
		led_state += 4;
	
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
	int i = 0;
	
	SystemClock_Configure();
	SystemClockTick_Configure();
	TPM2_Init();
	
	// Baudrate 14.4kbps, Oversampling rate 8
	UART0_Initialize(14400, 8);
	
	initLeds();
	PIT_Init();
	ADC0_Init();
	
	
	while(1){
		
		while(!flag_1s){
			ADC0_IRQHandler();
		}
		if (!manual_servo){
			angle_left = 1;
			angle_right = 0;
		}
		Signal_Control();
		flag_1s = 0U;

		
		while(!flag_1s){
			ADC0_IRQHandler();
		}
		
		if (!manual_servo){
			angle_left = 0;
			angle_right = 1;
		}
		Signal_Control();
		flag_1s = 0U;
	}
	
	return 0;
}
