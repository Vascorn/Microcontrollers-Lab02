#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "gpio.h"
#include "leds.h"

#define P_LED P_LED_R

static int counter;
static uint8_t last_digit_AEM;

void switch_ISR(int status){
		char temp[100];
		gpio_toggle(P_LED);
		
		counter++;
	
		if (gpio_get(P_LED))
			sprintf(temp, "\r\nButton pressed, Led is on. Counter = %d \r\n", counter);
		else
			sprintf(temp, "\r\nButton pressed, Led is off. Counter = %d \r\n", counter);
		uart_print(temp);
		uart_print("Insert your AEM \r\n");
}



void getchar_ISR(uint8_t c){
	
	
	if (c != '\r'){
		last_digit_AEM = c;
	  uart_tx(c);
	}
	else{
		uart_print("\r\n");
		gpio_set(P_LED, last_digit_AEM % 2);
		if (gpio_get(P_LED))
			uart_print("Odd AEM, Led is on\r\n");
		else
			uart_print("Even AEM, Led is off \r\n");
		uart_print("Insert your AEM \r\n");
	}
		
}

int main(){
	__enable_irq();
	
	uart_init(115200);
	uart_enable();
	uart_print("Insert your AEM \r\n");
	uart_set_rx_callback(getchar_ISR);
	
	//setting modes to the LED and the button pin
	leds_init();
	
	gpio_set_mode(P_SW, PullDown);
	gpio_set_trigger(P_SW,Rising);
	
	gpio_set_callback(P_SW, switch_ISR);
	
	NVIC_SetPriority(EXTI15_10_IRQn, 1);
	

	while(1){
			//wait for interrupt
	}

	return 0;
}