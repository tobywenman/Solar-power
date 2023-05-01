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
}

void mppt(float maxCurrent,float maxVoltage,PIO pio,uint sm){

    static float prevPower = 0;
    static bool peturbUp = false;

    adc_select_input(0);
    float readCurrent = ((float) adc_read())*0.0007324;
    adc_select_input(1);
    float readVoltage = ((float)adc_read())*0.000982;

    float measuredPower = readCurrent*readVoltage;
    
    printf("power %f\n", measuredPower);
    printf("current %f\n",readCurrent);
    printf("voltage %f\n",readVoltage);

    if (readVoltage > maxVoltage){
        setVoltage(pio,sm,maxVoltage);
    }
    else if (readCurrent < 0.1){
        shiftWiper(0,false,pio,sm);
    }
    else if (readCurrent > maxCurrent){
        shiftWiper(0,true,pio,sm);
    }
    else if(measuredPower > prevPower){
        shiftWiper(0,peturbUp,pio,sm);
        prevPower = measuredPower;
    }
    else{
        peturbUp = !peturbUp;
        prevPower = measuredPower;
        shiftWiper(0,peturbUp,pio,sm);
    }
}

int main(){
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_select_input(1);
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
    float vOut;

    while(1){
        mppt(1,3.65,pio,sm);
        sleep_ms(100);
    }
}

