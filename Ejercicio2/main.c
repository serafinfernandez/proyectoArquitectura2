#include <msp430.h> 
#include "./TI_MSP_UART.h"
#include <stdio.h>
#include <string.h>

#define MSP430_UART_SMCLK_FREQ SMCLK_1MHZ
#define MSP430_UART_BAUD_RATE  BAUD_RATE_9600BPS

char buffer[7];
char debug[20];
char datoLeido;
int cantidadDatosLeidos;

void push(char a);
void debugger(char *debug);
void parser();
void realizarOperacion(int n1, char operacion, int n2);

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /* configure internal digitally control led oscillator */
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;
	
    Init_UART();

	__enable_interrupt();

	while(1);
}

#pragma vector=UART_ISR_RX_VECTOR
__interrupt void ISR_UartRx(void){
    if(UART_IFG & UART_UCAxRXIFG){
      datoLeido = UART_UCAxRXBUF;
	  push(datoLeido);
    }
}

void push(char a){
	UART_Tx_char(a);
	if(a == 13){
		debugger("\n\r");
		parser();
	}else{
		buffer[cantidadDatosLeidos] = a;
		cantidadDatosLeidos++;
	}
}

void parser(){

	int n1 = (int)(buffer[0]-48)*100 + (int)(buffer[1]-48)*10 + (int)(buffer[2]-48);
	char operacion = buffer[3];
	int n2 = (int)(buffer[4]-48)*100 + (int)(buffer[5]-48)*10 + (int)(buffer[6]-48);
	cantidadDatosLeidos = 0;
	realizarOperacion(n1, operacion, n2);
}

void realizarOperacion(int n1, char operacion, int n2){
	int resultado;

	switch(operacion){
		case 43: //suma
			resultado = n1 + n2;
			break;
		case 45: //resta
			resultado = n1 - n2;
			break;
	}

	sprintf(debug, "%d\n\r", resultado);
	debugger(debug);

}

void debugger(char *debug){
	UART_Tx_string(debug, 0);
}
