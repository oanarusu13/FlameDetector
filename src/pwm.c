#include "pwm.h"
#include "general.h"

volatile int angle_left;
volatile int angle_right;
extern volatile int flame_status;
volatile int8_t manual_servo;

// Function to convert an integer to a string
void itoa(int num, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, otherwise empty string is printed
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers for bases other than 10
    if (num < 0 && base != 10) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int remainder = num % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        num = num / base;
    }

    // Add negative sign for base 10
    if (isNegative && base == 10)
        str[i++] = '-';

    str[i] = '\0'; // Null-terminate the string

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

}

void TPM2_Init(){
	
	// Activarea semnalului de ceas pentru utilizarea LED-ului de culoare rosie
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Utilizarea alternativei de functionare pentru perifericul TMP
	// TMP2_CH0
	PORTB->PCR[OSCILLOSCOPE_PIN] |= PORT_PCR_MUX(3);
	
	// Selects the clock source for the TPM counter clock (MCGFLLCLK) - PG. 196
	// MCGFLLCLK Freq. - 48 MHz
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	// Activarea semnalului de ceas pentru modulul TPM
	SIM->SCGC6 |= SIM_SCGC6_TPM2(1);
	
	// Divizor de frecventa pentru ceasul de intrare
	// PWM CLK -> 48MHz / 128 = 48.000.000 / 128 [Hz] = 375.000 [Hz] = 375 kHz
	TPM2->SC |= TPM_SC_PS(7);
	
	// LPTPM counter operates in up counting mode. - PG. 553
	// Selects edge aligned PWM
	TPM2->SC |= TPM_SC_CPWMS(0);
	
	// LPTPM counter increments on every LPTPM counter clock
	TPM2->SC |= TPM_SC_CMOD(1);
	
	
	// LPTPM counter operates in up-down counting mode. - PG. 553
	// Selects center aligned PWM
	//TPM2->SC |= TPM_SC_CPWMS(1);
	
	
	// Edge-Aligned Pulse Width Modulation
	// TPM->SC[CPWMS] = 0
	
	// ===== Mode selection ====
	// Configured for EPWM
	// TPM->CnSC[MnSB] = 1
	// TPM->CnSC[MnSB] = 0
	
	// Regiter associated to the control of channel 0
	TPM2->CONTROLS[0].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
	
	
}

void Signal_Control(){
	static uint8_t duty_cycle = 0;

	// Manual Control
	if (angle_left){
		duty_cycle = 15;
		angle_left = 0;
	}
	else if (angle_right){
		duty_cycle = 6;
		angle_right = 0;
	}
	
	// Resetarea valorii numaratorului asociat LPTPM Counter
	TPM2->CNT = 0x0000;
	
	// Setarea perioadei semnalului PWM generat
  TPM2->MOD = 375 * 20;
	
	// Setarea duty cycle-ului asociat semnalului PWM generat
  TPM2->CONTROLS[0].CnV = 37 * (duty_cycle + 5);
	
	/*
	duty_cycle ++;
  if (duty_cycle > 20){
		duty_cycle = 0;
  }*/
	
	#ifdef DEBUG
		char str[16];
		itoa(duty_cycle, (char*)str, 10);
		UART0_Transmit_String(str);
		UART0_Transmit(0x0A);
		UART0_Transmit(0x0D);
	#endif

}
