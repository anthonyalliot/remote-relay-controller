// Copyright (c) Matthew Garman.  All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for
// license information.


#include "mcu-relay-controller-iface.h"
#include "hardware-details/attiny.h"


// hardware design:
// the microcontroller should have at least four general purpose IO pins for
// the folloing:
//     - momentary switch - configure as INPUT with internal pullup resistor
//     - status indicator LED - configure as output
//     - relay coil 1 - configure as output
//     - relay coil 2 - configure as output


void relay_activate(void) // aka "set"
{
    MRC_relay_coil_pin1_set_high();
    MRC_sleep_millisecs(RELAY_SETTLE_TIME_MS);
    MRC_relay_coil_pin1_set_low();
}

void relay_deactivate(void) // aka "reset"
{
    MRC_relay_coil_pin2_set_high();
    MRC_sleep_millisecs(RELAY_SETTLE_TIME_MS);
    MRC_relay_coil_pin2_set_low();
}

uint8_t debounce_switch(void)
{
    uint8_t state = (LOW == MRC_switch_pin_get_state() ? 1 : 0);

    for (   uint8_t i=0 ;
            ((i<MAX_N_SWITCH_DEBOUNCE_READS) &&
             (SWITCH_DEBOUNCE_TARGET != state)) ;
            ++i )
    {
        state  = (uint8_t)(state << 1);
        state |= (LOW == MRC_switch_pin_get_state() ? 1 : 0);
        state &= SWITCH_DEBOUNCE_TARGET;
        MRC_sleep_millisecs(1);
    }

    return SWITCH_DEBOUNCE_TARGET == state;
}

// blink the status LED a few times
void led_greeting(void)
{
    for (uint8_t i=0; i<4; ++i)
    {
        MRC_led_pin_set_high();
        MRC_sleep_millisecs(250);
        MRC_led_pin_set_low();
        MRC_sleep_millisecs(250);
    }
}

int main(int argc, char* argv[])
{
    // initialize hardware
    MRC_hardware_init();

    // just for fun
    led_greeting();

    // make relay and status indicator LED consistent consistent with the
    MRC_relay_coil_pin2_set_low();
    MRC_relay_coil_pin1_set_low();
    relay_deactivate();
    MRC_led_pin_set_low();

    while (1)
    {
        MRC_disable_interrupts();
        MRC_disable_sleep();

        uint8_t switch_pressed = debounce_switch();
        MRC_switch_pin_clear_int_flags();
        if (switch_pressed)
        {
            relay_activate();
            MRC_led_pin_set_high();
        }
        else
        {
            relay_deactivate();
            MRC_led_pin_set_low();
        }
        MRC_sleep_millisecs(SWITCH_DEBOUNCE_TIME_MS);

        MRC_enable_interrupts();
        MRC_enter_sleep_mode();
    }

    return 0;
}
