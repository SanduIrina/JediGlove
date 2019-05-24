/*
* ATmega16_WIFI
* http://www.electronicwings.com
*
*/


#define F_CPU 16000000UL			/* Define CPU Frequency e.g. here its Ext. 12MHz */
#include <avr/io.h>					/* Include AVR std. library file */
#include <util/delay.h>				/* Include Delay header file */
#include <stdbool.h>				/* Include standard boolean library */
#include <string.h>					/* Include string library */
#include <stdio.h>					/* Include standard IO library */
#include <stdlib.h>					/* Include standard library */
#include <avr/interrupt.h>			/* Include avr interrupt header file */
// #include "USART_RS232_H_file.h"		/* Include USART header file */
#include "adxl345/adxl345.h"		/* Include accelerometer header file */
#include "usart.h"
#define SREG    _SFR_IO8(0x3F)

#define DEFAULT_BUFFER_SIZE		160
#define DEFAULT_TIMEOUT			10000

/* Connection Mode */
#define SINGLE					0
#define MULTIPLE				1

/* Application Mode */
#define NORMAL					0
#define TRANSPERANT				1

/* Application Mode */
#define STATION							1
#define ACCESSPOINT						2
#define BOTH_STATION_AND_ACCESPOINT		3

/* Define Required fields shown below */
#define DOMAIN				"192.168.3.117"
#define PORT				8888
#define API_WRITE_KEY		"C7JFHZY54GLCJY38"
#define CHANNEL_ID			"119922"

#define SSID				"acasalanoi"
#define PASSWORD			"4c4s4lan013b1n3s1frum0s"

#define AT					"AT\r\n"
#define MODE				"AT+CW_MODE=3\r\n"
#define MUX					"AT+CIPMUX=0\r\n"
#define CIPSTART			"AT+CIPSTART=\"TCP\",\"192.168.1.1\",8888\r\n"
#define CIPSEND				"AT+CIPSEND=1\r\n"
#define SENDL				"L"
#define SENDl				"l"
#define SENDR				"R"
#define SENDr				"r"
#define LEFT				1
#define RIGHT				2
#define BACK				3
#define FRONT				4

char last_cmd = 0;
int PANIC = 0;

ISR(USART1_RX_vect){
	uint8_t oldsrg = SREG;
	cli();
	PANIC = 1;
	SREG = oldsrg;
}

int main(void)
{
	char test[40];
	int16_t ax, ay, az, pitch, roll;
	ax = 0;
	ay = 0;
	az = 0;
	/* Read data from accelerometer test */
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	USART0_init();
	DEBUG_init();
	sei();
	adxl345_init();
	// USART1_Init(0);
	int16_t ayy = ay;
	int16_t axx = ax;
	int b = 0, f = 0, r = 0, l = 0;
	int old_f = 0, old_s = 0, new_f = 0, new_s = 0;
	int last_cmd;
	DDRA |= (1 << PA0);
	DDRB |= (1 << PB0) | (1 << PB1);
	DDRD |= (1 << PD7);
	int i = 0;
	for(int j=0; j<5; j++){
		PORTB |= (1 << PB0);
		_delay_ms(120);
		PORTB &= !(1 << PB0);
		PORTB |= (1 << PB1);
		_delay_ms(120);
		PORTB &= !(1 << PB1);
		PORTA |= (1 << PA0);
		_delay_ms(120);
		PORTA &= !(1 << PA0);
	}
	for(;;){
		// if(PANIC != 0){
		// 	for(int j=0; j<5; j++){
		// 		PORTB |= (1 << PB0);
		// 		_delay_ms(120);
		// 		PORTB &= !(1 << PB0);
		// 		PORTB |= (1 << PB1);
		// 		_delay_ms(120);
		// 		PORTB &= !(1 << PB1);
		// 		PORTA |= (1 << PA0);
		// 		_delay_ms(120);
		// 		PORTA &= !(1 << PA0);
		// 	}
		// 	PANIC = 1;
		// }
		i++;
		adxl345_getdata(&ax, &ay, &az);
		if(ay == 0){
			b++;
		}else if(ay > 0){
			f++;
		}
		if(f + b >= 150){
			if(f > b){
				if(old_f == BACK){
					last_cmd = 'F';
					// sprintf(test, "F");
					for(int k=0; k<6; k++){
						USART0_send_byte('F');	
					}
					// _delay_ms(20);
					// USART0_SendString('F');
					// USART0_SendString('F');
					PORTD |= (1 << PD7);
				}
				old_f = FRONT;
			} else if( b  > f + 50){
				if(old_f == FRONT){
					last_cmd = 'H';
					for(int k=0; k<6; k++){
						USART0_send_byte('H');	
					}
					PORTD &= !(1 << PD7);
				}
				old_f = BACK;
			}
			f = 0;
			b = 0;
		}
		// X axis
		if(ax == 0){
			l++;
		} else if(ax > 0){
			r++;
		}
		if(r + l >= 150){
			if(r > l){
				if(old_s == LEFT){
					last_cmd = 'R';
					for(int k=0; k<6; k++){
						USART0_send_byte('R');	
					}
					PORTB &= !(1 << PB0);
					PORTA |= (1 << PA0);
				}
				old_s = RIGHT;
			} else if( l > r){
				if(old_s == RIGHT){
					last_cmd = 'L';
					sprintf(test, "L");
					for(int k=0; k<6; k++){
						USART0_send_byte('L');	
					}
					// _delay_ms(100);
					// USART0_SendString(test);
					// _delay_ms(100);
					// USART0_SendString(test);
					PORTA &= !(1 << PA0);
					PORTB |= (1 << PB0);
				}
				old_s = LEFT;
			}
			l = 0;
			r = 0;
		}
			
		// _delay_ms(10);
		// if(ay < 2000){
		// 	USART0_SendString("L\n");
		// }
		// if(ay > 4000){
		// 	USART0_SendString("R\n");
		// }
		// _delay_ms(200);
	}
}
