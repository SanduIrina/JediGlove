#include <util/delay.h>
#include "usart.h"

// baud rates in Table 19-12

void DEBUG_init()
{
	// baud rate 115200
	UBRR1H = 0;
	UBRR1L = 103;

	// receptie si transmisie
	UCSR1B = (1<<TXEN1) | (1<<RXEN1) | (1<<RXCIE1);

	// 1 bit de stop
	UCSR1C &= ~(1 << USBS1);

	// 8 biti de date
	UCSR1C |= (2 << UCSZ10);
}

void DEBUG_send_byte(char data)
{
	while (!(UCSR1A & (1 << UDRE1)));

	UDR1 = data;

	while (!(UCSR1A & (1 << UDRE1)));
}

char DEBUG_recv()
{
	while (!(UCSR1A & ( 1<< RXC1)));

	return UDR1;
}

void DEBUG_print(const char *data)
{
	while (*data != '\0')
		DEBUG_send_byte(*data++);
}

/*
 * Functie de initializare a controllerului USART
 */
void USART0_init()
{
	UBRR0H = 0;
	UBRR0L = 103;

	/* porneste transmitatorul */
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

	// 1 bit de stop
	UCSR0C &= ~(1 << USBS0);

	// 8 biti de date
	UCSR0C |= (2 << UCSZ00);
}

/*
 * Functie ce transmite un caracter prin USART
 *
 * @param data - caracterul de transmis
 */
void USART0_send_byte(char data)
{
    /* asteapta pana bufferul e gol */
    while(!(UCSR0A & (1<<UDRE0)));

    /* pune datele in buffer; transmisia va porni automat in urma scrierii */
    UDR0 = data;

    while(!(UCSR0A & (1<<UDRE0)));
}

/*
 * Functie ce primeste un caracter prin USART
 *
 * @return - caracterul primit
 */
char USART0_recv()
{
    /* asteapta cat timp bufferul e gol */
    while(!(UCSR0A & (1<<RXC0)));

    /* returneaza datele din buffer */
    return UDR0;
}

/*
 * Functie ce transmite un sir de caractere prin USART
 *
 * @param data - sirul (terminat cu '\0') de transmis
 */
void USART0_print(const char *data)
{
    while(*data != '\0')
	    USART0_send_byte(*data++);
}

void USART0_flush(char *buff, size_t len, size_t *flushed_len)
{
	if (flushed_len != NULL)
		*flushed_len = 0;
	char dummy;
	size_t idx = 0;
	while (UCSR0A & (1<<RXC0)) {
		dummy = UDR0;
		if (idx < len)
			buff[idx++] = dummy;

		if (flushed_len != NULL)
			++(*flushed_len);

		_delay_ms(1);
	}
}
