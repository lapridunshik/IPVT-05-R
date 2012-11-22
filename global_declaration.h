// predeclaration
extern volatile Flags_t           Flags;

extern Int8U                  UartBuffer[];
extern U8                     Seconds, Minutes, Houres, Days, Monthes, Years;
extern Int16U                 RxCounter, TxPointer;
extern unsigned char          Strobe(unsigned char strobe);
extern unsigned char          ReadSingleReg(unsigned char addr);

extern const Int16U           ConfigWord, NetAdr;
extern const U8               Version[], SerialNumberStr[], MonthSize[];

extern unsigned int           SetVCore         (unsigned char level);
extern unsigned int           SetVCoreUp       (unsigned char level);
extern unsigned int           SetVCoreDown     (unsigned char level);

void                          uart_process(void);
void                          date_time(void);
void                          ini(void);
void                          radio(void);

void                          ResetRadioCore (void);
void                          WriteRfSettings(RF_SETTINGS *pRfSettings);
void                          WriteSingleReg(unsigned char addr, unsigned char value);
void                          WriteBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count);
void                          ReadBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count);
void                          WriteSinglePATable(unsigned char value);
void                          WriteBurstPATable(unsigned char *buffer, unsigned char count); 
void                          Transmit(unsigned char *buffer, unsigned char length);
void                          ReceiveOn(void);
void                          ReceiveOff(void);

void                          InitButtonLeds(void);
void                          InitRadio(void);

extern unsigned char          packetReceived;
extern unsigned char          packetTransmit; 

extern unsigned char          RxBuffer[PACKET_LEN+2];
extern unsigned char          RxBufferLength;
extern unsigned char          TxBuffer[PACKET_LEN];
extern Int16U                 Rclock,Rclock_main;
extern Int16S                 clock_err[256];
     

extern unsigned char          transmitting; 
extern unsigned char          receiving; 