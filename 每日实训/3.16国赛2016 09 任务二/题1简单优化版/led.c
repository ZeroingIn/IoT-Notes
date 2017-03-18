#include "ioCC2530.h"

#define SW1 (P1_2)
#define D3 (P1_0)
#define D4 (P1_1)
#define D5 (P1_3)
#define D6 (P1_4)

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

void main(void)
{
  init();
  
  D5 = 1;
  delay(1000);
  D5 = 0;
  delay(1000);
  D5 = 1;
  
  while(1)
  {
    if(SW1 == 0)
    {
      delay(100);
      if(SW1 == 0 && flag == 0) //加了一个标志位 判断之前是否长按
      {
        D5 = 1;
        delay(1000);
        D5 = 0;
        delay(1000);
        D5 = 1;
        
        if(SW1 == 0)
        {
          D5 = 0;
          D6 = 0;
          D3 = 0;
          D4 = 0;
          flag = 1; //长按标志位
          continue;
        }
        
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
      while(SW1)  //精巧的设计 等待长按按键松开
        flag = 0; //松开按键时 将长按标志位复位
    }
  }
  
}