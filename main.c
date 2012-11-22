#include "define.h"

void main( void )
{
  
  ini();
  Flags.radio_receive=      1;


    WDRCLR;
    __enable_interrupt();  
  for(;;)
 {
         WDRCLR;
   if(Flags.systick)            
    {
      Flags.systick    = 0;
      date_time();
    }
 /*
   if (Flags.radio_receive)
   {
     static Int8U j;
     *((pInt8U)&Rclock_main)=RxBuffer[0];
     *((pInt8U)&Rclock_main+1)=RxBuffer[1];
   //  Rclock_main=*((pInt16U) &RxBuffer[0]);
     clock_err[j++]=Rclock-Rclock_main;
     Rclock=Rclock_main;    
     
     Flags.radio_receive=0;
   }     
   */  
   if (Flags.radio)                      
    {
  P1OUT= 0x02;                        // Pulse LED during Transmit   
//P1OUT &= ~(BIT0+BIT1+BIT2);

      
 /*     ReceiveOff();
     
      receiving = 0; 
//      *((pInt16U)&TxBuffer[0])=Rclock;
      Transmit( (unsigned char*)TxBuffer, sizeof TxBuffer);         
      transmitting = 1;
  */ 
      Flags.radio = 0;                        // Re-enable button press  
    }
  else if(!transmitting)
    {
      ReceiveOn();      
      receiving = 1; 
    }   
     if(Flags.uart_recieve)    uart_process(); 
 }
 
}
