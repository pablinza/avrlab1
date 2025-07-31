# MCU AVR LAB1 pablinza@me.com
Descripcion de Circuitos Logicos Combinatorios __CLC__ y Secuencuales __CLS__ con HDL / __Verilog__ <br />
Carpeta con los proyectos generados por GOWIN EDA utilizando el chip GW1NR-LV9QN88PC6/I5 de la placa Tangnano9k.<br />
El software IDE esta disponible en la pagina del fabricante GOWIN [ --> Click](https://www.gowinsemi.com/en/support/home/) <br />
La documentacion de la Tangnano9k puedes encontrarla en el sitio [ --> Click](https://wiki.sipeed.com/hardware/en/tang/Tang-Nano-9K/Nano-9K.html) <br />
La descripcion de circuitos se realiza con Verilog evitando en lo posible el uso de codigo IP, a excepcion de implementaciones mas complejas.
Se utilizo la version linux GOWIN EDA V1.9.11.02 corriendo en la distro Debian 13, abajo dejo un video de como realizar la instalacion del software a utilizar <br />
Para cargar el bitstream del circuito en la memoria RAM/FPGA o EEPROM utilizare openFPGALoader por linea de comandos [ -->Click](https://github.com/trabucayre/openFPGALoader) <br />
## Instalacion del GOWIN EDA y Activacion de Licencia 
En el siguiente enlace [--> Click](https://www.youtube.com/watch?v=hmUjBX4lj0o) dejo un video que muestra la instalacion de MPLABX + XC8 y la creacion de un proyecto base para el microcontrolador ATMega328. La implementacion de los circuitos en cada ejemplo, sera con una tarjeta UNO, considerando el mapa de pines del esquema inferior. 
<p align="center">
  <img src="/avruno.png"></img>
</p> <br />
El siguiente enlace (https://pcz1.blogspot.com/2025/06/introduccion-tarjeta-fpga-tangnano-9k.html) a mi blog brinda una descripcion resumida de esta tarjeta. Si realmente quieres entrar a fondo con el desarrollo de circuitos digitales te recomiendo que revisese los documentos del fabricante que son la mejor referencia de uso y resolucion de posibles errores (errdata). <br />
Cada carpeta del proyecto Gowin tiene el nombre precedido por el numero de actividad, en su estructura encontrara los modulos descriptivos Verilog con extension .v en la subcarpeta /src, mientras que el firmware .fs generado durante la sintesis se encuentra en la subcarpeta impl/pnr. <br />

## Lista de circuitos digitales implementados an la tangnano9k


Autor: Pablo Zarate, puedes contactarme a pablinza@me.com / pablinzte@gmail.com.  <br />
Visita mi Blog  [Blog de Contenidos](https://pablinza.blogspot.com/). <br />
Visita mi Canal [Blog de Contenidos](http://www.youtube.com/@pablozarate7524). <br />
Santa Cruz - Bolivia 
<br clear="left"/>
