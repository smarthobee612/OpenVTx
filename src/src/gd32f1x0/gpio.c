#include "Arduino.h"
#include "gd32f1x0_gpio.h"
#include "gd32f1x0_rcu.h"


void pinMode(uint32_t pin, uint8_t type)
{
    uint32_t gpio_periph = GPIO_BASE + 0x400 * GPIO2PORT(pin);
    uint32_t gpio_pin = 0x1 << GPIO2IDX(pin);

    /* enable the clock */
    rcu_periph_clock_enable(RCU_REGIDX_BIT(IDX_AHBEN, (17U + GPIO2PORT(pin))));

    /* configure the port */
    if (type == OUTPUT || type & ALTERNATE) {
        if (type & ALTERNATE) {
            gpio_mode_set(gpio_periph, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio_pin);
            gpio_af_set(gpio_periph, AF((type & 0x7F)), gpio_pin);
        } else {
            gpio_mode_set(gpio_periph, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, gpio_pin);
        }
        gpio_output_options_set(gpio_periph, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                                gpio_pin);
        /* clear pin */
        if (type == OUTPUT)
            GPIO_BC(gpio_periph) = gpio_pin;
    } else if (type == ANALOG) {
        gpio_mode_set(gpio_periph, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, gpio_pin);
    } else {
        gpio_mode_set(gpio_periph, GPIO_MODE_INPUT,
                      (type == INPUT) ? GPIO_PUPD_NONE : GPIO_PUPD_PULLUP,
                      gpio_pin);
    }
}

void digitalWrite(uint32_t pin, uint8_t val)
{
    uint32_t gpio_periph = GPIO_BASE + 0x400 * GPIO2PORT(pin);
    uint32_t gpio_pin = 0x1 << GPIO2IDX(pin);
    if (val)
        GPIO_BOP(gpio_periph) = gpio_pin;
    else
        GPIO_BC(gpio_periph) = gpio_pin;
}
