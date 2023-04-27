#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "gpio.h"


#define P_LED P_LED_R

static int counter;

void switch_ISR(int status){
	char temp[10];
	gpio_toggle(P_LED);
	counter++;
	sprintf(temp, "%d \r\n", counter);
	uart_print(temp);
}

void led_ISR(int status){
	const char* temp = gpio_get(P_LED) ? "The led is now on!": "The led is now off!";
	uart_print(temp);
}


int main(){
	
	char next, AEM_last_digit;
	
	uart_init(115200);
	uart_enable();
	
	//setting modes to the LED and the button pin
	gpio_set_mode(P_LED, Output);
	gpio_set_mode(P_SW, Input);
	
	gpio_set_callback(P_SW, switch_ISR);
	gpio_set_callback(P_LED,led_ISR);
	
	while(1){
		uart_print("Please enter your AEM: \r\n");
		while ((next = uart_rx()) != '\r')
			AEM_last_digit = next;

		
		gpio_set(P_LED, (AEM_last_digit % 2));	
		
	}

	return 0;
}