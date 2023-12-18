#include "MKL25Z4.h"

#define RED_LED_PIN (18) // PORT B RED LED
#define GREEN_LED_PIN (19) // PORT B GREEN LED
#define BLUE_LED_PIN (1) // PORT D BLUE LED

#ifndef OUTPUT
	#define OUTPUT 1
#endif

#ifndef INPUT
	#define INPUT 0
#endif

#ifndef HIGH
	#define HIGH 1
#endif

#ifndef LOW
	#define LOW 0
#endif

#

void setPin(uint8_t pin, uint8_t state, uint8_t port);
void digitalWrite(uint8_t pin, uint8_t state, uint8_t port);