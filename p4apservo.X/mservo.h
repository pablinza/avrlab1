/* File:   mservo.h / mservo.c
 * Author: Pablo Zarate email:pablinza@me.com
 * Comments: 8-bit Timer Servo Implementation for AVR 
 * Designed for multiple servo controller with timer
 * 8-bit Timer module for 08us step is required for operation
 * #define MSVPORTx Port servo control <pos6><pos5><pos4><pos3><pos2><pos1>
 * #define MSVTRIS  Port servo direction configuration
 * Set MSV_t MSV declaration on main.c 
 * Set MSV.svxpos beetwen 0 (1ms pulse width) - 125(2ms pulse width) 
 * Revision history: 25.08 */
#ifndef  _XC_MSERVO_H
#define  _XC_MSERVO_H
/* USER PORT DEFINITION */
#define MSVNUM 1      //Numero de servos a conectar, Maximo 8. 20ms/8 > 2.5ms.
#define MSVPIN 0      //Indica el numero de pin indice 0-7
#define MSVSLOT 250U  //Maximo desplazamiento 250 x 8us = 2ms 
#define MSVTIME0 125U //Minimo desplazamiento 120 x 8us = 1ms 
#define MSVPORT PORTC //Port servo control <pos6><pos5><pos4><pos3><pos2><pos1>
#define MSVTRIS DDRC  //Port servo config mode
/* END USER PORT DEFINITIOS*/
typedef struct
{
    uint8_t sv1pos; 
    uint8_t sv2pos;
    uint8_t sv3pos;
    uint8_t sv4pos;
    uint8_t sv5pos;
    uint8_t sv6pos;
    uint8_t sv7pos;
    uint8_t sv8pos;
} MSV_t;
uint8_t MSERVOHandler(void);
void MSERVOSetup(void);
#endif
