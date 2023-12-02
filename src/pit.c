#include "pit.h"
#include "uart.h"

uint32_t timer_value;
uint8_t led_state;

extern void updateLEDs();

void PIT_Init(void) {
	
	// Activarea semnalului de ceas pentru perifericul PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	// Utilizarea semnalului de ceas pentru tabloul de timere
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;
	// Oprirea decrementarii valorilor numaratoarelor in modul debug
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	// Setarea valoarea numaratorului de pe canalul 0 la o perioada de 0.286 secunde
	PIT->CHANNEL[0].LDVAL = 2998926;//0x9FFFFF;
	
  // Activarea întreruperilor pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	// Setarea valoarea numaratorului de pe canalul 1 la o perioada de 10 secunde
	PIT->CHANNEL[1].LDVAL = 0x63FFFFF;
	
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
		led_state++;
		led_state %= 4;
		updateLEDs();
	}
	
	if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	}
}