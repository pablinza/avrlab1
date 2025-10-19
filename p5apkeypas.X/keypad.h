 /*File:   keypad.h / keypad.c
 * Author: Pablo Zarate email:pablinza@me.com
 * Comments: Libreria de teclado Matricial para AVR
 * Port map for   Inputs --- Outputs
 * Keypad array [R1R2R3R4]  [C1C2C3C4]
 * Se requiere Pull-up en las entradas filas (ROWS)
 * Revision history: 25.8 */
#ifndef XC_KEYPAD_H
#define	XC_KEYPAD_H
#include <xc.h>
/* USER PORT DEFINITION */
#define KB_CPORT PORTB //Columns output port
#define KB_CTRIS DDRB  //Columns port mode configuration
#define KB_CMASK 0x0F  //Mask pinout 0x0F=Lower pins, 0xF0=Upper pins
#define KB_RPORT PIND  //Row input port
#define KB_RTRIS DDRD  //Row port mode config
#define KB_RPUD  PORTD //Port pull-ups enable register
#define KB_RMASK 0xF0  //Set bit mask pinout 0x0F=Lower 0xF0=Upper pins 
/* END USER PORT DEFINITIOS */
void KBSetup(void);
uint8_t KBScan(void); //Retorna 1 si existe dato 
uint8_t KBGetValue(uint8_t n); //Recupera valor Decimal
uint8_t KBGetChar(uint8_t n); //Recupera valor ASCII
#endif
