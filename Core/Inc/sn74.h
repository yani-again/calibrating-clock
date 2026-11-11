#ifndef SN74_H
#define SN74_H


#include <stdint.h>
#include "simple_stm32g0xx.h"


void sn74_write(simple_gpio_typeDef* gpiox, uint8_t ser, uint8_t srclk, uint8_t value);
void sn74_latch(simple_gpio_typeDef* gpiox, uint8_t rclk);


#endif
