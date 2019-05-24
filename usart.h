#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdio.h>

void DEBUG_init();
void DEBUG_send_byte(char data);
char DEBUG_recv();
void DEBUG_print(const char *data);

void USART0_init();
void USART0_send_byte(char data);
char USART0_recv();
void USART0_print(const char *data);

/* Flush WIFI USART and put up to len chars in buff.
 * TODO: flushed_len is for debugging, remove later */
void USART0_flush(char *buff, size_t len, size_t *flushed_len);

#endif // USART_H_
