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

void setVoltage(PIO pio, uint sm, float voltage){
    float wiperPos = 2 - 3.936/voltage;
    int steps = (int)(wiperPos*100);
    if (steps > 100){
        steps = 100;
    }
    else if (steps < 0){
        steps = 0;
    }
    resetWiper(pio,sm);
    shiftWiper(steps,false,pio,sm);
    printf("voltage: %f\n",voltage);
    printf("steps %d\n",steps);
    printf("wiper pos %f\n",wiperPos);
}

void updateCurrent(float current,PIO pio,uint sm){
    adc_select_input(0);
    float readCurrent = ((float) adc_read())*0.0007324;

    if (readCurrent < current) {
        shiftWiper(0,false,pio,sm);
    }
    else{
        shiftWiper(0,true,pio,sm);
    }
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

    float voltage = 2.5;

    uint adcRaw;
    float voltDrop;

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
            printf("voltage up\n");
            shiftWiper(0,false,pio,sm);
            break;
        case 'd':
            printf("voltage down\n");
            shiftWiper(0,true,pio,sm);
            break;
        case 'o':
            adcRaw = adc_read();
            voltDrop = (float)(adcRaw);
            printf("voltage drop raw %f\n",voltDrop);
            voltDrop *= 0.0007324;
            printf("voltage drop %f\n",voltDrop);

            printf("adc value %u\n",adcRaw);
            

            break;
        default:
            break;
        }
        //setVoltage(pio,sm,voltage);
    }
}

