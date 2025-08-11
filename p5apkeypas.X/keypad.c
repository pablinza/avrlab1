 /*File:   keypad.h / keypad.c
 * Author: Pablo Zarate email:pablinza@me.com
 * Comments: Matrix 4x4 Keypad library for AVR
 * Port map for   Inputs --- Outputs
 * Keypad array [R1R2R3R4]  [C1C2C3C4]
 * Pull-up required for every inputs rows
 * Revision history: 25.08 */
#include <xc.h>
#include "keypad.h"
/* void KBSetup(void)
 * Setup Port ROW and COL
 * Precon: KB_TRIS and KB_PORT will be defined
 * Params: none
 * Return: ROW are inputs, COL are outputs */
void KBSetup(void)
{
	KB_CTRIS |= KB_CMASK; //Outputs
    KB_RTRIS &= ~KB_RMASK;//Inputs
#ifdef KB_RPUD
    KB_RPUD |= KB_RMASK; //Enable pin pullups
#endif
    KB_CPORT |= KB_CMASK; //Columns in High level Walking to zeros
}
/* uint8_t KBScan(uint8_t n)
 * Scan entire ROW and COL cicle
 * Precon: none
 * Params: none
 * Return: Raw code format R1R2R3R4C1C2C3C4 
 *         if 0 = Not key pressed */
uint8_t KBScan(void)
{
    uint8_t row, col, i = 4;
#if KB_CMASK==0x0F 
    col = 0xFE; //Check if pins is LOWER mode
#else
    col = 0xEF; //Check if pins is UPPER mode
#endif
    while(i--)
    {
        KB_CPORT |= KB_CMASK; //Columns in High level Walking to zeros
        KB_CPORT &= col;
        NOP();
        if((KB_RPORT & KB_RMASK)!= KB_RMASK)
        {
            row = (KB_RPORT & KB_RMASK);
            if(KB_RMASK == 0x0F) row <<= 4;
            if(KB_CMASK == 0xF0) col >>= 4;
            else col &= KB_CMASK;
            return (row|col); //Return code
        }
        else col = (col<<1)+1;
    }
    KB_CPORT |= KB_CMASK; //Columns in High level Walking to zeros
    return 0;
}
/* uint8_t KBGetChar(uint8_t n)
 * Convert raw scan code to char ascii
 * Precon: KBScan called before
 * Params: byte raw code 
 * Return: ascii char code*/
uint8_t KBGetChar(uint8_t n)
{
	switch(n)
	{
		case 0b11101110: return 'D';
		case 0b11101101: return '#';
		case 0b11101011: return '0';
		case 0b11100111: return '*';
		case 0b11011110: return 'C';
		case 0b11011101: return '9';
		case 0b11011011: return '8';
		case 0b11010111: return '7';
		case 0b10111110: return 'B';
		case 0b10111101: return '6';
		case 0b10111011: return '5';
		case 0b10110111: return '4';
		case 0b01111110: return 'A';
		case 0b01111101: return '3';
		case 0b01111011: return '2';
		case 0b01110111: return '1';
		default: return '?';
	}
}
/* uint8_t KBGetValue(uint8_t n)
 * Convert raw scan code to decimal value
 * Precon: KBScan called before
 * Params: byte raw code 
 * Return: decimal char code*/
uint8_t KBGetValue(uint8_t n)
{
	switch(n)
	{
		case 0b11101110: return 13; //Return
		case 0b11101101: return 15;
		case 0b11101011: return 0;
		case 0b11100111: return 14;
		case 0b11011110: return 12;
		case 0b11011101: return 9;
		case 0b11011011: return 8;
		case 0b11010111: return 7;
		case 0b10111110: return 11;
		case 0b10111101: return 6;
		case 0b10111011: return 5;
		case 0b10110111: return 4;
		case 0b01111110: return 10; //Line Feed
		case 0b01111101: return 3;
		case 0b01111011: return 2;
		case 0b01110111: return 1;
		default: return 0xFF;
	}
}

