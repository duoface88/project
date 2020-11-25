#include <stdio.h>
#include "led.h"

int main(void){
    
    ledLibInit();

    ledLibRaw(0xFF);
    sleep(1);
    ledLibRaw(0xAA);
    sleep(1);
    ledLibRaw(0x55);
    sleep(1);
    ledLibRaw(0x00);
    
    int i  = 0;
    for(i = 0; i < 8; i++){
        ledLibOnOff(i , 1);
        sleep(1);
        ledLibOnOff(i , 0);
        sleep(1);
    }

    ledLibBlink(5, 10, 500);

    ledLibMorseBlink(2, 3, 500, 500);
    ledLibMorseBlink(2, 3, 1500, 500);
    ledLibMorseBlink(2, 3, 1500, 500);

    sleep(1);
    ledLibExit();
}

