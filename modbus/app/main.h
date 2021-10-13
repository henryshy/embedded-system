#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"

#define usart_baud 9600
#define local_addr 0x01

extern volatile uint32_t tx_flag;

extern volatile uint8_t rx_rec[50];
extern volatile uint8_t rx_rec_length;
extern volatile uint8_t rx_rec_cnt;

extern volatile uint8_t tx_rec[50];
extern volatile uint8_t tx_rec_length;
extern volatile uint8_t tx_rec_cnt;

extern volatile uint32_t* in_reg[100];//����Ĵ���,����32λ�Ĵ����ĵ�ַ
extern volatile uint32_t* out_reg[100];//����Ĵ���,����32λ�Ĵ����ĵ�ַ
extern volatile uint32_t* hold_reg[100];//���ּĴ���,����32λ�Ĵ����ĵ�ַ

extern volatile uint32_t in_testdata1;//��ʼ���Ĵ�����������
extern volatile uint32_t in_testdata2;
extern volatile uint32_t in_testdata3;

extern volatile uint32_t out_testdata1;
extern volatile uint32_t out_testdata2;
extern volatile uint32_t out_testdata3;
extern volatile uint32_t out_testdata4;

extern volatile uint32_t hold_testdata1;
extern volatile uint32_t hold_testdata2;
extern volatile uint32_t hold_testdata3;
extern volatile uint32_t hold_testdata4;
extern volatile uint32_t hold_testdata5;

void  clear_rec(void);
#endif




