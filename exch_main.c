#include  "define.h"

Int8U  hex2char(Int16U pointer)
{
  Int8U a, b;
     
  a= UartBuffer[pointer++] - 48;
  if(a > 9) a-= 7;
           
  b= UartBuffer[pointer] - 48;
  if(b > 9) b-= 7;
           
  return((a << 4) | b);
}

Int16U  hex2int(Int16U pointer)
{
  return((hex2char(pointer)<<8) | hex2char(pointer+2));
}

Int16U  char2hex(Int8U cnum)
{
  Int8U  a,b;
      
  a= (cnum & 0x0f);
  b= cnum>>4;
      
  if(a > 9) a+= 55;
  else      a+= 48;
          
  if(b > 9) b+= 55;
  else      b+= 48;
          
  return(a | (b<<8));
}


// check sum 
Int8U check_sum(Int16U len)
{
  Int8U    crc=0;
  Int16U   i;
     
  for(i= 0; i < len; i++) crc+= UartBuffer[i];
  return crc;
}

void  hexbuffer_2_binbuffer(pInt8U hexBuffer, pInt8U binBuffer, Int16U nBytes, Int16U Pointer)
{
  Int16U i;
  
  for(i= 0; i < nBytes; i++, Pointer+= 2) binBuffer[i]= hex2char(Pointer);
}

void  binbuffer_2_hexbuffer(pInt8U hexBuffer,Int8U __data20* binBuffer,Int16U nBytes,Int16U Pointer)
{
  Int16U i, codedbyte;
  
  for(i= 0; i < nBytes; i++) 
  {
    codedbyte=  char2hex(binBuffer[i]);
    hexBuffer[Pointer++]= codedbyte >> 8;
    hexBuffer[Pointer++]= codedbyte;
  }
}

void  uart_process(void)
{
  Int8U       num;
  Int16U      adr, d, c, command;
    
    Flags.uart_recieve=  0;
    
  // UART mode
  if( (hex2int(1) != NetAdr) && (hex2int(1) != SERVICE_ADR) ) { RxCounter=0; RXI_ENABLE; return; }
  if(check_sum(RxCounter-2) != hex2char(RxCounter-2))         { RxCounter=0; RXI_ENABLE; return; }
  
  RxCounter=  0;
  adr=        hex2int(7);
  num=        hex2char(11);
  command=    UartBuffer[5]*256 + UartBuffer[6];
       

  UartBuffer[0]=  '!';
  c=              7;
  
  switch(command) 
  {   
    case 'WR': if((num > MAX_DATA_LEN) || ((adr + num) > RAMSIZE) ) goto fail;
               hexbuffer_2_binbuffer(UartBuffer, (pInt8U)(RamBeginAddr + adr), num, 13);
               goto final;               
               
    case 'RI': if(num > MAX_DATA_LEN) goto fail;
               binbuffer_2_hexbuffer(UartBuffer,(Int8U __data20 *)(adr), num, 7);
               c= 7 + num*2;
               goto final;
               
    case 'RR': if(num > MAX_DATA_LEN) goto fail;
               binbuffer_2_hexbuffer(UartBuffer,(Int8U __data20 *)(RamBeginAddr + adr), num, 7);
               c= 7 + num*2;
               goto final;
               
    case 'IR': for(; SerialNumberStr[c-7]; )  
               { 
                 UartBuffer[c]= SerialNumberStr[c-7]; 
                 c++; 
               } 
               UartBuffer[c++]=' ';
               for(; Version[c-16]; )  
               { 
                 UartBuffer[c]= Version[c-16]; 
                 c++; 
               }
               goto final; 

               
fail:
     default:  UartBuffer[0]= '?'; 
final:
               d= char2hex(check_sum(c)); 
               UartBuffer[c]=   d>>8; 
               UartBuffer[c+1]= d; 
               UartBuffer[c+2]= 0x0d;
  }  
  TX_START;
}
