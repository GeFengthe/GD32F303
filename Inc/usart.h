#ifndef usart_H
#define usart_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "gd32f30x.h"
#include <stdio.h>

extern uint8_t USART1_RX_BUF[256];
extern uint8_t USART1_RX_STA;
extern uint32_t USART1_RX_LEN;

void usart0_config(void);
void usart1_config(void);
extern void u1_printf(char* fmt,...);
extern void usart1_sendata(uint8_t *pdata,uint32_t len);
extern void EC600_ATSendStirng(char *str);
#ifdef __cplusplus
}
#endif

#endif
