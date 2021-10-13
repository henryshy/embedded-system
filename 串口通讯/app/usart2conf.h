#ifndef _USART2CONF_H

#define _USART2CONF_H

#define DATASIZE 24
#define MAXSIZE  50

extern volatile unsigned int flag;
extern volatile uint8_t rec[MAXSIZE];
extern volatile unsigned int length;

void USART2_configuration(int baud);

#endif

