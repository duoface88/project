#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include <sys/stat.h>
#include <pthread.h>
#include "button.h"
#define INPUT_DEVICE_LIST "/dev/input/event 5"
//실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음., ex)/dev/input/event5
#define PROBE_FILE "/proc/bus/input/devices "
//PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알수 있다"는 바로 그 파일
#define MESSAGE_ID 112
static int returnValue =0;
int main(void)
{
/*int fp;
int readSize,inputIndex;
char inputDevPath[200]={0,};
if ( probeButtonPath(inputDevPath) == 0)
{
printf ("ERROR! File Not Found!\r\n");
printf ("Did you insmod?\r\n");
return 0;
}
printf ("inputDevPath: %s\r\n", inputDevPath);
fp = open(inputDevPath, O_RDONLY);*/
printf("시작 버튼을 누르시오\n\r");
buttonInit();    //쓰레드생성 및 메시지큐 설정
BUTTON_MSG_T msgRx;     //메시지 recieve구조체
int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);//메시지큐
//printf("여기까지왓다");
while(1)
{
//usleep(1000*50);
//printf("stEvent는 : %d\n\r",stEvent.type); 
//printf("stEvent는 : %d\n\r",stEvent.type); 
returnValue=msgrcv(msgID,&msgRx,8,0,IPC_NOWAIT);
//printf("리턴밸류는 : %d\n\r",returnValue); //입력받으면 리턴밸류 4된다
//원래 if(returnValue>0)
//다음 if((msgRx.keyInput>0)&&(msgRx.pressed>0))

if(returnValue>0)
{
	
//printf("msgRx.keyInput는 %d이고 msgRx.pressed는 %d이다\n\r",msgRx.keyInput,msgRx.pressed);

switch(msgRx.keyInput)
{
case KEY_VOLUMEUP: printf("Volume up key:"); break;
case KEY_HOME: printf("Home key:"); break;
case KEY_SEARCH: printf("Search key:"); break;
case KEY_BACK: printf("Back key:"); break;
case KEY_MENU: printf("Menu key:"); break;
case KEY_VOLUMEDOWN: printf("Volume down key:"); break;
}
if(msgRx.pressed)
printf("pressed\n\r");
else printf("relesed\n");
//printf("두번째 msgRx.keyInput는 %d이고 msgRx.pressed는 %d이다\n\r",msgRx.keyInput,msgRx.pressed);
msgRx.keyInput=0;
}
else
;

} 
return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
/*buttonInit();    //쓰레드생성 및 메시지큐 설정
BUTTON_MSG_T msgRx;     //메시지 recieve구조체
int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);//메시지큐
printf("여기까지왓다");
//int i=1;
while(1)
{
msgrcv(msgID,&msgRx,sizeof(int),0,IPC_NOWAIT);
switch(msgRx.keyInput)
{
case KEY_VOLUMEUP: printf("Volume up key:"); break;
case KEY_HOME: printf("Home key:"); break;
case KEY_SEARCH: printf("Search key:"); break;
case KEY_BACK: printf("Back key:"); break;
case KEY_MENU: printf("Menu key:"); break;
case KEY_VOLUMEDOWN: printf("Volume down key:"); break;
}

//usleep(1000*1000);
//i=0;
}

buttonExit();
return 0;
}*/
