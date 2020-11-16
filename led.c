#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "led.h"

static int fd = 0;
static int ledStatus = 0;
int ledLibInit(void){
    fd = open(LED_DRV_NODE, O_RDWR);
    if(fd < 0){
        printf("device open error\r\n");
        exit(1);
    }
    ledStatus = 0;
    ledLibRaw(ledStatus);
    return 1;
}

int ledLibOnOff(int ledNumber, int OnOff){
    int lamp = 1;
    lamp = lamp << ledNumber; // 00010000 해당하는 led bit만 1로 바꿈
    ledStatus = (ledStatus & (~lamp)); // 해당하는 led bit만 0으로 바꿈
    if(OnOff) ledStatus = ledStatus | lamp; // 해당하는 led bit만 1로 바꾸고 나머지는 그대로
    ledLibRaw(ledStatus);
    return 1;
}

int ledLibStatus(void){
    return ledStatus;
}

int ledLibExit(void){
    ledLibRaw(0);
    close(fd);
}

int ledLibRaw(char ledraw){
    if(fd > 0){
        write(fd, &ledraw, 1);
    }
    else{
        printf("wrong! you must ioen device node!\r\n");
        exit(2);
    }
    ledStatus = ledraw;
    return 1;
}

int ledLibBlink(int ledNumber, int nth, int msec){
    ledLibMorseBlink(ledNumber, nth, msec, msec);
}

int ledLibMorseBlink(int ledNumber, int nth, int msecON, int msecOFF){
    int i = 0;
    for(i = 0; i < nth; i++){
        ledLibOnOff(ledNumber, 1); // ON
        usleep(1000*msecON);
        ledLibOnOff(ledNumber, 0); // OFF
        usleep(1000*msecOFF);
    }
}