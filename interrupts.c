#include  "define.h"

#pragma vector=TIMER1_A1_VECTOR
__interrupt void ta1_isr(void)
{          
  static U16   rtc;  
  static U8    clock_ctr;
  
  TA1CTL &= ~TAIFG;
  
  // RTC 
  rtc += (TA1CCR0 + 1);
  if(rtc >= 1024)
  {
    rtc -= 1024;
    
    clock_ctr++;
    Flags.systick=    1;
    if(clock_ctr==5) 
    {
      Flags.radio_receive=      1;
      clock_ctr=0;
    }
   // Flags.radio_receive=      1;
    Flags.radio=      1;
    __low_power_mode_off_on_exit();                  // для сброса WDT и подсчета времени
  }
  
  
  
}
 

#pragma vector=CC1101_VECTOR
__interrupt void CC1101_ISR(void)
{
  
  switch(__even_in_range(RF1AIV,32))        // Prioritizing Radio Core Interrupt 
  {
    case  0: break;                         // No RF core interrupt pending                                            
    case  2: break;                         // RFIFG0 
    case  4: break;                         // RFIFG1
    case  6: break;                         // RFIFG2
    case  8: break;                         // RFIFG3
    case 10: break;                         // RFIFG4
    case 12: break;                         // RFIFG5
    case 14: break;                         // RFIFG6          
    case 16: break;                         // RFIFG7
    case 18: break;                         // RFIFG8
    case 20:                                // RFIFG9
      if(receiving)			    // RX end of packet
      {
        // Read the length byte from the FIFO       
        RxBufferLength = ReadSingleReg( RXBYTES );               
        ReadBurstReg(RF_RXFIFORD, RxBuffer, RxBufferLength); 
         
        // Stop here to see contents of RxBuffer
        __no_operation(); 		   
        // Check the CRC results
        if(RxBuffer[CRC_LQI_IDX] & CRC_OK)  
        {
     //  P1OUT= 0x02;                       // Toggle LED1 
       P1OUT &= ~(BIT0+BIT1+BIT2);   
 /*
       if(!Flags.count)
    {
     *((pInt8U)&Rclock_main)=RxBuffer[0];
     *((pInt8U)&Rclock_main+1)=RxBuffer[1];
     Rclock=Rclock_main;    
     Flags.count=1;
    }
      
            static Int8U j;
     *((pInt8U)&Rclock_main)=RxBuffer[0];
     *((pInt8U)&Rclock_main+1)=RxBuffer[1];
     clock_err[j++]=Rclock-Rclock_main;
    // Rclock=Rclock_main;    
   */  
        }

      }
      else if(transmitting)		    // TX end of packet
      {
        RF1AIE &= ~BIT9;                    // Disable TX end-of-packet interrupt
        P1OUT &= ~(BIT0+BIT1+BIT2);                     // Turn off LED after Transmit               
        transmitting = 0; 
      }
      else while(1); 			    // trap 
      break;
    case 22: break;                         // RFIFG10
    case 24: break;                         // RFIFG11
    case 26: break;                         // RFIFG12
    case 28: break;                         // RFIFG13
    case 30: break;                         // RFIFG14
    case 32: break;                         // RFIFG15
  }  
 // __bic_SR_register_on_exit(LPM3_bits);     
}



#pragma vector = USCI_A0_VECTOR
__interrupt void uart_a0_isr(void)  

{  
  volatile Int8U     inbyte;
   
  switch(UCA0IV)
  {     
    
    case 2: inbyte=     UCA0RXBUF;         
    P1OUT= 0x02;
  if(Flags.uart_end)
            {   
             if(inbyte == 0x0d)
              {
              Flags.uart_end= RxCounter=  TxPointer= UCA0STAT= 0;
              }
              break;
            }
    
            UCA0STAT=   0;
            if(!RxCounter && (inbyte != '$'))  break;
            if(inbyte == '$') RxCounter= 0;     
            if(RxCounter == UART_BUF_LEN) 
            { 
              RxCounter= 0; 
              break; 
            }  
            if(inbyte != 0x0d) 
            { 
              UartBuffer[RxCounter++]= inbyte; 
              break;
            }
            
            Flags.uart_recieve=  1;
            RXI_DISABLE;
            __low_power_mode_off_on_exit();
            break;
             
    case 4: if(UartBuffer[TxPointer] == 0x0d)
            {
              UCA0STAT=             UCLISTEN;   // clear error bits
              inbyte=               UCA0RXBUF;  // clear RXI_F
              RXI_ENABLE;
              TXI_DISABLE;
              Flags.uart_end= 1;
            }
            UCA0TXBUF= UartBuffer[TxPointer++];
            break;
  }  
}


  void  date_time(void)
{  
  if(Seconds++ == 59)
  {
    Seconds= 0;
    if(Minutes++ == 59)
    {
       Minutes= 0;
       if(Houres++ == 23)
       {
          Houres= 0;
          if(Days++ == MonthSize[Monthes-1])
          {
             Days= 1;
             if(Monthes++ == 12)
             {
                Monthes= 1;
                if(Years++ == 99) Years= 0;
             }
          }
       }
    }
  }
}
