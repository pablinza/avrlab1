/* File:   lcd.h / lcd.c
 * Author: Pablo Zarate email:pablinza@me.com
 * Comments: LCD single and multi line library for AVR
 * Designed for 4-bit mode operation pin map data D7:D4
 * Data Port can be Upper or Lower bits mode for LCD [D7:D4] 
 * LCD Pins RW RS RE will defined by user 
 * If LCD_RWENABLE=0 RW pin of will be to grounded 
 * F_CPU <VALUE> definition is required for timing 
 * Revision history: 25.08 */
#ifndef XC_LCD_H
#define	XC_LCD_H
#include <xc.h>
/* USER PORT DEFINITION */
#define LCD_DATAPORT PORTC  //Data port pins D7:D4
#define LCD_TRISPORT DDRC   //Data port mode config
#define LCD_RSPORT PORTC    //RS pin port 
#define LCD_RSTRIS DDRC     //RS pin mode config
#define LCD_ENPORT PORTC    //EN pin port
#define LCD_ENTRIS DDRC     //EN pin mode config
#define LCD_MASK  0x0F      //Mask port pins 0xF0=Upper[7:4] 0x0F=Lower[3:0]
#define LCD_RSPIN 0x10      //define pin position in RSPORT
#define LCD_ENPIN 0x20      //define pin position in ENPORT
#define LCD_RWENABLE 0      //Write mode enable 1, busy flag read from LCD
#define LCD_RWPORT PINC     //RW pin port used only if LCD_RWENABLE defined
#define LCD_RWTRIS DDRC     //RW pin mode used only if LCD_RWENABLE defined
#define LCD_RWPIN 0x40      //define pin position in RWPORT used only if LCD_RWENABLE
#ifndef F_CPU
#define F_CPU 16000000UL
#include <util/delay.h>
#endif 
/* END USER PORT DEFINITIOS */
#define LCD_delay_tcy() _delay_us(2) //EN clock required 1.2us
#define LCD_delay_cmd() _delay_us(40)//CMD Wait required 37us
#define LCD_delay_por() _delay_us(1600)//CLEAR & HOME CMD Wait 1.52ms 
/*INPUTSET COMMAND MAY BE AND*/
#define LCD_INPDEC      0x05   
#define LCD_INPSHIFTOFF 0x06
/*DISPLAY COMMAND MAY BE AND*/
#define LCD_DOFF        0x0B
#define LCD_DCURSOROFF  0x0D
#define LCD_DBLINKOFF   0x0E
/*SHIFT COMMAND MAY BE AND*/
#define LCD_SHIFTCURSOR 0x17 
#define LCD_SHIFTLEFT   0x1B
/* FUNCTIONSET COMMAND MAY BE AND*/ 
#define LCD_FSETLINE1   0x37
#define LCD_FSETFONT57  0x3B
/*LIST OF COOMANDS*/
#define LCD_CLEAR       0x01 //Clear Screen Wait 1.5ms
#define LCD_HOME        0x02 //Return Wait 1.5m
#define LCD_INPUTSET    0x07 //Default ID(INCREMENT) S(SHIFT)
#define LCD_DISPLAY     0x0F //Default D(DON) C(CURSOR) B(BLINK)
#define LCD_SHIFT       0x1F //Default SC(DISPLAY SHIFT) RL(RIGHT)
#define LCD_FUNCTIONSET 0x2F //Default DL(DATA4BIT) N(2ROWS) F(7x10)
void LCDWriteChar(char data);
void LCDWriteNum(uint16_t num, uint8_t padn);
void LCDWriteCMD(uint8_t cmd);
void LCDGotoXY(uint8_t col, uint8_t row);
void LCDWriteMsg(char *str);
void LCDWriteMsgROM(const char *str);
void LCDSetup(void);
#endif
