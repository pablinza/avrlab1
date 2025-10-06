/* File:   mservo.h / mservo.c
 * Author: Pablo Zarate email:pablinza@me.com
 * Comments: Libreria de control de servos para AVR
 * Creado para el control de multiples servos con temporizador TC0 de 8-bit
 * Se requiere configurar temporizador para pasos de 8uS
 * #define MSVPORTx Registro de control Pines <pos7>....<pos3><pos2><pos1>
 * #define MSVTRISx Registro de configuracion de Pines
 * Set MSV_t MSV declaration on main.c 
 * Set MSV.svxpos beetwen 0 (1ms pulse width) - 125(2ms pulse width) 
 * Revision history: 25.08 
 * 25.10 Se cambia definicion MSVTIME0 por MSVPOS0 
 * 25.08 Optimizacion de codigo
 */
#include <xc.h>
#include "mservo.h"
extern MSV_t MSV;
/* Interrupt SERVO handler for control   
 * Params: None, wait for timer   
 * Return: next timer setting in us 
 *   if(PIR1bits.TMRxIF) TSERVOHandler();*/
uint8_t MSERVOHandler(void)
{
    static uint8_t pos = 0x01, cnt = 0;
    uint8_t *svptr; //Pointer to byte struct
    uint8_t res; 
    uint8_t timer; //next timer value
    if(cnt > (MSVNUM-1U)) //Toma control de pines
    {
        timer = MSVSLOT;
        if(++cnt == 10) //completa hasta 10 slots
        {
            cnt = 0;
            pos = 0x01; //Reinicia posicion de pin
        }
    }
    else
    {
        svptr = &MSV.sv1pos;
        svptr += cnt; //Increment count
        res = (pos << MSVPIN); //Valor del pin de control real
        if(MSVPORT & res) //Read state of pin by pos
        {
            MSVPORT &= (~res); //Set pint to LOW
            timer = (MSVSLOT-125U) - (*svptr);
            cnt++; //Increment counter
            pos <<= 1;
        }
        else 
        {
            MSVPORT |= res; //Set pint to HIGH
            timer = 125u + (*svptr); //Set active pulse width
        }
    }
    return timer;
}

/* TSERVO Port Setup   
 * Params: None    
 * Return: None */
void MSERVOSetup(void)
{
    uint8_t res = 0xFE;
    if(MSVNUM)
    {
        res <<= (MSVNUM - 1U);
        res = (~res) << MSVPIN;
        MSVTRIS |= res; //Config output
        MSVPORT &= ~res; //Set value
    }
}
