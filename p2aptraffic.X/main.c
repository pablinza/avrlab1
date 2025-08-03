/*
 * File:   main.c
 * Author: Pablo Zarate
 *
 * Created on August 2, 2025, 11:51 PM
 */
#include <xc.h>
#define LEDpin 0x20 //Led integrado PB.bit5 (PB5)
#define SW1pin 0x01 //Entrada pulsador PB.bit0 (PB0)
#define LIGHTS PORTD //Puerto de luces <7:R2><6:A2><5:V2><4:R1><3:A1><2:V1><1:x><0:x>
volatile uint8_t tickms;
uint8_t sw1sta; //Variable de estado SW1
void taskAPP(void); //Procedimiento modo Normal
void taskERR(void); //Procedimiento con Alarma
void taskSW1(void); //Procedimiento lectura SW1
void taskLED(void); //Destello LED 
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
            taskSW1(); //Lectura SW1
            if(sw1sta) taskAPP(); //Operacion normal
            else taskERR(); //Operacion con alarma
        }
    }
    return 0;
}
void taskAPP(void)
{
    static uint16_t cnt = 0;
    static uint8_t state = 0;
    cnt++; //Incrementa contador milisegundos
    switch(state)
    {
        case 0: //Primer estado de luces
            LIGHTS = 0b10000100;
            if(cnt > 9999) //Tiempo en ms
            {
                cnt = 0;
                state++; //Siguiente estado
            } break;
        case 1: //Segundo estado de luces
            LIGHTS = 0b10001000;
            if(cnt > 2999)
            {
                cnt = 0;
                state++; //Siguiente estado
            } break;
        case 2: //Tercer estado de luces
            LIGHTS = 0b00110000;
            if(cnt > 9999)
            {
                cnt = 0;
                state++; //Siguiente estado
            } break;
        case 3: //Cuarto estado de luces
            LIGHTS = 0b01010000;
            if(cnt > 2999)
            {
                cnt = 0;
                state = 0; //Reinicia maquina secuencial
            } break;
    }
}
void taskERR(void)
{
    static uint16_t cnt = 0;
    static uint8_t state = 0;
    cnt++;
    switch(state)
    {
        case 0: //Estado luces apagadas
            LIGHTS = 0b00000000;
            if(cnt > 499)
            {
                cnt = 0;
                state++;
            } break;
        case 1: //Estado luces amarillas
            LIGHTS = 0b01001000;
            if(cnt > 499)
            {
                cnt = 0;
                state = 0;
            } break;
    }
}
void taskSW1(void)
{
    static uint8_t cnt = 0;
    if(sw1sta != (PINB & SW1pin)) //Si estado actual difiere 
    {
        if(cnt++ > 199) sw1sta = (PINB & SW1pin); //200ms
    } else cnt = 0;
}
void taskLED(void) //Destello de LED1 1Hz al 20%
{
    static uint16_t cnt = 0;
    if(cnt++ > 999)
    {
        cnt = 0;
        PORTB |= LEDpin; //Activa LED
    }
    if(cnt == 200) PORTB &= ~LEDpin; //Apaga LED
}
void setupMCU(void)
{
  DIDR0 = 0x3F;     //Desactiva canales ADC5:0
  DDRB |= _BV(PB5); //PB5 salida LED1 13(UNO) 
  PORTB |= _BV(PB0); //Activa pull-up PB0 pulsador SW1
  DDRD |= 0xFC; //salida luces PD7:PD2
  PORTB &= ~LEDpin;//LED1 apagado
  PORTD = 0b01001000; //Luces del puerto solo amarillos 
  /* CONFIGURACION TIMER0 1MS Fosc=16MHz */
  TCCR0A = 0x00; //Modo normal
  TCCR0B = 0x03; //Ajuste Divisor CS=1:64 
  TCNT0 = 5; //TCNT0=255-(0.001*Fosc/64)=05
  TIMSK0 |= _BV(TOIE0); //Activa interrupcion del T0
  SREG = _BV(SREG_I);   //Habilitador Global de interrupcion  
}