#include "define.h"

void main( void )
{
  
  ini();
  SFRIE1= WDTIE+OFIFG;
  Flags.radio_receive=      1;


    WDRCLR;
    SFRIFG1 &= ~OFIFG;
    __enable_interrupt();
  for(;;)
 {
        WDRCLR;
        if(Flags.systick)            
    {
      Flags.systick    = 0;
      Rclock++;
      date_time();
    }
/*
   if (Flags.radio_receive)
   {
     static Int8U j;
     *((pInt8U)&Rclock_main)=RxBuffer[0];
   //  Rclock_main=*((pInt16U) &RxBuffer[0]);
     clock_err[j++]=Rclock-Rclock_main;
     Rclock=Rclock_main;    
     
     Flags.radio_receive=0;
   }     
*/
    
    if (Flags.radio)                      
    {
      P1OUT &= ~(BIT0+BIT1+BIT2);
       Flags.radio = 0; 
    }
        
      // Pulse LED during Transmit   
 // 

   if (Flags.radio_receive)                      
    {
                          // Pulse LED during Transmit   
      static U8 h;
        h=0;
      
      ReceiveOff();
     
      receiving = 0; 
    
      *((pInt8U)&TxBuffer[0])=Rclock;
    
      for(h=0;h<4;h++)
      {
      if(!transmitting)
    {
      Transmit( (unsigned char*)TxBuffer, sizeof TxBuffer);         
      transmitting = 1;
    }
      }
      Flags.radio_receive=0;
    }
 /*
   else if(!transmitting)
    {
      ReceiveOn();      
      receiving = 1; 
    }   
    */
     if(Flags.uart_recieve)    uart_process(); 
 }
 
}
