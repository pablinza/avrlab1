/*
 * File:   main.c
 * Author: Pablo Zarate 
 *
 * Created on July 31, 2025, 11:03 PM
 */
//#pragma config SUT_CKSEL=SUT_CKSEL_EXTXOSC_8MHZ_XX_16KCK_14CK_65MS, CKOUT=CLEAR, CKDIV8=CLEAR
//#pragma config BOOTRST=SET, BOOTSZ=BOOTSZ_256W_3F00, RSTDISBL=CLEAR
#include <xc.h>
volatile uint8_t tickms;
void taskLED(void);
void setupMCU(void);
void __interrupt(TIMER0_OVF_vect_num) t0_isr(void) //Interrupcion
{
    TCNT0 += 5; //Actualiza el contador
    tickms = 1; //Activa bandera
}
int main(void) //Programa principal
{
    setupMCU(); //Configura el MCU
    while(1)
    {
        if(tickms) //Cada 1ms
        {
            tickms = 0; //limpia bandera
            taskLED(); //Destella LED
        }
    }
    return 0;
}
void taskLED(void) //Ejecucion cada 1ms
{
    static uint16_t cnt = 0;
    if(cnt++ > 999)
    {
        cnt = 0;
        PORTB |= _BV(PB5); //Nivel alto en PB5
    }
    if(cnt == 100) PORTB &= ~_BV(PB5); //Nivel bajo en PB5
}
void setupMCU(void)
{
	DDRB |= _BV(PB5);   //PB5 salida led 13(UNO) 
	PORTB &= ~_BV(PB5); //PB5 en nivel bajo
	/* CONFIGURACION TIMER0 1MS Fosc=16MHz */
	TCCR0A = 0x00; //Modo normal
	TCCR0B = 0x03; //Ajuste Divisor CS=1:64 
	TCNT0 = 5; //TCNT0=255-(0.001*Fosc/64)=05
	TIMSK0 |= _BV(TOIE0);//Activa interrupcion del T0 
	SREG = _BV(SREG_I);  //Habilitador Global de interrupcion
}
