#include "pit.h"
#include "uart.h"
#include "gpio.h"

uint32_t timer_value;
int8_t led_state;

volatile int8_t reverse_leds = 0;
static const float ratio_sys_tick = 2.2918;

extern void updateLEDs();

void PIT_Init(void) {

	
	// Activarea semnalului de ceas pentru perifericul PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	// Utilizarea semnalului de ceas pentru tabloul de timere
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;
	// Oprirea decrementarii valorilor numaratoarelor in modul debug
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	// Setarea valoarea numaratorului de pe canalul 0 la o perioada de 0.286 secunde
	uint32_t ldval = (uint32_t)(0.286f * 24000000UL) - 1;
	PIT->CHANNEL[0].LDVAL = ldval;
	//PIT->CHANNEL[0].LDVAL = 2998926;//0x9FFFFF;

  // Activarea întreruperilor pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	// Setarea valoarea numaratorului de pe canalul 1 la o perioada de 1 milisecunda
	PIT->CHANNEL[1].LDVAL = 0x9FFFFF;
	
	// Activara întreruperilor pe canalul 1
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 1
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	
	// Activarea întreruperii mascabile si setarea prioritatiis
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler(void) {
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		if (reverse_leds){
			led_state--;
		}
		else {
			led_state++;
			led_state %= 4;
		}
		updateLEDs();
	}
	
	if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK; 
	}
}