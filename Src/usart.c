#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"


uint8_t USART1_TX_BUF[256];
uint8_t USART1_RX_BUF[256];
uint32_t USART1_RX_LEN=0;
uint8_t USART1_RX_STA =0;
/* for MicroLIB */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART3 and Loop until the end of transmission */
	usart_data_transmit(USART0, (uint8_t)ch);
	while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
	return ch;
}

/* for GNU */
int _write (int fd, char *pBuffer, int size)
{ 
	for (int i = 0; i < size; i++)
	{
		usart_data_transmit(USART0, (uint8_t)pBuffer[i]);
	  while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
	}
	return size;
}

void usart0_config(void)
{
		/* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}
/*
PA2:        USART1_TX
PA3:        USART1_RX
*/          
void usart1_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART1);
    
    gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2);
    gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_3);
    
    usart_deinit(USART1);
    usart_baudrate_set(USART1,115200U);
    usart_receive_config(USART1,USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1,USART_TRANSMIT_ENABLE);
    nvic_irq_enable(USART1_IRQn,0,5);
    usart_interrupt_enable(USART1,USART_INT_RBNE);
    usart_enable(USART1);
    
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u1_printf(char* fmt,...)  
{  
	uint16_t i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{  
		usart_data_transmit(USART1,USART1_TX_BUF[i]);
        while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));        
	} 
}
void usart1_sendata(uint8_t *pdata,uint32_t len)
{
    for(uint8_t i=0;i<len;i++)
    {
        usart_data_transmit(USART1,pdata[i]);
        while(RESET ==usart_flag_get(USART1,USART_FLAG_TBE));
    }
}
void USART1_IRQHandler(void)
{
    if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE)!=RESET)
    {
        usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE);
        if(USART1_RX_LEN ==0)
        {
            timer_enable(TIMER5);
        }
        USART1_RX_BUF[USART1_RX_LEN++]=usart_data_receive(USART1);
        timer_counter_value_config(TIMER5,0);
    }
}
//发送单个字节
void USART1_SendOneByte(uint8_t val)
{
    usart_data_transmit(USART1,val);
}
//向EC600发送字符串
void EC600_ATSendStirng(char *str)
{
    memset(USART1_RX_BUF,0,256);
    USART1_RX_STA =0;
    while(*str)
    {
        USART1_SendOneByte(*str++);
    }  
}
