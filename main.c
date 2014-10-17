#include "include/control.h"
#include "include/interrupts.h"
#include "include/init.h"
#include "include/methods.h"

void main(void){
   
   init_config();
           
   if(input(IR_RECEIVER)){ fprintf(SERIAL, "Conexión no iniciada.\n\r\n\r"); }
   
   while(input(IR_RECEIVER));
      
   fprintf(SERIAL, "Conexión iniciada.\n\r\n\r");         
   RBIF=0;                          // Se limpia la bandera de interrupcion por cambio de estado de RB.
   enable_interrupts(GLOBAL);       // Permiso Global de interrupciones.
   
   while(1){            
      adcPwmEnhanced(52.0); //52 es el valor de CCPRxL:CCPxCON<5:4> cuando el "duty" esta al 100%.
      
      //Si hay algo que notificar entonces se realiza por medio de una comunicacion serial.
      if(notification){
         notification=false;     //Se desactiva la notificacion serial.
         serialNotification();   
      }
            
      //Si el enlace esta bloqueado da vuelta, sino camina hacia adelante.            
      if(really_blocked){
         driverStop();
         driverLeft();
      }
      else{ driverForward(); }
      
      //Calculo para la distancia recorrida y velocidad (odometro).
      if(sec_odom){
         sec_odom=false;
         odometer(2, 0.01);         
      }
      
   }  //</while>
   
}  //</main>
