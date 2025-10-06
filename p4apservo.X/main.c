/*
 * File:   main.c
 * Author: pablo
 *
 * Created on August 4, 2025, 10:17 PM
 */
#include <xc.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "mservo.h"
MSV_t MSV;
#define LEDpin 0x20 //Led interno PB.bit5 de la tarjeta
#define ECHOpin 0x01 //Entrada del sensor PB.bit0 pin ECHO
#define TRIGpin 0x02 //Salida al sensor PB.bit1 pin TRIGGER
volatile uint8_t tickms;
uint8_t distcnt = 0;
volatile uint8_t distOK;
int putch(char byte, FILE *stream);
FILE mystdout = FDEV_SETUP_STREAM(putch, NULL, _FDEV_SETUP_WRITE);
void taskLED(void);
void setupMCU(void);
void taskSR04(void); //Procedimiento lectura SR04
void __interrupt(TIMER0_OVF_vect_num) t0_isr(void) //Interrupcion T0
{
    TCNT0 += 5; //Actualiza el contador
    tickms = 1; //Activa bandera
}
void __interrupt(TIMER2_COMPA_vect_num) t2_isr(void) //Interrupcion T1
{
    OCR2A = MSERVOHandler();
}
int main(void) //Programa principal
{
    stdout = &mystdout; //Asignacion de la salida estandar
    setupMCU(); //Configura el MCU
    MSERVOSetup();
    MSV.sv1pos = 60; //Posicion central
    while(1)
    {
        if(tickms) //Cada 1ms
        {
            tickms = 0; //limpia bandera
            taskLED(); //Destella LED
            taskSR04(); //Lectura SR04
            if(distOK)
            {
                distOK = 0;
                printf("R:%u\r\n", distcnt);
                if(distcnt < 3) MSV.sv1pos = 25; //Levanta
                else MSV.sv1pos = 85; //Cierra
            }
        }
    }
    return 0;
}
void taskSR04(void) //Lectura de sensor pasos en ms
{
    static uint16_t cnt = 0;
    static uint8_t state = 0;
    switch(state)
    {
        case 0: //Disparo TRIG 10us
            PORTB |= TRIGpin;
            _delay_us(10); 
            PORTB &= ~TRIGpin;
            state++;
            break;
        case 1: //Espera pulso ECHO  
            cnt = 0; //Luego de 1ms el pulso ECO ya esta en nivel alto
            state++;
            break;
        case 2: //Mide fin del pulso ECHO
            if(cnt++ > 35) state = 3;//Si el pulso es mayor a 35ms
            if((PINB & ECHOpin) == 0)
            {
                distcnt = (uint8_t) cnt; //valor en pasos ms
                distOK = 1;
                state = 3;
            } break;
        case 3: //Espera nuevo ciclo
            if(cnt++ > 999) state = 0;
            break;
    }
}
void taskLED(void) //Ejecucion cada 1ms
{
    static uint16_t cnt = 0;
    if(cnt++ > 999)
    {
        cnt = 0;
        PORTB |= LEDpin; //Nivel alto en PB5
    }
    if(cnt == 500) PORTB &= ~LEDpin; //Nivel bajo en PB5
}
void setupMCU(void)
{
    DDRB |= LEDpin;   //Salida led 13(UNO) 
    PORTB &= ~LEDpin; //Led en nivel bajo
    DDRB |= TRIGpin;  //Salida pin TRIGGER del sensor
    PORTB |= ECHOpin; //Pull-up entrada ECHO
    /*CONFIGURACION UART 9600 Fosc=16MHz*/
    UBRR0H = 0; //[Fosc/(16*BAUD)]-1
	UBRR0L = 103;
	UCSR0B |= _BV(TXEN0); //Habilita el Transmisor
	//UCSR0B |= _BV(RXEN0); //Habilita el Receptor
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); //Dato de 8-bit
    /* CONFIGURACION TIMER0 1MS Fosc=16MHz */
    TCCR0A = 0x00; //Modo normal
    TCCR0B = 0x03; //Ajuste Divisor CS=1:64 
    TCNT0 = 5; //TCNT0=255-(0.001*Fosc/64)=05
    TIMSK0 |= _BV(TOIE0); //Activa interrupcion del T0
    /* CONFIGURACION TIMER2 CON INCR 8us A 16MHz */
    TCCR2A = 0x02; //Sin OC2A/OC2B, Modo CTC 
    TCCR2B = 0x05;  //Ajuste Divisor CS=1:128
    OCR2A = 125; //1ms = 8uS * 125
    TIMSK2 |= _BV(OCIE2A); //Activa interrupcion del T0  
    SREG = _BV(SREG_I);   //Habilitador Global de interrupcion  
}
int putch(char byte, FILE *stream) //funcion de print-byte UART
{
  while((UCSR0A & _BV(UDRE0))==0); //Wait for buffer empty
  UDR0 = byte;
  return 0;
}