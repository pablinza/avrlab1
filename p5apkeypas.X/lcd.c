/* File:   lcd.h / lcd.c
 * Author: Pablo Zarate email:pablinza@me.com
 * Comments: Libreria de pantalla LCD para AVR
 * Designed for 4-bit mode operation pin map data D7:D4
 * Data Port can be Upper or Lower bits mode for LCD [D7:D4] 
 * LCD Pins RW RS RE will defined by user 
 * If LCD_RWENABLE=0 RW pin of will be to grounded 
 * F_CPU <VALUE> definition is required for timing 
 * Revision history: 25.08 */
#include <xc.h>
#include "lcd.h"
/* void LCDWriteChar(char data)
 * Write a char in actual display cursor
 * Precon: none
 * Params: char value in ASCII
 * Return: none */
void LCDWriteChar(char data)
{
    LCD_RSPORT |= LCD_RSPIN; //Set RS=1 Data mode
    LCD_TRISPORT |= LCD_MASK; //Set data port as output 
    LCD_DATAPORT &= ~LCD_MASK;//Data pins in low level
#if LCD_MASK==0x0F 
    LCD_DATAPORT |= (data >> 4); //Send 4-bit MSB of CMD to Lower pins
#else
    LCD_DATAPORT |= (data & LCD_MASK); //Send 4-bit MSB of CMD to Upper pins
#endif
    LCD_ENPORT |= LCD_ENPIN; //Set pin to high level 
    LCD_delay_tcy(); 
    LCD_ENPORT &= ~LCD_ENPIN; //Set pin to low level
    LCD_DATAPORT &= ~LCD_MASK;//Data pins in low level
    
#if LCD_MASK==0x0F 
    LCD_DATAPORT |= (data & LCD_MASK); //Send 4-bit LSB of CMD to Lower pins
#else
    LCD_DATAPORT |= (data << 4); //Send 4-bit LSB of CMD to Upper pins
#endif
    LCD_ENPORT |= LCD_ENPIN; //Set pin to high level 
    LCD_delay_tcy(); 
    LCD_ENPORT &= ~LCD_ENPIN; //Set pin to low level
    LCD_delay_cmd();
    LCD_TRISPORT &= ~LCD_MASK;// Make port to input
}
/* void LCDWriteCMD(uint8_t data)
 * Write an LCD Command
 * Precon: none
 * Params: LCD command definitions
 * Return: none */
void LCDWriteNum(uint16_t num, uint8_t padn) //00000-65535
{
    uint8_t digs[5] = {0,0,0,0,0};
    uint8_t i, res;
    if((num > 9999) && (padn > 4))
    {   
        digs[0] = (uint8_t) num / 10000;
        num = num % 10000;
    }
    if((num > 999) && (padn > 3))
    {   
        digs[1] = (uint8_t) num / 1000;
        num = num % 1000;
    }
    if((num > 99) && (padn > 2))
    {   
        digs[2] = (uint8_t) num / 100;
        num = num % 100;
    }
    if((num > 9) && (padn > 1))
    {   
        digs[3] = (uint8_t) num / 10;
        num = num % 10;
    }
    if(num && padn) digs[4] = (uint8_t) num;
    if(padn)
    {
        i = 5-padn;
        while(i < 5)
        {
            res = digs[i] + 0x30;
            LCDWriteChar(res);
            i++;
        }
    }
}

void LCDWriteCMD(uint8_t cmd)
{
    LCD_RSPORT &=~LCD_RSPIN; //Set RS=0 Command mode
    LCD_TRISPORT |= LCD_MASK; //Set data port as output 
    LCD_DATAPORT &= ~LCD_MASK;//Data pins in low level
#if LCD_MASK==0x0F 
    LCD_DATAPORT |= (cmd >> 4); //Send 4-bit MSB of CMD to Lower pins
#else
    LCD_DATAPORT |= (cmd & LCD_MASK); //Send 4-bit MSB of CMD to Upper pins
#endif
    LCD_ENPORT |= LCD_ENPIN; //Set pin to high level 
    LCD_delay_tcy(); 
    LCD_ENPORT &= ~LCD_ENPIN; //Set pin to low level
    LCD_DATAPORT &= ~LCD_MASK;//Data pins in low level
    
#if LCD_MASK==0x0F 
    LCD_DATAPORT |= (cmd & LCD_MASK); //Send 4-bit LSB of CMD to Lower pins
#else
    LCD_DATAPORT |= (cmd << 4); //Send 4-bit LSB of CMD to Upper pins
#endif
    LCD_ENPORT |= LCD_ENPIN; //Set pin to high level 
    LCD_delay_tcy(); 
    LCD_ENPORT &= ~LCD_ENPIN; //Set pin to low level
    if(cmd > 0x02) LCD_delay_cmd();
    else LCD_delay_por(); //For CLEAR or HOME Command
    LCD_TRISPORT &= ~LCD_MASK;// Make port to input
}
/* void LCDGotoXY(col, row)
 * Set cursor to row and columns position
 * Precon: none
 * Params: row 0-3 columns 0-max length
 * Return: none*/
