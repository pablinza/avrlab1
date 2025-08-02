# AVRMCU LAB1 pablinza@me.com
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

Programa base __main.c__ que destella el LED interno de la placa UNO y se utilizara en todas las practicas de este repositorio.

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
  DIDR0 = 0x3F;     //Desactiva canales ADC5:0
  DDRB |= _BV(PB5); //PB5 salida led 13(UNO) 
  PORTB &= ~_BV(PB5);  //PB5 en nivel bajo
  /* CONFIGURACION TIMER0 1MS Fosc=16MHz */
  TCCR0A = 0x00; //Modo normal
  TCCR0B = 0x03; //Ajuste Divisor CS=1:64 
  TCNT0 = 5; //TCNT0=255-(0.001*Fosc/64)=05
  TIMSK0 |= _BV(TOIE0); //Activa interrupcion del T0
  SREG = _BV(SREG_I);   //Habilitador Global de interrupcion  
}
```

El siguiente enlace (http://pcz1.blogspot.com/2022/03/proteccion-de-la-memoria-atmega.html) a mi blog describo el uso de AVRDUDE para la carga del firmware a la tarjeta UNO. Si realmente quieres entrar a fondo con la programacion de este Microcontrolador te recomiendo que revisese los documentos del fabricante que son la mejor referencia de uso y resolucion de posibles errores (errdata). <br />
Cada proyecto MPLABX es una carpeta con extension __.X__ que tiene el nombre precedido por el numero de actividad <br />

## Lista de practicas de programacion Placa UNO (ATMega328)
### - P01 - Destello de Luz Secuencial   [APPBASE]
  <p align="center">
  <img src="/images/ap1schm.png"></img> </p>

El programa destella los leds conectados al __MCU__ con frecuencia y ciclo de trabajo diferente, donde se utiliza la interrupcion del temporizador T0 de 8-bit configurado a 1ms. La rutina de interrupcion ISR para el temporizador activa la bandera __[tickms]__ que servira para la ejecucion concurrente de dos tareas dentro del bucle __while__ del programa principal. Se muestra un video que resume la implementacion de este ejercicio -> (https://youtu.be/rYyD9MK57Po)

### - P02 - Control de Trafico Vehicular [APTRAFFIC]
<p align="center">
  <img src="/images/ap2schm.png"></img> </p>

El programa muestra un ejemplo de como implementar un controlador de trafico para dos fases (interseccion de calles), utilizando la interrupcion del temporizador __T0__ como base de tiempo para la ejecucion concurrente de tareas en el programa principal. Ademas se adiciona una entrada de alerta __SW1__ que se activa en caso de falla ocasionando que el controlador ingrese en modo de mantenimiento. En el modo mantenimiento las luces amarillas destellaran de manera intermitende. Se muestra un video que resume la implementacion de este ejercicio -> (https://youtu.be/hlDj-E___kY)

### - P03 - Monitor de Datos Serial ADC  [APMONITOR]
<p align="center">
  <img src="/images/ap3schm.png"></img> </p>
  
En esta practica se elabora un programa para realizar la lectura de los canales analogicos __ADC0__ y __ADC1__, y la lectura del estado que tiene el pulsador __SW1__. Las tareas se ejecutan de forma concurrente en el programa principal utilizando como base de tiempo la interrupcion del temporizador __T0__. Adicionalmente se envia por el puerto serie UART un mensaje con el resultado de la conversion ADC y estado del pulsador. La implementacion utiliza como fuente ADC un joystick de doble eje y como salida una interfaz con graficas en PC. Se muestra un video que resume la implementacion de este ejercicio -> (https://youtu.be/-FM--FO0nE8)

### - P04 - Controlador para Servomotor  [APSERVO]
<p align="center">
  <img src="/images/ap4schm.png"></img>
</p>

Se elabora un programa que muestra como controlar la posicion de un servomotor __SG90__, donde el control de movimiento o posicion del eje servomotor se realiza con escala en porcentaje de 0% a 100%. Ademas se realiza la lectura de un sensor de distancia por ultrasonido __SR04__ para detectar la presencia de un objeto y desplazar una barra conectada el eje del sermovotor. El programa hace uso de la interrupcion del temporizador __T0__ para la ejecucion de tareas concurrentes dentro del programa principal y el temporizador __T2__ para la modulacion PWM del servomotor. Se muestra un video que resume la implementacion de este ejercicio -> (https://youtu.be/369__Zxra0M)

### - P05 - Control de Acceso Autorizado [APKEYPAS]
<p align="center">
  <img src="/images/ap5schm.png"></img>
</p>

Programa que muestra el uso de un teclado matricial 3x4 para la lectura de una contraseña de cuatro digitos y proceder con su validacion, mostrando el resultado en una pantalla LCD 16x2. Durante el proceso la pantalla muestra el ingreso de los datos y un mensaje indicando si la contraseña fue aceptada o rechazada, permitiendo como maximo tres intentos antes de bloquear el acceso, que unicamente se restaura reiniciando el programa. Se utiliza el __T0__ como base de tiempo para la ejecucion de tareas concurrentes. Se muestra un video que resume la implementacion de este ejercicio -> (https://youtu.be/mYJeusZr0Jg)

 <br />
Autor: Pablo Zarate, puedes contactarme a pablinza@me.com / pablinzte@gmail.com.  <br />
Visita mi Blog  [Blog de Contenidos](https://pablinza.blogspot.com/). <br />
Visita mi Canal [Blog de Contenidos](http://www.youtube.com/@pablozarate7524). <br />
Santa Cruz - Bolivia 
<br clear="left"/>
