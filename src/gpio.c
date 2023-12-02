#include "gpio.h"

extern led_state;

void setPin(uint8_t pin, uint8_t state, uint8_t port){
	// Siguranta -> status e ori 0 ori 1
	state = state % 2;
	switch(port){
		case 'a':				
			GPIOA->PDDR |= (state << pin);
			break;
		case 'b':
			GPIOB->PDDR |= (state << pin);
			break;
		case 'c':
			GPIOC->PDDR |= (state << pin);
			break;
		case 'd':
			GPIOD->PDDR |= (state << pin);
			break;
		default:
			break;
	}
}

void digitalWrite(uint8_t pin, uint8_t state, uint8_t port){
	// Siguranta -> status e ori 0 ori 1
	state = state % 2;
	switch(port){
		case 'a':
			GPIOA->PCOR |= (1U << pin);
			GPIOA->PSOR |= (state << pin);
			break;
		case 'b':
			GPIOB->PCOR |= (1U << pin);
			GPIOB->PSOR |= (state << pin);
			break;
		case 'c':      
			GPIOC->PCOR |= (1U << pin);
			GPIOC->PSOR |= (state << pin);
			break;
		case 'd':
			GPIOD->PCOR |= (1U << pin);
			GPIOD->PSOR |= (state << pin);
			break;
		default:
			break;
	}
}