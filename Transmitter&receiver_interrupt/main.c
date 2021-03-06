/*!
    \file    main.c
    \brief   USART transmit and receive interrupt
    
    \version 2017-06-06, V1.0.0, firmware for GD32F3x0
    \version 2019-06-01, V2.0.0, firmware for GD32F3x0
    \version 2020-09-30, V2.1.0, firmware for GD32F3x0
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f3x0.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
//#include "gd32f350r_eval.h"

#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define TRANSMIT_SIZE   (ARRAYNUM(transmitter_buffer) - 1)

uint8_t transmitter_buffer[] = "\n\rUSART interrupt test\n\r";

uint8_t transfersize = TRANSMIT_SIZE;
uint8_t receivesize = 32;
__IO uint8_t txcount = 0; 
__IO uint16_t rxcount = 0; 
extern uint32_t recevie_data;
extern volatile bool recevie_done;
void usart0_gpio_config(void);
void usart0_config(void);

extern char receiver_buffer[32];
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 0, 0);
     
    /* initilize the com */
    usart0_gpio_config();
    usart0_config();
    
    /* enable USART TBE interrupt */  
    //usart_interrupt_enable(USART0, USART_INT_TBE);
    
    /* wait until USART send the transmitter_buffer */
//    while(txcount < transfersize);
    
    //while (RESET == usart_flag_get(USART0, USART_FLAG_TC));
    
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    
    /* wait until USART receive the receiver_buffer */
//    while(rxcount < receivesize);
//    if(rxcount == receivesize)
//        printf("\n\rUSART receive successfully!\n\r");
//        printf("recevie_data:%c",(char)recevie_data);
		
    while (1){
			
			   if(recevie_done==true)
				 {
					  
 					  uint8_t len = strlen(receiver_buffer);
			      printf("recevie_data %d:", rxcount);
					  for(uint8_t i=0; i< len; i++)
					  {
							 printf("%c", receiver_buffer[i]);
						}
						printf("\r\n");
						memset(receiver_buffer, 0, 32);
					  recevie_done = false;
						rxcount = 0;
						//usart_interrupt_enable(USART0, USART_INT_RBNE);
				 }
		}
}

/*!
    \brief      cofigure the USART0 GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usart0_gpio_config(void)
{
    /* enable COM GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_6);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_7);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_6);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_7);
}

/*!
    \brief      cofigure the USART0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usart0_config(void)
{
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

    usart_enable(USART0);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
	  
    return ch;
}
