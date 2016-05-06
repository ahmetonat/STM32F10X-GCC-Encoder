
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#define LED_PORT GPIOB
#define LED_PIN GPIO_Pin_9

void LED_ON(void); //Turn on the LED
void LED_OFF(void); //Turn off
void init_LED_GPIO (void); //Initialize the LED ports.
