#include "ioCC2530.h"

#define D5 (P1_3)
#define SW1 (P1_2)
#define uchar unsigned char
#define uint unsigned int

void delay(uint time)
{
  uint i;
  uchar j;
  for(i=0;i<time;i++)
    for(j=0;j<240;j++)
    {
      asm("NOP");
      asm("NOP");
      asm("NOP");
    }
}

void main(void)
{
  P1SEL &= ~0x0C;
  P1DIR |= 0x08;
  P1DIR &= ~0x04;
  D5 = 0;
  
  while(1)
  {
    if(SW1 == 0)
    {
      delay(100);
      if(SW1 == 0)
      {
        D5 = 1;
        delay(4000);
        D5 = 0;
      }
    }
  }
}