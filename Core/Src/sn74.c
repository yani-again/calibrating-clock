#include "sn74.h"
#include <assert.h>


#define SER_HIGH(gpiox, pin)        (gpiox->BSRR = (1U << (pin)))
#define SER_LOW(gpiox, pin)         (gpiox->BSRR = (1U << ((pin) + 16)))
#define SRCLK_HIGH(gpiox, pin)      (gpiox->BSRR = (1U << (pin)))
#define SRCLK_LOW(gpiox, pin)       (gpiox->BSRR = (1U << ((pin) + 16)))
#define RCLK_HIGH(gpiox, pin)       (gpiox->BSRR = (1U << (pin)))
#define RCLK_LOW(gpiox, pin)        (gpiox->BSRR = (1U << ((pin) + 16)))


/*
 * @brief Writes the passed 8-bit data to all SN74's without latching it.
 *
 * @param Unsigned 8-bit digit to write to the register's outputs.
 */
void sn74_write(simple_gpio_typeDef* gpiox, uint8_t ser, uint8_t srclk, uint8_t value)
{
    for (uint8_t i = 0; i < 8; ++i)
    {
        if (value & (1U << i)) SER_HIGH(gpiox, ser); else SER_LOW(gpiox, ser);
        SRCLK_HIGH(gpiox, srclk);
        SRCLK_LOW(gpiox, srclk);
    }

    return;
}

void sn74_latch(simple_gpio_typeDef* gpiox, uint8_t rclk)
{
    RCLK_HIGH(gpiox, rclk);
    RCLK_LOW(gpiox, rclk);

    return;
}
