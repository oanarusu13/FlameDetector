#include "uart.h"

extern volatile int angle_left;
extern volatile int angle_right;

extern volatile int8_t reverse_leds;
extern volatile int8_t manual_servo;


/* Transmite un string */
void UART0_Transmit_String(uint8_t* str) {
	uint8_t i = 0;
	while(str[i] != '\0'){
		UART0_Transmit(str[i++]);
	}
}

/* Transmite un octet de date */
void UART0_Transmit(uint8_t data) {
	// Utilizam masca TDRE pentru a verifica
	// disponibilitatea buffer-ului de transmisie
	
	// Asteapta ca buffer-ul e transmisie sa fie disponibil
	// UART->D = registrul de date al UART
	while(!(UART0->S1 & UART_S1_TDRE_MASK)) {}
	UART0->D = data;
}

/* Primeste un octet de date */
uint8_t UART0_Receive(void) {
	// Utilizam masca RDRF pentru a verifica
	// disponibilitatea buffer-ului de receptie
	
	// Asteapta ca buffer-ul e transmisie sa fie ocupat
	// UART->D = registrul de date al UART
	while(!(UART0->S1 & UART_S1_RDRF_MASK)) {}
	return UART0->D;
}

/* Initializeaza UART0 cu parametrii dati */
void UART0_Initialize(uint32_t baud_rate, uint16_t osr) {
	uint16_t sbr;
	uint32_t sys_clock = 48000000UL;
	
	// Activeaza ceasul pentru modulul UART0
	SIM->SCGC4 = SIM->SCGC4 | SIM_SCGC4_UART0_MASK;

	// Activarea semnalului de ceas pentru portul A
	// PTA1 - receptie UART0
	// PTA2 - transmisie UART0
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	// Dezactivare receptor si emitator pentru realizarea configuratiilor
	UART0->C2 &= ~((UART0_C2_RE_MASK) | (UART0_C2_TE_MASK)); 
	
	// Setarea sursei de ceas pentru modulul UART la 48MHz
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(01);
	
	// Configurarea pinilor pentru emisie/receptie
	PORTA->PCR[1] = ~PORT_PCR_MUX_MASK;
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // RX
	PORTA->PCR[2] = ~PORT_PCR_MUX_MASK;
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // TX
	
	// Setarea baud rate-ului si a ratei de supraesantionare
	uint16_t temp = sys_clock;
	sbr = (uint16_t)((sys_clock)/(baud_rate * (osr)));
	UART0->BDH &= UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(osr - 1);
	
	// Setarea numarului de biti de date la 8 si fara bit de paritate
	UART0->C1 = UART0_C1_M(0) | UART0_C1_PE(0);
	
	// Transmisie incepand cu LSB
	UART0->S2 = UART0_S2_MSBF(0);
	
	UART0->C2 |= UART0_C2_TIE(0) | UART0_C2_TCIE(0);
	UART0->C2 |= UART0_C2_RIE_MASK;
	
	
	UART0->C2 |= ((UART_C2_RE_MASK) | (UART_C2_TE_MASK));
	
	// UART0_IRQn = Interrupt Request Number pentru UART0
	// Sterge intreruperile curente
	NVIC_ClearPendingIRQ(UART0_IRQn);
	// Activeaza intreruperile
	NVIC_EnableIRQ(UART0_IRQn);
	
	__enable_irq();
	
}


/* Se apeleaza cand se declanseaza o intrerupere la primirea de date */
void UART0_IRQHandler(void) {
	if(UART0->S1 & UART0_S1_RDRF_MASK) {
		uint8_t data = UART0->D;
		if (data == 'r')
			reverse_leds = !reverse_leds;
		else if (data == 'm')
			manual_servo = !manual_servo;
		else if (data == ',')
			angle_left = 1;
		else if (data == '.')
			angle_right = 1;
	}
}
