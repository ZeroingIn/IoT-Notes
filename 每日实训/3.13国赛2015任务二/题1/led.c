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

void initT1(void)
{
  T1CTL |= 0x0C;  //128
  T1CC0L = 0x12;  //0.25s
  T1CC0H = 0x7A;
  T1IE = 1;       //使能定时器1中断
  T1OVFIM = 1;    //使能定时器1溢出中断
  EA = 1;         //使能总中断
}

#pragma vector = T1_VECTOR
__interrupt void T1_INT(void)
{
  T1STAT &= ~0x20;  //clear timer1 overflow sign
  if(++flag == 4)
  {
    D5 = 0;
    flag = 0;
  }
}

void main(void)
{
  P1SEL &= ~0x0C;
  P1DIR |= 0x08;
  P1DIR &= ~0x04;
  
  initT1();   //不要忘了初始化
  D5 = 0;
  
  while(1)
  {  
    if(SW1 == 0)
    {
      delay(100);
      if(SW1 == 0)
      {
        T1CTL |= 0x03;  //采用正计数模式 开始工作
        D5 = 1;
        /*//延时还是有问题 写4000约2秒
        D5 = 1;
        delay(4000);
        D5 = 0;*/
      }
    }
  }
}