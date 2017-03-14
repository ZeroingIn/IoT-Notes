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
#define PAN_ID                    0x8006     //����id 
#define MY_ADDR                   0xAC3A     //����ģ���ַ
#define SEND_ADDR                 0x1015     //���͵�ַ
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
  //��ʹ���ж� ��������������޷�ѭ��
  if(P1IFG & 0x04)
  {
    basicRfSendPacket(SEND_ADDR,sendBuf,3);
    P1IFG &= ~0x04; //�����α�־λ������
  }
  P1IF = 0; //�����α�־λ������
}

void main(void)
{
    halBoardInit();//ѡ�ֲ����ڴ˺�������Ӵ���
    ConfigRf_Init();//ѡ�ֲ����ڴ˺�������Ӵ���

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
            isLight = 1;  //���ڵ㲻�ɱ����� ������ѭ�� 
            //isLight = ~isLight; //�����ڵ�ɱ����� ����ת
          }
        }
      }
    /* user code end */
    }
}