#include  "define.h"

void  choose_uart_speed(void)
{
  switch(ConfigWord & SPEED_MASK)
  {
    case    S9600:    UCA0BRW=  6;  UCA0MCTL= (13 << 4) + UCOS16; break;
    case    S19200:   UCA0BRW=  3;  UCA0MCTL= (1 << 1) + (6 << 4) + UCOS16; break;
     
    default:         UCA0BRW=  6;  UCA0MCTL= (13 << 4) + UCOS16;          // 9600 
  } 
}

void  ini(void)
{
  __disable_interrupt(); 
  
  SFRIE1= SFRIFG1= SYSCTL= 0;         // всяку дребедень выкл
  SFRRPCR=  BIT2 + BIT3;              // ресетный резистор вкл
  
    // WDT
  WDTCTL=   WDTPW + WDTSSEL__VLO + WDTCNTCL + WDTIS__32K;    // ~ 2.4c
  
    // PMM
  PMMCTL0=  PMMPW;                    // Vcore = 1,4 В
  SVSMHCTL= SVSHE + SVSHRVL0;         // вкл только супервизор на напр. около 2 В
  SVSMLCTL= SVSLE;                    // вкл только супервизор на напр. 1,4 В
  PMMRIE=   SVSHPE + SVSLPE;          // вкл ресет от супервизоров H и L
  
    // Ports init  
  
  PMAPKEYID=  PMAPKEY;

  PMAPCTL=    0;
  P1MAP5=     PM_UCA0RXD;
  P1MAP6=     PM_UCA0TXD;
    
  P2MAP0=     PM_CBOUT1;
  CBCTL3=     CBPD1 + CBPD2;
  P2MAP3=     31;
  P2MAP5=     31;
    
  P3MAP4=     PM_TA0CCR3A;
  PMAPKEYID=  0;
  
  //P1SEL= 0x60;
  //P1DIR= 0xff;
  P1IE= P1DS= P1REN= 0;
  P1DIR= 0x02;
  P1OUT= 0x00;
  
  P2SEL= 0x2f;
  P2OUT= 0;
  P2IE= P2DS= P2REN= 0;
  P2DIR= 0xef;
  
  P3SEL= 0;
  P3REN= 0;
  P3DIR= 0xff; 
  P3OUT= 0;
  
  P5SEL=  0;    
  P5OUT=  P5DS= P5REN= 0; 
  P5DIR=  0xff;
  
  PJOUT= 0;
  PJDS= PJREN= 0;
  PJDIR=  255;  

 
 
  
  UCSCTL1=  DCORSEL_1;                                        // 0.5...3.5 MHz
  UCSCTL2=  31;                                               // 32,768*(31+1) ~ 1MHz на выходе FLL
  UCSCTL3=  FLLREFDIV_0 + SELREF__REFOCLK;                    // 32kHz на вход FLL

  UCSCTL5=  DIVA__32;                                         // ACLK = XT1/32 = 1024 Hz
  UCSCTL8=  0;                                                // запретить тактирование по требованию

   // ждем генератор DCO
  while(UCSCTL7 & DCOFFG) UCSCTL7 &= ~DCOFFG; 
  SFRIFG1 &= ~OFIFG; 
  
  UCSCTL4=    SELM__DCOCLKDIV + SELS__DCOCLKDIV + SELA__REFOCLK;    // MCLK - FLL(1MHz), SMCLK - FLL(1MHz), ACLK - XT1/32(1024Hz)
  
  // TA0
  TA0CTL=     TASSEL_1 + MC__UP + TACLR; //ACLK
  TA0CCTL0=   OUTMOD_0 + OUT;  
  TA0CCR3=    500;
  
  // TA1 inclk = ACLK (1024Hz)
  TA1CTL=     TASSEL_1 + MC__UP + TACLR + TAIE;  // clk= ACLK прерывания настраиваются по режимам
  TA1CCTL0=   TA1CCTL1= TA1CCTL2= 0;
  TA1CCR0=    51; // первое через 0,05с
  
  //UART

  UCA0CTL1=    UCSWRST + UCSSEL1; 
  UCA0CTL0= 0;  
  UCA0STAT=    0x00;                            // enable error symbol set int flag ?
  choose_uart_speed();
  UCA0CTL1=    UCSSEL1;
  UCA0IE =     UCRXIE;
  
  //Radio
     // Increase PMMCOREV level to 2 for proper radio operation
  SetVCore(2);                            
  RF1AIFERR &= ~BIT0;
  ResetRadioCore();     
  InitRadio();
 // InitButtonLeds();
  ReceiveOff();
  ReceiveOn(); 
  receiving = 1; 
  transmitting = 0;
  P1OUT= 0x02;


 // Инициализация переменных из сегмента RAM  

  if((Seconds > 59) || (Minutes > 59) || (Houres > 23)  || (Days > 31) || (Monthes > 12) || (Years < 12) || (Years > 99) )
  {
    Seconds= Minutes= Houres= Rclock= 0;
    Days= Monthes= 4;
    Years = 12;
  }

}