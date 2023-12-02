#include "MKL25Z4.h"

#define RED_LED_PIN (18) // PORT B RED LED
#define GREEN_LED_PIN (19) // PORT B GREEN LED
#define BLUE_LED_PIN (1) // PORT D BLUE LED

#define OUTPUT 1
#define INPUT 0

#define HIGH 1
#define LOW 0

void setPin(uint8_t pin, uint8_t state, uint8_t port);
void digitalWrite(uint8_t pin, uint8_t state, uint8_t port);