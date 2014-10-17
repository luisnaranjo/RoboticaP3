/* -------------------------------------------------------------------------------------------------------------------- */
/*                                                 COMUNICACION SERIAL                                                  */
/* -------------------------------------------------------------------------------------------------------------------- */
void serialNotification(){
//Metodo que notifica por medio de la interfaz serial si se ha establecido o terminado el enlace IR.
      
   if(really_blocked){
      output_high(STATUS_LED);   //LED indicador activo.
      fprintf(SERIAL, "El enlace IR ha sido interrumpido.\n\r");
   }
   else{
      seconds=(float)hundred_ms/10.00;
      output_low(STATUS_LED); //LED indicador inactivo.
      fprintf(SERIAL, "Enlace IR restablecido. \n\rConexión interrumpida por %4.2f segundos \n\r\n\r", seconds);                        
      
      //Se reinician los contadores.
      seconds=0;
      hundred_ms=0;
   }                                  
}  //</serialNotification>


/* -------------------------------------------------------------------------------------------------------------------- */
/*                                                           PWM                                                        */
/* -------------------------------------------------------------------------------------------------------------------- */
void adcPwmEnhanced(float max_pwm){
//Metodo que convierte el valor leído del modulo ADC en el ancho de pulso del modulo PWM.
//Da un "Duty" de entre 10% y 90%.
//El parametro "max_pwm" se refiere al valor de CCPRxL:CCPxCON<5:4> cuando el "duty" esta al 100%.

   float adc_reader=0;
   int16 duty=0;
   
   //Ciclo de trabajo: si 0 = 0%, si 1000 = 100%. Como se usara entre  10% y 90% Entonces se calcula para que el valor maximo sea 800 (1023/800 = 1.27875),
   //Asi se le podra aumentar 100 para que el valor minimo sea 100 (10%) y el valor maximo 900 (90%).
   
   adc_reader=(read_adc()/1.27875)+100;         //Formula para que el valor del duty sea de entre 10% y 90%.
   duty=(int16)((max_pwm/1023.0)*adc_reader);   //Formula para adaptar los valores a la frecuencia utilizada.    
   delay_ms(20);                                //Delay de estabilización.
   set_pwm1_duty(duty);                         //Asignacion del "duty" al modulo CCP1. 
} //</adc_pwmEnhanced>


/* -------------------------------------------------------------------------------------------------------------------- */
/*                                                          TRACCION                                                    */
/* -------------------------------------------------------------------------------------------------------------------- */
void driverForward(){
//Metodo encargado de operar la direccion de manejo del robot hacia adelante.
   //Para manejar hacia adelante al robot se deben de establecer a LW como CCW y RW como CW.
   
   output_low(STBY); //Modo standby del driver acrivado.
   
   //Rueda izquierda [CCW].
   output_low(LW_1);
   output_high(LW_2);
   
   //Rueda derecha [CW].
   output_high(RW_1);
   output_low(RW_2);
   
   output_high(STBY); //Modo standby del driver desactivado.      
} //</driveForward>


void driverReverse(){
//Metodo encargado de operar la direccion de manejo del robot hacia atras.
   //Para manejar en reversa al robot se deben de establecer a LW como CW y a RW como CCW.

   output_low(STBY); //Modo standby del driver activado.
   
   //Rueda izquierda [CW].
   output_high(LW_1);
   output_low(LW_2);
   
   //Rueda derecha [CCW].
   output_low(RW_1);
   output_high(RW_2);
   
   output_high(STBY); //Modo standby del driver desactivado.
}  //</driveReverse>


void driverLeft(){
//Metodo encargado de operar la direccion de manejo del robot hacia la izquierda.
   
   output_low(STBY); //Modo standby del driver activado.
   
   //Rueda izquierda [CW].
   output_high(LW_1);
   output_low(LW_2);   
   
   //Rueda derecha [CW].
   output_high(RW_1);
   output_low(RW_2);   
   
   output_high(STBY); //Modo standby del driver desactivado.
} //</driveLeft>


void driverRight(){
//Metodo encargado de operar la direccion de manejo del robot hacia la derecha.

   output_low(STBY); //Modo standby del driver activado.
   
   //Rueda izquierda [CCW].
   output_low(LW_1);
   output_high(LW_2);   
   
   //Rueda derecha [CCW].
   output_low(RW_1);
   output_high(RW_2);   
   
   output_high(STBY); //Modo standby del driver desactivado.
} //</driveRight>


void driverStop(){
//Metodo encargado de operar la direccion de manejo, dando un stop.
   //Para parar al robot inmediatamente se deben de establecer en HIGH las dos entradas de cada una de las ruedas.
   //Establecimiento en alto de las entradas para la rueda izquierda.
   output_high(LW_1);
   output_high(LW_2);
   
   //Establecimiento en alto de las entradas para la rueda derecha.
   output_high(RW_1);
   output_high(RW_2);
     
   output_low(STBY); //Modo standby del driver activado.
} //</driveStop>


/* -------------------------------------------------------------------------------------------------------------------- */
/*                                                        ODOMETRO                                                      */
/* -------------------------------------------------------------------------------------------------------------------- */
void odometer(int8 wheel, float dist_per_hole){
//Funcion que calcula la distancia recorrida en metros.
//wheel:          Cantidad de ruedas a calcular.
//dist_per_hole:  Distancia que se tiene por cada cambio de estado [Valor en metros].
      int8 aux=0; //Variable auxiliar donde se almacenara el valor de la variable odom_cont.
      float distance_temp=0;
      
      aux=odom_cont;
      odom_cont=0;
      distance_temp=((float)aux/(float)wheel)*dist_per_hole;
      distance+=distance_temp;      
      vel=distance_temp*3.6;  // Para la conversion de m/s a Km/h.
      
      fprintf(SERIAL, "Distancia recorrida: %4.2f metros.\n\r Velocidad:  %4.2f \n\r\n\r", distance, vel);  
}  //</odometer>
