/*
 * File:   main.c
 * Author: Pablo Zarate
 *
 * Created on August 2, 2025, 11:32 PM
 */

#include <xc.h>
#define LED1pin 0x20 //Led integrado PB.bit5 (PB5)
#define LED2pin 0x04 //Led adicional PD.bit2 (PD2)
volatile uint8_t tickms;
void taskLED1(void);
void taskLED2(void);
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
            taskLED1(); //Destella LED1
            taskLED2(); //Destella LED2
        }
    }
    return 0;
}
void taskLED1(void) //Destello de LED1 1Hz al 20%
{
    static uint16_t cnt = 0;
    if(cnt++ > 999)
    {
        cnt = 0;
        PORTB |= LED1pin; //Activa LED
    }
    if(cnt == 200) PORTB &= ~LED1pin; //Apaga LED
}
void taskLED2(void) //Destello de LED2 5Hz al 50%
{
    static uint16_t cnt = 0;
    if(cnt++ > 199) 
    {
        cnt = 0;
        PORTD |= LED2pin; //Activa LED
    }
    if(cnt == 100) PORTD &= ~LED2pin; //Apaga LED
}
void setupMCU(void)
{
  DIDR0 = 0x3F;     //Desactiva canales ADC5:0
  DDRB |= _BV(PB5); //PB5 salida LED1 13(UNO) 
  DDRD |= _BV(PD2); //PD2 salida LED2
  PORTB &= ~LED1pin;//LED1 apagado
  PORTD &= ~LED2pin;//LED2 apagado
  /* CONFIGURACION TIMER0 1MS Fosc=16MHz */
  TCCR0A = 0x00; //Modo normal
  TCCR0B = 0x03; //Ajuste Divisor CS=1:64 
  TCNT0 = 5; //TCNT0=255-(0.001*Fosc/64)=05
  TIMSK0 |= _BV(TOIE0); //Activa interrupcion del T0
  SREG = _BV(SREG_I);   //Habilitador Global de interrupcion  
}