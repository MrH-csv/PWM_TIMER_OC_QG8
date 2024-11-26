#include <hidef.h>
#include "derivative.h"


void PWM(unsigned int periodo, unsigned int ancho);
unsigned int adc_leer(unsigned char t);
void retardo(void);

unsigned char canal = 0x00;
unsigned int ADCresult=0X0000,ADCresult1=0x0000;

void main(void) {
	
	
  /*INICIALIZACION DE PUERTOS*/
  SOPT1 = 0x12;//Apagar wD

  /*INICIALIZACION ADC*/
  ADCCFG = 0x10;
  APCTL1_ADPC0 = 1;
  ADCSC1 = 0x00;
  ADCSC2 = 0x00;
 
  /*BUCLE INFINITO*/
  for(;;) {
	  
    ADCresult = adc_leer(canal)*8;
    if(ADCresult1!=ADCresult){
    	ADCresult1=ADCresult;
    	//TPMCNT = 0X0000;
    	TPMSC=0X00;
    }
    PWM(2083,ADCresult1);//2083 CUENTAS = 16.666ms = 60hz
    
   	//retardo();
   	
  } 
}


void PWM(unsigned int periodo, unsigned int ancho)
{
TPMMOD=periodo;
TPMC1V=ancho + 20;
TPMC1SC = 0x28; //ModoPWM, Alineado de subida
TPMSC = 0x0D; // Sin IRQ, BusClk=4Mhz, Preescaler=32
while(!TPMC1SC_CH1F);
TPMC1SC_CH1F=0; // Borra la bandera

}

unsigned int adc_leer(unsigned char t) {
    ADCSC1 = 0x00 | t;
    while (ADCSC1_COCO != 1);
    return ADCR;
}

void retardo(void) {
    SRTISC = 0x01;
    while(SRTISC_RTIF == 0);
    SRTISC_RTIACK = 1;
    SRTISC = 0x00;
}


