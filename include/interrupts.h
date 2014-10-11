#int_timer0
void isr_tmr0(){
//Rutina de interrupcion la cual aumentara un contador cada vez que pasen 100ms (0.1s), siempre y cuando el enlace IR este bloqueado
   if(ir_blocked){
      if(timer_overflow>=3){
         hundred_ms++;
         
         //Si el enlace ha sido bloqueado por el tiempo de BLOCKED_HMS y no habia sido interrumpido anteriormente, entonces se activa la bandera.
         if((hundred_ms>=BLOCKED_HMS)&&(!really_blocked)){
            really_blocked=true; 
            notification=true;   //Se activa la notificacion serial. 
         }
         
         timer_overflow=1;
      }
      else{ timer_overflow++; }
   }   
}  //</isr_tmr0>
 
#int_timer1
void isr_tmr1(){
//Rutina de interrupcion en donde se genera la se�al a 4KHz.
   output_toggle(LW_PWM);
   output_toggle(RW_PWM);
   set_timer1(64536); 
}  //</isr_tmr1>

#int_rb
void isr_ext(){
// Rutina de interrupcion la cual se ejecutara cada vez que cambie de estado el PIN RB4[IR] o RB5,RB6[ODOMETRO].
// Verifica si se ha bloqueado o desbloqueado la se�al.
//Contabiliza los cambios de estado para el odometro.
     
   //Si se ha bloqueado el enlace entonces se activa la bandera. 
   if(input(IR_RECEIVER)){ 
      ir_blocked=true;
      set_timer0(0); //Se reinicia el contador del TMR0 debido a que debe de iniciar a contar cuando se bloquee el enlace.
   }
   else { 
      if(really_blocked){ notification=true; }  //Si se habia bloqueado la se�al, entonces se activa la notificacion.
      ir_blocked=false;
      really_blocked=false;
   }
   
   //Odometro:
   if((input(DISTANCE_LW))&&(!really_blocked))
      odom_cont++;
}  //<isr_ext>