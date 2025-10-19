/*
 * File:   main.c
 * Author: pablo
 *
 * Created on August 7, 2025, 12:10 AM
 */
#include <xc.h>
#include <string.h>
#include "keypad.h"
#include "lcd.h"
#define LEDpin 0x20 //Led de la tarjeta PB.bit5
volatile uint8_t tickms;
char pass[5]; //Contrasena de cuatro digitos
void taskAPP(void);
void taskLED(void);
void setupMCU(void);
void __interrupt(TIMER0_OVF_vect_num) t0_isr(void) //Interrupcion T0
{
    TCNT0 += 5; //Actualiza el contador
    tickms = 1; //Activa bandera
}

int main(void) 
{
    setupMCU(); //Configura el MCU
    KBSetup();  //Configura el Teclado
    LCDSetup(); //Configura el modulo LCD
    while (1) 
    {
        if(tickms) //Cada 1ms
        {
            tickms = 0;//limpia bandera
            taskLED(); //Destella LED
            taskAPP(); //Aplicacion teclado y lcd
        }
    }
}
void taskAPP(void) //Aplicacion Lectura teclado y muestra LCD
{
    static uint8_t keycnt, state = 0, nerr = 0;
    static uint16_t cnt = 0;
    uint8_t res, value;
    switch(state)
    {
        case 0: //Muestra el mensaje inicial
            LCDGotoXY(0,0);
            LCDWriteMsg(" PASSWORD");
            LCDGotoXY(0,1);
            LCDWriteMsg("  [    ]");
            LCDGotoXY(3,1);
            cnt = 0; 
            keycnt = 0; //Inicia contador de digitos
            state = 1;
            break;
        case 1: //Lectura del teclado
            res = KBScan(); //Lectura del Teclado
            if(res) //Si hay dato valido
            {
                if(cnt++ > 29) //Espera confirmacion
                {
                    value = KBGetChar(res);//Recupera valor char
                    LCDWriteChar(value); //Escribe valor
                    pass[keycnt] = value; 
                    cnt = 0;
                    state = 2;
                }
            } else cnt = 0;
            break;
        case 2: //Lectura de cuatro digitos    
            keycnt++;
            if(keycnt > 3) //Espera el cuarto digito
            {
                pass[keycnt] = 0; //Clave lista
                state = 4;
            } 
            else state = 3;
            break;
        case 3: //Espera liberacion de teclado
            if(cnt > 199)
            {
                if(KBScan()==0) state = 1; //Si teclado es liberado    
            }
            else cnt++;
            break;
        case 4: //Compara password y valida
            LCDGotoXY(0,1);
            res = (uint8_t) strcmp("3022", pass);
            if(res == 0) 
            {
                LCDWriteMsg(" Autorizado");
                state = 6; //Acceso autorizado
            }
            else 
            {
                if(++nerr >= 3) //Numero de intentos permitidos
                {
                    LCDWriteMsg(" Bloqueado");
                    state = 7; //Usuario bloqueado
                }
                else
                {
                    LCDWriteMsg("  Error ");
                    state = 5;
                }
            }
            break;
        case 5: //Genera retardo 
            if(cnt++ > 1999)
            {
                LCDWriteCMD(LCD_CLEAR);
                state = 0;
            }
            break;
        case 6: //Acceso autorizado
            NOP();
            break;
        case 7://Usuario Bloqueado
            NOP();
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
    if(cnt == 200) PORTB &= ~LEDpin; //Nivel bajo en PB5
}
void setupMCU(void)
{
    DDRB |= _BV(PB5);   //PB5 salida led 13(UNO) 
    PORTB &= ~LEDpin;  //PB5 en nivel bajo
    /* CONFIGURACION TIMER0 1MS Fosc=16MHz */
    TCCR0A = 0x00; //Modo normal
	TCCR0B = 0x03; //Ajuste Divisor CS=1:64 
	TCNT0 = 5; //TCNT0=255-(0.001*Fosc/64)=05
	TIMSK0 |= _BV(TOIE0);//Activa interrupcion del T0 
    SREG = _BV(SREG_I);  //Habilitador Global de interrupcion
}
