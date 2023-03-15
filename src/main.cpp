#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pot.pio.h"
#include "hardware/adc.h"

#define LED_PIN PICO_DEFAULT_LED_PIN
#define BUCK_ENABLE 12
#define UP_DOWN 7
#define INC 8
#define CS 10

void shiftWiper(uint8_t steps, bool upDown,PIO pio, uint sm){
    if (upDown){
        steps |= 0b10000000;
    }
    pio->txf[sm] = steps;
}

void resetWiper(PIO pio, uint sm){
    shiftWiper(100,true,pio,sm);
}

int main(){
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(BUCK_ENABLE, GPIO_OUT);
    gpio_set_dir(CS, GPIO_OUT);
    bool lit = false;
    uint wiper = 0;

    PIO pio = pio0;
    int sm = 0;

    uint offset = pio_add_program(pio, &pot_program);

    pot_program_setup(pio,sm,offset,INC,UP_DOWN,100000);
    pio_sm_set_enabled(pio,sm,true);
    resetWiper(pio,sm);

    gpio_put(BUCK_ENABLE,1);
    gpio_put(CS,0);
    
    char inp;
    bool power = true;

    while(1){
        
        // if (wiper < 100){
        //     shiftWiper(0,false,pio,sm);
        //     wiper+=1;
        // }
        // else{
        //     wiper = 0;
        //     resetWiper(pio,sm);
        // }

        printf("Voltage up/down/toggle power/r u/d/t/r:\n");

        inp = getchar();

        switch (inp)
        {
        case 'u':
            shiftWiper(0,false,pio,sm);
            printf("voltage up\n");
            break;
        case 'd':
            shiftWiper(0,true,pio,sm);
            printf("voltage down\n");
            break;
        case 't':
            power = !power;
            gpio_put(BUCK_ENABLE,power);
            gpio_put(LED_PIN,power);
            printf("power toggled: %d\n",power);
            break;
        case 'r':
            resetWiper(pio,sm);
            printf("wiper reset\n");
            break;
        default:
            break;
        }
        
    }
}

