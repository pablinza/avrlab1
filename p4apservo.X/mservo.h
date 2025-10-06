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
#ifndef  _XC_MSERVO_H
#define  _XC_MSERVO_H
/* USER PORT DEFINITION */
#define MSVNUM 1      //Numero de servos a conectar, Maximo 8. 20ms/8 > 2.5ms.
#define MSVPIN 2      //Indica el numero de pin indice 0-7
#define MSVSLOT 250U  //Maximo desplazamiento 250 x 8us = 2.0ms 
#define MSVPOS0 188U  //Posicion central servo 188 x 8us = 1.5ms 
#define MSVPORT PORTD //Port servo control <pos6><pos5><pos4><pos3><pos2><pos1>
#define MSVTRIS DDRD  //Port servo config mode
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
