void init_config(){
//Metodo que se encarga de realizar las configuraciones iniciales.

   //Configuracion de oscilador.
   setup_oscillator(OSC_32MHZ);  // Oscilador interno a 8Mhz con PLL
   delay_ms(10);
           
   //Configuracion de TRISx.          
   set_tris_a(0x01); //0000 0001
   set_tris_b(0xF0); //1111 0000
   set_tris_c(0x80); //1000 0000
   set_tris_d(0x00); //0000 0000
   
   //Configuracion de estado inicial de los pines.
   output_low(STATUS_LED); //El estado inicial del LED indicador es en LOW.
   
   //Configuracion de la traccion.
   output_low(STBY);
   
   //Configuracion del modulo ADC.
   setup_adc(ADC_CLOCK_INTERNAL);// Se utiliza el reloj interno para la conversion en el modulo ADC.
   setup_adc_ports (AN0);        // Sin entradas analogicas para el ADC.
   set_adc_channel(0);           //Seleccion de canal analogico (AN0).
   delay_ms(20);                 //Delay de estabilización.
   
   //Configuracion del PWM.
   setup_ccp1(ccp_pwm);                   //Configuracion del modulo CCP1 como PWM.
   setup_timer_2(T2_DIV_BY_16,12,1);      //PWM con una frecuencia de 38Khz.
   set_pwm1_duty(520);                    //PWM con un ciclo de trabajo de 10%.
      
   //Configuracion del TMR0.
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_4);  //Se utiliza el reloj interno.
                                             //Se utiliza un preescaler de 4.
                                             //Se utiliza la variable de desbordamiento de 16 bits.
                                             //Se requerira 3 desbordamientos para que pasen 100ms.
   set_timer0(0);                            //Valor inicial para el timer0.
   
   //Configuracion del TMR1.
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);   //Se utiliza el reloj interno.
                                             //Se utiliza un preescaler de 1.
   set_timer1(64536);                        //Valor establecido en timer1 para una frecuencia de 4KHz.                 
   
      
   //Configuracion de interrupciones.
   enable_interrupts(INT_RB);
   enable_interrupts(INT_TIMER0);   //Habilitación de la interrupcion del timer0.
   enable_interrupts(INT_TIMER1);   //Habilitación de la interupcion del timer1.
//   enable_interrupts(GLOBAL);       // Permiso Global de interrupciones

    
}

/*
DEFINICION DE PINES:
   |-----------------------------------------------------------------------------------------------------------------|
   |  PIN   |    TIPO   |                                          FUNCION                                           |                                  
   |--------|-----------|--------------------------------------------------------------------------------------------|
   |   A0   |  Entrada  |  Pin analogico donde se realiza la lectura del potenciometro.                              |
   |   B0   |  Entrada  |  Pin donde se conectara el receptor infrarojo.                                             |
   |   B1   |  Salida   |  Pin (LED) que sera el encargado de mostrar el estado de la conexion.                      |
   |   C2   |  Salida   |  Pin donde se conectara el Diodo IR emisor                                                 |
   |   C6   |  Salida   |  Pin transmisor de la interfaz serial: SERIAL.                                             |
   |   C7   |  Entrada  |  Pin receptor de la interfaz serial: SERIAL.                                               |
   |   D0   |  Salida   |  Pin de salida donde se emitira una frecuencia de 4KHz con duty de 50% (Rueda izquierda).  |
   |   D1   |  Salida   |  Pin de salida No.1 para el manejo de la rueda izquierda.                                  |
   |   D2   |  Salida   |  Pin de salida No.2 para el manejo de la rueda izquierda.                                  |
   |   D3   |  Salida   |  Pin de salida donde se emitira una frecuencia de 4KHz con duty de 50% (Rueda derecha).    |
   |   D4   |  Salida   |  Pin de salida No.1 para el manejo de la rueda derecha.                                    |
   |   D5   |  Salida   |  Pin de salida No.2 para el manejo de la rueda derecha.                                    |
   |   D6   |  Salida   |  Pin de salida tipo STANDBY para el driver.                                                |
   
NOTAS:
   - Mejorar el problema con la interrupcion del cambio de estado en PORTB, limpiando la bandera de interrupcion.
   - El sistema odometro esta configurado para una sola rueda. Si se implemementa para dos ruedas se tendrá que modificar el isr para la medicion de ambas
     ruedas. Ademas de indicarle lo mismo a la funcion odometer por medio de sus parametros [en main].
   
DUDAS:
   
*/
