// Copyright (c) Matthew Garman.  All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for
// license information.


#include "mcu-relay-controller-iface.h"


// hardware design:
// the microcontroller should have at least four general purpose IO pins for
// the folloing:
//     - status indicator LED
//     - momentary switch
//     - relay coil 1
//     - relay coil 2


// global constants
#define RELAY_SETTLE_TIME_MS 5
#define SWITCH_DEBOUNCE_TIME_MS 75
#define OFF 0
#define ON 1

// the relay has two states, which we'll call ON or OFF
volatile uint8_t relay_state = OFF;

void relay_activate()
{
    MRC_relay_coil_pin1_set_high();
    MRC_relay_coil_pin2_set_low(); // should already be low
    MRC_sleep_millisecs(RELAY_SETTLE_TIME_MS);
    MRC_relay_coil_pin1_set_low();
    relay_state = ON;
}

void relay_deactivate()
{
    MRC_relay_coil_pin2_set_high();
    MRC_relay_coil_pin1_set_low(); // should already be low
    MRC_sleep_millisecs(RELAY_SETTLE_TIME_MS);
    MRC_relay_coil_pin2_set_low();
    relay_state = OFF;
}

void relay_toggle()
{
    if (relay_state == OFF) { relay_activate();  }
    else                    { relay_deactivate(); }
}

int main(int argc, char* argv[])
{
    MRC_hardware_init();

    while (1)
    {
        MRC_disable_interrupts();
        MRC_disable_sleep();

        if (0 == MRC_get_switch_pin_state())
        {
            relay_toggle();
            MRC_led_toggle();
            MRC_sleep_millisecs(SWITCH_DEBOUNCE_TIME_MS);
        }

        MRC_enable_interrupts();
        MRC_enter_sleep_mode();
    }

    return 0;
}
