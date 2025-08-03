/*
 * File:   main.c
 * Author: Pablo Zarate 
 * 
 * Created on August 1, 2025, 8:01 PM
 */
#include <xc.h>
#include <stdio.h>
#define LED1pin 0x20 //Led de la tarjeta PB.bit5
#define SW1pin  0x01 //Entrada pulsador PB.bit0
volatile uint8_t tickms;
uint8_t sw1sta; //Variable de estado SW1
uint8_t an0pos = 0, an1pos = 0;
uint8_t adcOK; //Bandera que indica datos del ADC listos
void taskSW1(void); //Procedimiento lectura SW1
void taskADC(void); //Procedimiento Lectura ADC
void taskLED(void);
void setupMCU(void);
int putch(char byte, FILE *stream);
FILE mystdout = FDEV_SETUP_STREAM(putch, NULL, _FDEV_SETUP_WRITE);

void __interrupt(TIMER0_OVF_vect_num) t0_isr(void) //Interrupcion T0
{
    TCNT0 += 5; //Actualiza el contador
    tickms = 1; //Activa bandera
}
int main(void) //Programa principal
{
    stdout = &mystdout; //Asignacion de la salida estandar
    setupMCU(); //Configura el MCU
    while(1)
    {
        if(tickms) //Cada 1ms
        {
            tickms = 0;//limpia bandera
            taskLED(); //Destella LED
            taskSW1(); //Lectura SW1
            taskADC(); //Lectura ADC
            if(adcOK) //Envia mensaje listo por UART
            {
                adcOK = 0;
                printf("R:%u,%u,%u\r\n",an0pos, an1pos, sw1sta);
            }
        }
    }
    return 0;
}

void taskADC(void) //Procedimiento lectura canales ADC
{
    static uint16_t cnt = 0;
    static uint8_t state = 0;
    uint16_t res;
    cnt++;
    switch(state)
    {
        case 0: //Prepara modulo ADC
            ADCSRA |= _BV(ADEN); //Activa modulo ADC
            ADMUX &= ~_BV(MUX0); //Selecciona canal ADC0 
            state++;
            break;
        case 1: //Inicia la conversion ADC Canal ADC0
            ADCSRA |= _BV(ADSC); //Inicia la conversion
            state++;
            break;
        case 2: //Lectura y ajuste de canal ADC
            {
                res = ADCL; //Carga los 2-bit de menos peso
                res |= (uint16_t) (ADCH << 8); //Carga el restp de bots
                res >>= 6; //Alinea los ADRESH:ADRESL    
                an0pos = (uint8_t) (res / 10u); //Divide entre 10 el valor ADC
                ADMUX |= _BV(MUX0); //Selecciona canal ADC1
                state++;
            }
            break;
        case 3: //Inicia la conversion ADC Canal ADC1
            ADCSRA |= _BV(ADSC); //Inicia la conversion
            state++;
            break;
        case 4: //Lectura y ajuste de canal ADC
             {
                res = ADCL;
                res |= (uint16_t) (ADCH << 8);
                res >>= 6;   
                an1pos = (uint8_t) (res / 10u);
                ADCSRA &= ~_BV(ADEN); //Apaga el modulo ADC
                adcOK = 1;
                state++;
            }
            break;
        case 5: //Espera de nuevo ciclo ADC
            if(cnt++ > 499) //Espera en milisegundos
            {
                cnt = 0;
                state = 0;
            }
            break; //Espera 
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
void taskLED(void) //Ejecucion cada 1ms
{
    static uint16_t cnt = 0;
    if(cnt++ > 999)
    {
        cnt = 0;
        PORTB |= LED1pin; //Nivel alto en PB5
    }
    if(cnt == 200) PORTB &= ~LED1pin; //Nivel bajo en PB5
}
void setupMCU(void)
{
    DIDR0 = 0x3F;       //Desactiva canales ADC5:0
    DDRB |= _BV(PB5);   //PB5 salida led 13(UNO) 
    PORTB &= ~LED1pin;  //PB5 en nivel bajo
    PORTB|= _BV(PB0);   //Activa pull-up del PB0 
    /* CONFIGURACION ADC ADC0 y ADC1 Fosc=16MHz */
    ADMUX = _BV(REFS0); //Referencia AVCC, VRef con capacitor a GND
    ADMUX |= _BV(ADLAR); //Justificacion a la izquierda ADCH:ADCL
	ADCSRA = 0x07; //Pre 1:128 16M/128 = 125Khz debe ser entre 50-200Khz
	ADCSRA |= _BV(ADEN); //Habilita el modulo ADC
	DIDR0 &= ~_BV(ADC0D); //Activa canal ADC0
	DIDR0 &= ~_BV(ADC1D); //Activa canal ADC1
    /*CONFIGURACION UART 9600 Fosc=16MHz*/
	UBRR0H = 0; //[Fosc/(16*BAUD)]-1
	UBRR0L = 103;
	UCSR0B |= _BV(TXEN0); //Habilita el Transmisor
	UCSR0B |= _BV(RXEN0); //Habilita el Receptor
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); //Dato de 8-bit
    /* CONFIGURACION TIMER0 1MS Fosc=16MHz */
    TCCR0A = 0x00; //Modo normal
	TCCR0B = 0x03; //Ajuste Divisor CS=1:64 
	TCNT0 = 5; //TCNT0=255-(0.001*Fosc/64)=05
	TIMSK0 |= _BV(TOIE0);//Activa interrupcion del T0 
    SREG = _BV(SREG_I);  //Habilitador Global de interrupcion
}
int putch(char byte, FILE *stream) //funcion de print-byte UART
{
  while((UCSR0A & _BV(UDRE0))==0); //Wait for buffer empty
  UDR0 = byte;
  return 0;
}
