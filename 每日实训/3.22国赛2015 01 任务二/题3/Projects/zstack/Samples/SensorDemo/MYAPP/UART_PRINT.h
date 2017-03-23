

#ifndef uart_PRINT_H
#define uart_PRINT_H
#define UartWrite(buf,len) (HalUARTWrite(HAL_UART_PORT_0,(buf),(len)))

uint16 RecvUartData(uint8 *uRxData, uint16 uRxlen);
uint16 RecvUart1Data(uint8 *uRxData, uint16 uRxlen);
extern  void	uart_datas(void *fmt,uint16 len);
extern  void	uart_enter(void);
extern  void	uart_printf(unsigned char *fmt,...);
extern  uint16  uart_read(uint8 *buf, uint16 maxlen);
extern  void	printf_str( char *buf, char *fmt,... );
extern uint8   lrc_checksum(uint8* buf, uint8 len);
#endif