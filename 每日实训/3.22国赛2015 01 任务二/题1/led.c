#include "ioCC2530.h"

#define D5 (P1_3)
#define SW1 (P1_2)

unsigned char flag = 0;

void delay(unsigned int time)
{
  unsigned int i;
  unsigned char j;
  for(i=0;i<time;i++)
    for(j=0;j<240;j++)
    {
      asm("NOP");
      asm("NOP");
      asm("NOP");
    }
}

void initP1()
{
  IEN2 |= 0x10;
  P1IEN |= 0x04;
  PICTL |= 0x02;
  EA = 1;
}

#pragma vector = P1INT_VECTOR
__interrupt void P1INT(void)
{
  if(P1IFG & 0x04)
  {
    flag = !flag;
    P1IFG &= ~0x04;
  }
  P1IF = 0;
}

void main(void)
{
  initP1();
  
  P1SEL &= ~0x0C;
  P1DIR |= 0x08;
  P1DIR &= ~0x04;
  
  D5 = 0;
  
  while(1)
  {  
    if(flag)
    {
      D5 = !D5;
      delay(2000);
    }
    else
      D5 = 0;
  }
}