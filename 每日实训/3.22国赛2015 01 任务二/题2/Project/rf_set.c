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
/*****��Ե�ͨѶ��ַ����******/
#define RF_CHANNEL                11         // Ƶ�� 11~26
#define PAN_ID                    0x8087     //����id 
#define MY_ADDR                   0x1015     //����ģ���ַ
#define SEND_ADDR                 0xAC3A     //���͵�ַ
/**************************************************/
static basicRfCfg_t basicRfConfig;
// ����RF��ʼ��
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
#define SW1 (P1_2)
#define D5 (P1_3)

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

void initP1(void)
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
    uint8 buf[1] = 'X';
    basicRfSendPacket(SEND_ADDR,buf,1);
    P1IFG &= ~0x04;
  }
  P1IF = 0;
}

void main(void)
{
    halBoardInit();//ѡ�ֲ����ڴ˺�������Ӵ���
    ConfigRf_Init();//ѡ�ֲ����ڴ˺�������Ӵ���
    
    P1SEL &= ~0x1C;
    P1DIR |= 0x08;
    P1DIR &= ~0x04;
    
    initP1();
    
    D5 = 0;
    
    while(1)
    {
    /* user code start */
      if(MY_ADDR == 0xAC3A) //�ж��Ƿ�Ϊ�ӻ�
      {
        if(basicRfPacketIsReady())
        {
          uint8 buf[1];
          basicRfReceive(buf, 1, NULL);
          if(buf[0] == 'X')
            flag = !flag;
        }
        if(flag)
        {
          D5 = !D5;
          delay(2000);
        }
        else 
          D5 = 0;
      }
    /* user code end */
    }
}