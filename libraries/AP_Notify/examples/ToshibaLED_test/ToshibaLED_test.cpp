// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: t -*-

#include <AP_HAL/AP_HAL.h>
#include <AP_Notify/AP_Notify.h>          // Notify library
#include <AP_Notify/ToshibaLED.h>

const AP_HAL::HAL& hal = AP_HAL::get_HAL();

#if CONFIG_HAL_BOARD == HAL_BOARD_PX4
static ToshibaLED_PX4 toshiba_led;
#else
static ToshibaLED_I2C toshiba_led;
#endif

void setup(void)
{
    // display welcome message
    hal.console->print_P(PSTR("Toshiba LED test ver 0.1\n"));

    // initialise LED
    toshiba_led.init();

    // check if healthy
    if (!toshiba_led.healthy()) {
        hal.console->print_P(PSTR("Failed to initialise Toshiba LED\n"));
    }

    // turn on initialising notification
    AP_Notify::flags.initialising = false;
    AP_Notify::flags.save_trim = true;
    AP_Notify::flags.gps_status = 1;
    AP_Notify::flags.armed = 1;
    AP_Notify::flags.pre_arm_check = 1;
}

void loop(void)
{
    // blink test
    //hal.console->print_P(PSTR("Blink test\n"));
    //blink();
    /*
    // full spectrum test
    hal.console->print_P(PSTR("Spectrum test\n"));
    full_spectrum();
    */

    // update the toshiba led
    toshiba_led.update();

    // wait 1/50th of a second
    hal.scheduler->delay(20);
}

// full_spectrum - runs through the full spectrum of colours the led can display
void full_spectrum()
{
    // go through the full range of colours but only up to the dim light level
    for (uint8_t red=0; red<=0x05; red++) {
        for (uint8_t green=0; green<=0x05; green++) {
            for (uint8_t blue=0; blue<=0x05; blue++) {
                toshiba_led.set_rgb(red,green,blue);
                hal.scheduler->delay(5);
            }
        }
    }
}

#define LED_DIM 0x11

// blink - blink the led at 10hz for 10 seconds
void blink()
{
    // set colour to red
    toshiba_led.set_rgb(LED_DIM,0,0);

    // full spectrum test
    for (uint8_t c=0; c<=2; c++ ) {
        if (c==0) {
            toshiba_led.set_rgb(LED_DIM,0,0);   // red
        }else if (c==1) {
            toshiba_led.set_rgb(0,LED_DIM,0);   // green
        }else{
            toshiba_led.set_rgb(0,0,LED_DIM);   // blue
        }
    }
}

AP_HAL_MAIN();