void LCDGotoXY(uint8_t col, uint8_t row)
{
    uint8_t DDaddr = 0;
    switch(row)
    {
        case 0: DDaddr = 0x80; break; //LCD 0x00
        case 1: DDaddr = 0xC0; break; //LCD 0x40
        case 2: DDaddr = 0x94; break; //LCD 0x14
        case 3: DDaddr = 0xD4; break; //LCD 0x54
    }
    DDaddr += col;
    LCD_RSPORT &=~LCD_RSPIN; //Set RS=0 Command mode
    LCD_TRISPORT |= LCD_MASK; //Set data port as output 
    LCD_DATAPORT &= ~LCD_MASK;//Data pins in low level
#if LCD_MASK==0x0F 
    LCD_DATAPORT |= (DDaddr >> 4); //Send 4-bit MSB of CMD to Lower pins
#else
    LCD_DATAPORT |= (DDaddr & LCD_MASK); //Send 4-bit MSB of CMD to Upper pins
#endif
    LCD_ENPORT |= LCD_ENPIN; //Set pin to high level 
    LCD_delay_tcy(); 
    LCD_ENPORT &= ~LCD_ENPIN; //Set pin to low level
    LCD_DATAPORT &= ~LCD_MASK;//Data pins in low level 
#if LCD_MASK==0x0F 
    LCD_DATAPORT |= (DDaddr & LCD_MASK); //Send 4-bit LSB of CMD to Lower pins
#else
    LCD_DATAPORT |= (DDaddr << 4); //Send 4-bit LSB of CMD to Upper pins
#endif
    LCD_ENPORT |= LCD_ENPIN; //Set pin to high level 
    LCD_delay_tcy(); 
    LCD_ENPORT &= ~LCD_ENPIN; //Set pin to low level
    LCD_delay_cmd();
    LCD_TRISPORT &= ~LCD_MASK;// Make port to input
}
/* void LCDWriteMsg(char *str)
 * Write a null string at actual cursor position
 * Precon: none
 * Params: char pointer to null string
 * Return: none */
void LCDWriteMsg(char *str)
{
    while(*str)         	// Write data to LCD up to null
    {
    	LCDWriteChar(*str); // Write character to LCD
        str++;              // Increment buffer
    }
}
/* void LCDWriteMsgROM(char *str)
 * Write a null string at actual cursor position
 * Precon: none
 * Params: ROM char pointer to null string
 * Return: none */
void LCDWriteMsgROM(const char *str)
{
    while(*str)         	// Write data to LCD up to null
    {
    	LCDWriteChar(*str); // Write character to LCD
        str++;              // Increment buffer
    }
}

/* void LCDSetup(void)
 * Setup LCD with 4-bit mode in default screen
 * Precon: none
 * Params: none
 * Return: none*/
void LCDSetup(void)
{
    LCD_ENTRIS |= LCD_ENPIN;
    LCD_RSTRIS |= LCD_RSPIN;
#if LCD_RWENABLE==0x01
    LCD_RWTRIS |= LCD_RWPIN;
#endif
    LCD_RSPORT &= ~LCD_RSPIN; //Low level for pin
    LCD_ENPORT &= ~LCD_ENPIN; //Low level for pin
    _delay_ms(15); //15ms for LCD Power on reset
    LCD_TRISPORT |= LCD_MASK; //Set data port as output 
    LCD_DATAPORT &= ~LCD_MASK;//Data pins in low level
#if LCD_MASK==0x0F  //Check if LOWER mode
    LCD_DATAPORT |= 0x03;//First instruction for initialization sequence 
#else
    LCD_DATAPORT |= 0x30;//First instruction for initialization sequence
#endif    
    LCD_ENPORT |= LCD_ENPIN; //Set pin to high level 
    LCD_delay_tcy(); 
    LCD_ENPORT &= ~LCD_ENPIN; //Set pin to low level
    _delay_ms(5);   //5ms delay required 4.1ms
    LCD_DATAPORT &= ~LCD_MASK; //Put Data pins to low level  	
#if LCD_MASK==0x0F
    LCD_DATAPORT |= 0x03;//First instruction for initialization sequence 
#else
    LCD_DATAPORT |= 0x30;//First instruction for initialization sequence
#endif        
    LCD_ENPORT |= LCD_ENPIN; //Set pin to high level 
    LCD_delay_tcy(); 
    LCD_ENPORT &= ~LCD_ENPIN; //Set pin to low level
    _delay_us(100); //100u delay required
    LCD_DATAPORT &= ~LCD_MASK; //Put Data pins to low level  	
#if LCD_MASK==0x0F
    LCD_DATAPORT |= 0x03;//First instruction for initialization sequence 
#else
    LCD_DATAPORT |= 0x30;//First instruction for initialization sequence
#endif    
    LCD_ENPORT |= LCD_ENPIN; //Set pin to high level 
    LCD_delay_tcy(); 
    LCD_ENPORT &= ~LCD_ENPIN; //Set pin to low level
    LCD_delay_cmd();
    //Now LCD is ready for setting
    LCD_DATAPORT &= ~LCD_MASK;    	
#if LCD_MASK==0x0F
    LCD_DATAPORT |= 0x02;//Set Function Set to 4-bit in Lower mode 
#else
    LCD_DATAPORT |= 0x20;//Set Function Set to 4-bit in Upper mode
#endif
    LCD_ENPORT |= LCD_ENPIN; //Set pin to high level 
    LCD_delay_tcy(); 
    LCD_ENPORT &= ~LCD_ENPIN; //Set pin to low level
    LCD_delay_cmd();
    //Now LCD is un 4-bit mode
    LCDWriteCMD(LCD_FUNCTIONSET);
    LCDWriteCMD(LCD_DISPLAY & LCD_DCURSOROFF & LCD_DBLINKOFF);
    LCDWriteCMD(LCD_INPUTSET & LCD_INPSHIFTOFF);
    LCDWriteCMD(LCD_CLEAR);
    LCDWriteCMD(LCD_HOME);
}
