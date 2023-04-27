#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "gpio.h"


#define P_LED P_LED_G

static int counter;
static uint8_t last_digit_AEM;

void switch_ISR(int status){
	if (status & 1 << GET_PIN_INDEX(P_SW)){
		char temp[10];
		gpio_toggle(P_LED);
		counter++;
		sprintf(temp, "%d \r\n", counter);
		uart_print(temp);
	}
}

void led_ISR(int status){
	if (status & 1 << GET_PIN_INDEX(P_LED)){
		const char* temp = gpio_get(P_LED) ? "The led is now on!": "The led is now off!";
		uart_print(temp);
	}
}

void getchar_ISR(uint8_t c){
	if (c != '\r')
		last_digit_AEM = c;
	else{
		gpio_set(P_LED, last_digit_AEM % 2);
	}
		
}

int main(){
	
	uart_init(115200);
	uart_enable();
	uart_print("Insert your AEM \r\n");
	uart_set_rx_callback(getchar_ISR);
	
	//setting modes to the LED and the button pin
	gpio_set_mode(P_LED, Output);
	gpio_set_mode(P_SW, Input);
	
	gpio_set_callback(P_SW, switch_ISR);
	gpio_set_callback(P_LED,led_ISR);
	
	NVIC_SetPriority(EXTI15_10_IRQn, 1);
	
	
	while(1){
			//wait for interrupt
	}

	return 0;
}