#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include "button.h"
// first read input device
#define INPUT_DEVICE_LIST "/dev/input/event5" //실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음., ex)/dev/input/event5
#define PROBE_FILE "/proc/bus/input/devices" //PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알수 있다"는 바로 그 파일
#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
#define HAVE_TO_FIND_2 "H: Handlers=kbd event"
#define MESSAGE_ID 112

char buttonPath;
static int fd=0;
int msgID=0;
static pthread_t buttonTh_id;

int buttonInit(void)
{
//if (probeButtonPath(buttonPath) == 0)
//return 0;
//fd=open (buttonPath, O_RDONLY);  //노드 열기
fd=open (INPUT_DEVICE_LIST, O_RDONLY);

msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);//메시지큐
pthread_create(&buttonTh_id, NULL, buttonThFunc, NULL);//쓰레드 생성 buttonFunc 실행
return 1;
}

int buttonExit(void)
{
	pthread_cancel(&buttonTh_id); //생성한 쓰레드 종료시킴
	close(fd);   //닫기
}

int probeButtonPath(char *newPath)
{
int returnValue = 0; //button에 해당하는 event#을 찾았나?
int number = 0; //찾았다면 여기에 집어넣자
FILE *fp = fopen(PROBE_FILE,"rt"); //파일을 열고
#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
#define HAVE_TO_FIND_2 "H: Handlers=kbd event"
while(!feof(fp)) //끝까지 읽어들인다.
{
char tmpStr[200]; //200자를 읽을 수 있게 버퍼
fgets(tmpStr,200,fp); //최대 200자를 읽어봄
//printf ("%s",tmpStr);
if (strcmp(tmpStr,HAVE_TO_FIND_1) == 0)
{
printf("YES! I found!: %s\r\n", tmpStr);
returnValue = 1; //찾음
}
if (
(returnValue == 1) && //찾은 상태에서
(strncasecmp(tmpStr, HAVE_TO_FIND_2,
strlen(HAVE_TO_FIND_2)) == 0) //Event??을 찾았으면
)
{
printf ("-->%s",tmpStr);
printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
number = tmpStr[strlen(tmpStr)-3] - '0';
//Ascii character '0'-'9' (0x30-0x39)
//to interger(0)
break;
}
}
fclose(fp);
if (returnValue == 1)
sprintf (newPath,"%s%d",INPUT_DEVICE_LIST,number);
return returnValue;
}

static void *buttonThFunc(void*a)     //버튼눌리거나 버튼 릴리즈될때마다 메세지보내는 쓰레드함수
{
	BUTTON_MSG_T msgTx;                    //메시지 전달구조체
	struct input_event stEvent;     //버튼 누르고 떼고 입력값받는구조체
	msgTx.messageNum = 1.0;         //롱인트에 아무 양수입력
	//msgTx.keyInput =0; //넣어야되나

	while(1)
	{
		
		read(fd,&stEvent,16);
		//printf("타입=%d, 코드=%d, 밸류=%d\n\r",stEvent.type,stEvent.code,stEvent.value);

		//printf("%d이건밸류  %d이건 코드 다음은타입=%d\n\r",stEvent.value,stEvent.code,stEvent.type);
		//printf("EV_KEY=%d\n\r",EV_KEY);
		//if((stEvent.type==EV_KEY)&&(stEvent.value>0))    //키가눌리면
		if(stEvent.type)
		{
			//printf("타입=%d, 코드=%d, 밸류=%d\n\r",stEvent.type,stEvent.code,stEvent.value);
			msgTx.keyInput=stEvent.code;   //값 메시지구조체에 입력
			msgTx.pressed=stEvent.value;   //눌리거나 떼어진 값 메시지구조체에 입력
			msgsnd(msgID,&msgTx,8,0);
		}
		//키가 릴리즈되었을때도 코딩필요
		else
		;
	
		
		//printf("안변함\n\r");
	}
}
