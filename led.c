#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdio.h>

#include "led.h"

static int fd =0;
static int ledStatus=0;
int ledLibInit(void) // 이라이브러리는 LED를 켜기전에 반드시 한번 불러주어야한다.
{
	
	fd =open(LED_DRV_NODE, O_RDWR);
	if(fd<0)
	{
		printf("Device Open Error!\r\n");
		exit(1);
	}
	ledStatus = 0;
		ledLibRaw(ledStatus);//전부끔
		return 1;

}
int ledLibOnOff(int ledNumber, int OnOff)
//이 함수는 ledNumber에 0~7을 넣으면, 그에 해당하는 led를 켜거나 끄거나 해준다.
{
	int lamp =1;
	lamp = lamp << ledNumber;                      //0001 0000해당하는 전구비트만 1임
	ledStatus = (ledStatus &(~lamp));      //해당하는 전구비트만 0으로 바꿈.
	if (OnOff)
	ledStatus = ledStatus | lamp;              //해당하는 전구비트만 1로 바꾸고 나머지는 그대로
	
	ledLibRaw(ledStatus);
	return 1;
}
int ledLibStatus(void)
//이함수를 호출하면 현재 LED상태가 어떤지 확인해 준다. 만약 모든 LED가 켜져 있으면 0xff를 리턴하고,
//만약 모든 LED가 꺼져 있으면 0x00을 리턴한다.
//만약 0xAA가 리넡되면 ,(10101010)-> LED는 4개만 켜져있는 것이다.
{
	return ledStatus;
}
int ledLibExit(void)
//이 함수를 호출하면 LED를 전부 끄고 driver를  close 한다.
{
	ledLibRaw(0);//전부끔
	close(fd);
}
int ledLibRaw(char ledwannabe)
//이 함수를 호출하면 ledStatus에 맞게 8개의 led를 전부 제어한다. 예를 들어 ledLibRaw(0x80); 1개만 켜진다
{
	if(fd>0)
	{
		//do something
		write(fd,&ledwannabe,1);
	}
	else
	{
		printf("WRONG! you must open device NODE!\r\n");
		exit(2);
	}
	ledStatus = ledwannabe;
	return 1;
}
int ledLibBlink(int ledNumber, int nth, int msec)//1~8까지의 LED를 하나 골라서 n번 깜박인이는데 , 주기가 msec이다.
{
ledLibMorseBlink(ledNumber, nth, msec, msec);
}
int ledLibMorseBlink(int ledNumber, int nth, int msecON, int msecOFF)
{
    int i = 0;
    for(i = 0; i < nth; i++){
        ledLibOnOff(ledNumber, 1); // ON
        usleep(1000*msecON);
        ledLibOnOff(ledNumber, 0); // OFF
        usleep(1000*msecOFF);
    }
}

