#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h" 
#include "sensor_drv/sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x8006     //网络id 
#define MY_ADDR                   0xAC3A     //本机模块地址
#define SEND_ADDR                 0x1015     //发送地址
/**************************************************/
static basicRfCfg_t basicRfConfig;
// 无线RF初始化
void ConfigRf_Init(void)
{
    basicRfConfig.panId       =   PAN_ID;
    basicRfConfig.channel     =   RF_CHANNEL;
    basicRfConfig.myAddr      =   MY_ADDR;
    basicRfConfig.ackRequest  =   TRUE;
    while(basicRfInit(&basicRfConfig) == FAILED);
    basicRfReceiveOn();
}

/********************MAIN************************/
#include "ioCC2530.h"
#define D5 (P1_3)
#define D6 (P1_4)
#define SW1 (P1_2)

uint8 recBuf[3] = " ";
uint8 sendBuf[3] = "New";

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

void initLED(void)
{
  P1SEL &= ~0x1C;
  P1DIR |= 0x18;
  P1DIR &= ~0x04;

  D5 = 0;
  D6 = 0;
}

void lightLED(void)
{
  D5 = 1;
  delay(5000);
  D6 = 1;
  D5 = 0;
  delay(5000);
  D6 = 0;
}

void initP1Break(void)
{
  IEN2 |= 0x10;
  P1IEN |= 0x04;
  PICTL |= 0x02;
  EA = 1;
}

#pragma vector = P1INT_VECTOR
__interrupt void P1_INT(void)
{
  //不使用中断 消抖不彻底则会无法循环
  if(P1IFG & 0x04)
  {
    basicRfSendPacket(SEND_ADDR,sendBuf,3);
    P1IFG &= ~0x04; //清两次标志位！！！
  }
  P1IF = 0; //清两次标志位！！！
}

void main(void)
{
    halBoardInit();//选手不得在此函数内添加代码
    ConfigRf_Init();//选手不得在此函数内添加代码

    initLED();
    initP1Break();

    unsigned char isLight = 0;
    
    while(1)
    {
    /* user code start */
      if(isLight)
        lightLED();
      
      if(basicRfPacketIsReady())
      {
        if(basicRfReceive(recBuf,3,NULL) == 3)
        {
          if(recBuf[0] == 'N' && recBuf[1] == 'e' && recBuf[2] == 'w')
          {
            isLight = 1;  //主节点不可被控制 即总是循环 
            //isLight = ~isLight; //非主节点可被控制 即反转
          }
        }
      }
    /* user code end */
    }
}