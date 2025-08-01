# MCU AVR LAB1 pablinza@me.com
Programacion del Microcontrolador ATMega328 en lenguaje C con el compilador oficial de Microchip __XC8__ <br />
Repositorio con las practicas desarrolladas en la asignatura Microcontroladores, Carrera de Ingenieria Electronica <br />
El software __MPLABX__ disponible en la pagina del fabricante Microchip [ --> Click](https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/MPLABX-v6.20-windows-installer.exe?authuser=0) <br />
El compilador __XC8__ puedes descargalo utilizando este enlace [ --> Click](https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/xc8-v2.50-full-install-windows-x64-installer.exe?authuser=0) <br />
Para cargar el firmware al microcontrolador se necesita un programador ISP(MK500,AVR-ISP,etc). Pero, esto no sera necesarioa si utilizamos una tarjeta Arduino UNO o NANO gracias que el MCU de estas placas  poseen un bootloader precargado para la actualizacion del firmware desde el skecth __Arduino__ [ -->Click](https://www.arduino.cc/en/software/) <br />

Cada carpeta del proyecto MPLABX tiene el nombre precedido por el numero de actividad y en su estructura encontrara el programa principal con el nombre __main.c__ y librerias de uso local, una vez compilado el codigo, se generara el archivo __.hex__ con el firmware correspondiente en la carpeta dist/default/production. Utilizaremos una herramienta de carga como AVRDUDE [-->Click](https://github.com/avrdudes/avrdude) para transfererir este __.hex__ a la memoria de programa de Microcontrolador ATMega328 <br />
Para instalar AVRDUDE en linux (Debian o Ubuntu) bastara con ejecutar el siguiente comando: __#apt-get install avrdude__ <br /> 
En el caso de Windows debes seguir las instrucciones del archivo ZIP descargado, como alternativa existe tambien una herramienta con interfaz grafica llamada AVRDUDESS [-->Click](https://github.com/ZakKemble/AVRDUDESS) la cual no he utilizado ya que me siento a gusto con la linea de comandos de AVRDUDE, pero si no es lo tuyo, prueba esta alternativa y me comentas que tal esta. :) <br />


## Configuracion del Proyecto Base
En el siguiente enlace [--> Click](https://www.youtube.com/watch?v=hmUjBX4lj0o) dejo un video que muestra la instalacion de MPLABX + XC8 y la creacion de un proyecto base para el microcontrolador ATMega328. La implementacion de los circuitos en cada ejemplo, sera con una tarjeta UNO, considerando el mapa de pines del esquema inferior. 
<p align="center">
  <img src="/avruno.png"></img>
</p> <br />

Programa base __main.c__ para las practicas de programacion

```c
#include <xc.h>
volatile uint8_t tickms;
void taskLED(void);
void setupMCU(void);
void __interrupt(TIMER0_OVF_vect_num) t0_isr(void) //Interrupcion
{
    TCNT0 += 5; //Actualiza el contador
    tickms = 1; //Activa bandera
}
int main(void) //Programa principal
{
    setupMCU(); //Configura el MCU
    while(1)
    {
        if(tickms) //Cada 1ms
        {
            tickms = 0; //limpia bandera
            taskLED(); //Destella LED
        }
    }
    return 0;
}
void taskLED(void) //Ejecucion cada 1ms
{
    static uint16_t cnt = 0;
    if(cnt++ > 999)
    {
        cnt = 0;
        PORTB |= _BV(PB5); //Nivel alto en PB5
    }
    if(cnt == 100) PORTB &= ~_BV(PB5); //Nivel bajo en PB5
}
void setupMCU(void)
{
  DIDR0 = 0x3F; //Desactiva canales ADC5:0
  DDRB |= _BV(PB5); //PB5 salida led 13(UNO) 
  PORTB &= ~_BV(PB5); //PB5 en nivel bajo
  /* CONFIGURACION TIMER0 1MS Fosc=16MHz */
  TCCR0A = 0x00; //Modo normal
  TCCR0B = 0x03;//Ajuste Divisor CS=1:64 
  TCNT0 = 5; //TCNT0=255-(0.001*Fosc/64)=05
  TIMSK0 |= _BV(TOIE0); //Activa interrupcion del T0  
}
```

El siguiente enlace (https://pcz1.blogspot.com/2025/06/introduccion-tarjeta-fpga-tangnano-9k.html) a mi blog brinda una descripcion resumida de esta tarjeta. Si realmente quieres entrar a fondo con el desarrollo de circuitos digitales te recomiendo que revisese los documentos del fabricante que son la mejor referencia de uso y resolucion de posibles errores (errdata). <br />
Cada carpeta del proyecto Gowin tiene el nombre precedido por el numero de actividad, en su estructura encontrara los modulos descriptivos Verilog con extension .v en la subcarpeta /src, mientras que el firmware .fs generado durante la sintesis se encuentra en la subcarpeta impl/pnr. <br />

## Lista de circuitos digitales implementados an la tangnano9k


Autor: Pablo Zarate, puedes contactarme a pablinza@me.com / pablinzte@gmail.com.  <br />
Visita mi Blog  [Blog de Contenidos](https://pablinza.blogspot.com/). <br />
Visita mi Canal [Blog de Contenidos](http://www.youtube.com/@pablozarate7524). <br />
Santa Cruz - Bolivia 
<br clear="left"/>
