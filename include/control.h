#include <18F4620.h>

#device adc=10

#fuses INTRC_IO, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG 

#use delay(clock=32000000)

#USE FAST_IO (A)                   // Registro de configuración TRISA fijo
#USE FAST_IO (B)                   // Registro de configuración TRISB fijo
#USE FAST_IO (C)                   // Registro de configuración TRISC fijo


/* -------------------------------------------------------------------------------------------------------------------- */
/*                                           IDs Y CONSTANTES DE CONTROL                                                */
/* -------------------------------------------------------------------------------------------------------------------- */
//REGISTROS:
#byte INTCON   =  0xFF2       // Registro de Control de Interrupcion.
#bit  RBIF     =  INTCON.0    // Bandera de la interrupcion por cambio de estado.

//TRANSMISION SERIAL:
#define SERIAL_TX    PIN_C6   // Pin transmisor serial.
#define SERIAL_RX    PIN_C7   // Pin receptor serial.

//INFRA-ROJO:
#define IR_RECEIVER  PIN_B4   // Pin donde se conecta el receptor IR.
#define STATUS_LED   PIN_B3   // Pin indicador de estado de conexion IR.
#define BLOCKED_HMS  5        // Indica cuantos cientos de milisegundos el enlace tiene que estar bloqueado para que se considere que se pierde la conexion.

//MANEJO DE RUEDAS:
#define LW_PWM    PIN_D0   // PWMA: Pin de salida del PWM de la rueda izquierda a 4KHz con duty de 50%.
#define LW_1      PIN_D1   // AIN1: Pin 1 para el manejo de la rueda izquierda.
#define LW_2      PIN_D2   // AIN2: Pin 2 para el manejo de la rueda izquierda.
#define RW_PWM    PIN_D3   // PWMB: Pin de salida del PWM de la rueda derecha a 4KHz con duty de 50%.
#define RW_1      PIN_D4   // BIN1: Pin 1 para el manejo de la rueda derecha.
#define RW_2      PIN_D5   // BIN2: Pin 1 para el manejo de la rueda derecha.
#define STBY      PIN_D6   // Pin para el manejo del Standby del Driver.

//ODOMETRO:
#define DISTANCE_LW  PIN_B0   //Pin por donde se analizaran los cambios de estado respecto a la rueda izquierda para el calculo de la distancia.
#define DISTANCE_RW  PIN_B1   //Pin por donde se analizaran los cambios de estado respecto a la rueda derecha para el calculo de la distancia.
#define SECOND       10       //Valor de 1 segundo en cientos de milisegundos.


/* -------------------------------------------------------------------------------------------------------------------- */
/*                                        DEFINICION DE INTERFACES DIGITALES DE COMUNICACION                            */
/* -------------------------------------------------------------------------------------------------------------------- */
#use RS232(STREAM = SERIAL, BAUD=9600, XMIT=SERIAL_TX, RCV=SERIAL_RX)


/* -------------------------------------------------------------------------------------------------------------------- */
/*                                            DEFINICION DE VARIABLES                                                   */
/* -------------------------------------------------------------------------------------------------------------------- */
//CAMBIO DE ESTADO:
short ir_blocked=false;       //Bandera que se activa cuando el enlace IR se ha terminado.
short really_blocked=false;   //Bandera que se activa cuando el enlace IR se ha bloqueado por la cantidad de cientos de milisegundos de BLOCKED_HMS.
short notification=false;     //Bandera encargada de notificar si se ha terminado o establecido el enlace IR.

//TIMER:
float seconds=0;        //Variable que indicara cuantos segundos el enlace fue interrumpido.
int32 hundred_ms=0;     //Variable contadora que incrementara cada 100ms siempre y cuando el enlace este interrumpido.
int8  timer_overflow=1; //Variable contadora para indicar que han pasado 100ms. 
                        
//ODOMETRO:
float distance=0;       //Variable que contendra el valor de distancia.
int8 odom_cont=0;       //Variable donde se almacena la cantidad de cambio de estados.
int32 hundred_ms_odom=0;
int8 timer_overflow_odom=0;
short sec_odom=false;        //Bandera para entrar a la funcion odometer.
float vel=0;            //Variable que contendra el valor de la velocidad.
