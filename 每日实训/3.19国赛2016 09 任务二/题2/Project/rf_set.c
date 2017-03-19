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
#define MY_ADDR                   0x1015     //本机模块地址
#define SEND_ADDR                 0xAC3A     //发送地址
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
  P2SEL&= ~0x01;     // 设置继电器出口为普通IO口 1111 1110
  P2DIR|= 0x01 ;    // 设置继电器引脚控制端为输出0000 0010
  P2_0 = status;               
}

void main(void)
{
    halBoardInit();//选手不得在此函数内添加代码
    ConfigRf_Init();//选手不得在此函数内添加代码
    
    P1SEL &= ~0x1C;
    P1DIR |= 0x08;
    P1DIR &= ~0x04;
    
    P1_3 = 1; //LED
    
    
    uint8 buf[3] = " ";
    unsigned char flag = 0;

    while(1)
    {
    /* user code start */
      if(MY_ADDR == 0x1015)  //判断是否为主机
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
      
      if(SW1 == 0 && MY_ADDR == 0xAC3A) //判断是否为从节点
      {
        delay(100);
        if(SW1 == 0 && flag == 0)
        {
          buf[0] = 0xFE;
          buf[1] = 0x01; //有险情
          basicRfSendPacket(SEND_ADDR,buf,2);
          delay(2000);
          
          if(SW1 == 0)
          {
            buf[0] = 0xFE;
            buf[1] = 0x00; //险情解除
            basicRfSendPacket(SEND_ADDR,buf,2);
            flag = 1;   //长按标志位
            continue;
          }
        }
        while(SW1)  //while的位置很关键 得放在continue跳出之后的位置
          flag = 0; //等待长按松开按键将标志位复位
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