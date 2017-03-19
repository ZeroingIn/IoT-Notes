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

void relay(unsigned char status)
{
  P2SEL&= ~0x01;     // ���ü̵�������Ϊ��ͨIO�� 1111 1110
  P2DIR|= 0x01 ;    // ���ü̵������ſ��ƶ�Ϊ���0000 0010
  P2_0 = status;               
}

void main(void)
{
    halBoardInit();//ѡ�ֲ����ڴ˺�������Ӵ���
    ConfigRf_Init();//ѡ�ֲ����ڴ˺�������Ӵ���
    
    P1SEL &= ~0x1C;
    P1DIR |= 0x08;
    P1DIR &= ~0x04;
    
    P1_3 = 1; //LED
    
    
    uint8 buf[3] = " ";
    unsigned char flag = 0;

    while(1)
    {
    /* user code start */
      if(MY_ADDR == 0x1015)  //�ж��Ƿ�Ϊ����
      {
        uint8 buffer[2];
        halUartRead(buffer,2);
        delay(1000);
        if(buffer[0]=='#'&&buffer[1]=='1')
        { 
          
          relay(1);
          P1_3 = 1;
          halUartWrite(buffer,2);
          buffer[0] = 0;
          buffer[1] = 0;
        }
        else if(buffer[0]=='#'&&buffer[1]=='0')
        {
          relay(0);
          P1_3 = 0;
          halUartWrite(buffer,2);
          buffer[0] = 0;
          buffer[1] = 0;
        }
      }
      
      if(SW1 == 0 && MY_ADDR == 0xAC3A) //�ж��Ƿ�Ϊ�ӽڵ�
      {
        delay(100);
        if(SW1 == 0 && flag == 0)
        {
          buf[0] = 0xFE;
          buf[1] = 0x01; //������
          basicRfSendPacket(SEND_ADDR,buf,2);
          delay(2000);
          
          if(SW1 == 0)
          {
            buf[0] = 0xFE;
            buf[1] = 0x00; //������
            basicRfSendPacket(SEND_ADDR,buf,2);
            flag = 1;   //������־λ
            continue;
          }
        }
        while(SW1)  //while��λ�úܹؼ� �÷���continue����֮���λ��
          flag = 0; //�ȴ������ɿ���������־λ��λ
      }
      
      
      if(basicRfPacketIsReady())
      {
        basicRfReceive(buf, 2, NULL);
        if(buf[0] == 0xFE)
        {
          if(buf[1] == 0x01)
          {
            relay(1);
            P1_3 = 1;
          }
          if(buf[1] == 0x00)
          {
            relay(0);
            P1_3 = 0;
          }
        }
     }
    /* user code end */
    }
}