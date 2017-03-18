#include "ioCC2530.h"

#define SW1 (P1_2)
#define D3 (P1_0)
#define D4 (P1_1)
#define D5 (P1_3)
#define D6 (P1_4)

unsigned int flag = 0;

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

void init(void)
{
  P1SEL &= ~0x1F;
  P1DIR |= 0x1B;
  P1DIR &= ~0x04;
  D5 = 0;
  D6 = 0;
  D3 = 0;
  D4 = 0;
}

void initT1(void)
{
  T1CTL |= 0x0C;
  T1CC0L = 0xD4;
  T1CC0H = 0x30;
  T1IE = 1;
  T1OVFIM = 1;
  T1CTL |= 0x01;
}

#pragma vector = T1_VECTOR
__interrupt void T1INT(void)
{
  T1STAT &= ~0x20;
  flag++;
}

int SWSwitch(void)
{
  EA = 1; //使能中断 定时器开始工作
  if(SW1 == 0)
  {
    while(1)
    {
      if(flag<10)
      {
        flag = 0;
        EA = 0;
        return 1; //1为按键按下
      }
      if(flag >= 10)
      {
        flag = 0;
        EA = 0;
        return 2; //2为长按按键
      }
    }
  }
  else
    flag = 0;
    EA = 0;
    return 0; //0为按键未按下
}

void main(void)
{
  init();
  initT1();
  
  D5 = 1;
  delay(1000);
  D5 = 0;
  delay(1000);
  D5 = 1;
  
  while(1)
  {
    if(SWSwitch() == 1)
    {
      D5 = 1;
      delay(1000);
      D5 = 0;
      delay(1000);
      D5 = 1;
      
      D6 = 1;
      delay(1000);
      D6 = 0;
      delay(1000);
      D6 = 1;
      
      D3 = 1;
      delay(1000);
      D3 = 0;
      delay(1000);
      D3 = 1;
      
      D4 = 1;
      delay(1000);
      D4 = 0;
      delay(1000);
      D4 = 1;
    }
    else if(SWSwitch() == 2)
    {
      D5 = 0;
      D6 = 0;
      D3 = 0;
      D4 = 0;
    }
  }
}