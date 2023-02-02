#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pot.pio.h"

#define LED_PIN PICO_DEFAULT_LED_PIN

void shiftWiper(uint8_t steps, bool upDown,PIO pio, uint sm){
    if (upDown){
        steps |= 0b10000000;
    }
    pio->txf[sm] = steps;
}

void resetWiper(PIO pio, uint sm){
    shiftWiper(100,false,pio,sm);
}

int main(){

    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    bool lit = false;
    uint wiper = 0;

    PIO pio = pio0;
    int sm = 0;

    uint offset = pio_add_program(pio, &pot_program);

    pot_program_setup(pio,sm,offset,0,1,100000);
    pio_sm_set_enabled(pio,sm,true);
    resetWiper(pio,sm);

    while(1){
        gpio_put(LED_PIN,lit);
        lit = !lit;
        if (wiper < 100){
            shiftWiper(0,true,pio,sm);
            wiper+=1;
        }
        else{
            wiper = 0;
            resetWiper(pio,sm);
        }
        
        sleep_ms(1);
    }
}

