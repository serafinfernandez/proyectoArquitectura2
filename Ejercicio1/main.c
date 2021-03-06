#include <msp430.h>

#define SW1  BIT0	//P2.0
#define SW2  BIT1	//P2.1
#define LED1 BIT5	//P4.5
#define LED2 BIT6	//P4.6


void prendoLed1();
void prendoLed2();
void apagoLed1();
void apagoLed2();
void prendoTodosLosLeds();
void apagoTodosLosLeds();

void main(void) {

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	P4DIR |= (LED1 + LED2);//
	P2DIR &= ~(SW1 + SW2);
	P2REN |= (SW1 + SW2);
	P2OUT |= (SW1 + SW2);

	int primeroSW1 = 0;
	int primeroSW2 = 0;
	int quedanPrendidos = 0;

	while(1){

		if(!(P2IN & SW1)){
			if(primeroSW2 == 0){
				primeroSW1 = 1;
				prendoLed1();
			}else{
				quedanPrendidos = 0;
				apagoTodosLosLeds();
			}
		}else{
			if(quedanPrendidos == 0)
				apagoLed1();
			primeroSW1 = 0;
		}

		if(!(P2IN & SW2)){
			if(primeroSW1 == 0){
				primeroSW2 = 1;
				prendoLed2();
			}else{
				prendoTodosLosLeds();
				quedanPrendidos = 1;
			}
		}else{
			if(quedanPrendidos == 0)
				apagoLed2();
			primeroSW2 = 0;
		}

	}
}



void prendoLed1(){
	P4OUT |= LED1;
}
void prendoLed2(){
	P4OUT |= LED2;
}

void apagoLed1(){
	P4OUT &= ~LED1;
}
void apagoLed2(){
	P4OUT &= ~LED2;
}


void prendoTodosLosLeds(){
	prendoLed1();
	prendoLed2();
}
void apagoTodosLosLeds(){
	apagoLed1();
	apagoLed2();
}

