#include "MKL25Z4.h"

#define ADC_CHANNEL (11) // PORT C PIN 2
extern int volatile flame_status;
void ADC0_Init(void);
int ADC0_Calibrate(void);
uint16_t ADC0_Read(void);
void ADC0_IRQHandler(void);
