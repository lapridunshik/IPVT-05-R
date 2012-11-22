#include  <msp430.h>
#include  "msp430_comm.h"
#include  "short_types.h"
#include  "sd.h"
#include  "limits.h"
#include  "math.h"
#include  "stdio.h"
#include  "stdlib.h"
#include  "stdarg.h"
#include  "icclbutl.h"

// config's bits
#define   SPEED_MASK        BIT0
#define   S115200           0
#define   S9600             BIT0
#define   S19200            BIT1
#define   S38400            (BIT0+BIT1)

#define RXI_ENABLE              UCA0IE|=   UCRXIE
#define RXI_DISABLE             UCA0IE&=  ~UCRXIE
#define TXI_DISABLE             UCA0IE&=  ~UCTXIE
#define TX_START                { UCA0STAT=  UCLISTEN; UCA0IFG|= UCTXIFG; UCA0IE|= UCTXIE; }
#define WDRCLR                  WDTCTL=  WDTPW + WDTSSEL__VLO + WDTCNTCL + WDTIS__8192

#define MAX_DATA_LEN          128
#define UART_BUF_LEN          272 
#define RAMSIZE               1536
#define RamBeginAddr          (Int16U)&Seconds

#define SERVICE_ADR           0xFFFF

#define PMM_STATUS_OK         0
#define PMM_STATUS_ERROR      1


#define  PACKET_LEN         (0x05)			// PACKET_LEN <= 61
#define  RSSI_IDX           (PACKET_LEN)    // Index of appended RSSI 
#define  CRC_LQI_IDX        (PACKET_LEN+1)  // Index of appended LQI, checksum
#define  CRC_OK             (BIT7)          // CRC_OK bit 
#define  PATABLE_VAL        (0x51)          // 0 dBm output 


typedef struct 
{ 
   Int16U    systick:       1;   
   Int16U    uart_recieve:  1;
   Int16U    uart_end:      1;
   Int16U    radio:         1;
   Int16U    radio_receive: 1;
   Int16U    count:         1;
   
} Flags_t;

typedef struct S_RF_SETTINGS {
    unsigned char fsctrl1;   // Frequency synthesizer control.
    unsigned char fsctrl0;   // Frequency synthesizer control.
    unsigned char freq2;     // Frequency control word, high byte.
    unsigned char freq1;     // Frequency control word, middle byte.
    unsigned char freq0;     // Frequency control word, low byte.
    unsigned char mdmcfg4;   // Modem configuration.
    unsigned char mdmcfg3;   // Modem configuration.
    unsigned char mdmcfg2;   // Modem configuration.
    unsigned char mdmcfg1;   // Modem configuration.
    unsigned char mdmcfg0;   // Modem configuration.
    unsigned char channr;    // Channel number.
    unsigned char deviatn;   // Modem deviation setting (when FSK modulation is enabled).
    unsigned char frend1;    // Front end RX configuration.
    unsigned char frend0;    // Front end RX configuration.
    unsigned char mcsm0;     // Main Radio Control State Machine configuration.
    unsigned char foccfg;    // Frequency Offset Compensation Configuration.
    unsigned char bscfg;     // Bit synchronization Configuration.
    unsigned char agcctrl2;  // AGC control.
    unsigned char agcctrl1;  // AGC control.
    unsigned char agcctrl0;  // AGC control.
    unsigned char fscal3;    // Frequency synthesizer calibration.
    unsigned char fscal2;    // Frequency synthesizer calibration.
    unsigned char fscal1;    // Frequency synthesizer calibration.
    unsigned char fscal0;    // Frequency synthesizer calibration.
    unsigned char fstest;    // Frequency synthesizer calibration control
    unsigned char test2;     // Various test settings.
    unsigned char test1;     // Various test settings.
    unsigned char test0;     // Various test settings.
    unsigned char fifothr;   // RXFIFO and TXFIFO thresholds.
    unsigned char iocfg2;    // GDO2 output pin configuration
    unsigned char iocfg0;    // GDO0 output pin configuration
    unsigned char pktctrl1;  // Packet automation control.
    unsigned char pktctrl0;  // Packet automation control.
    unsigned char addr;      // Device address.
    unsigned char pktlen;    // Packet length.
} RF_SETTINGS;


#include "global_declaration.h"