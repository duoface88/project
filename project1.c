#include <stdint.h>
#include <string.h>
#include <unistd.h>// for open/close
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>// for O_RDWR
#include <errno.h>
#include <sys/ioctl.h>// for ioctl
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <linux/input.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <pthread.h>
#include <dirent.h>
#define MAX_SCALE_STEP	8
///////////////////////////////////////////////버튼관련

#include "button.h"				//이 헤더를 사용
#include "temp.h"					//온도센서이용
#define MESSAGE_ID 125			//메시지 아이디
static int returnValue =0;	//메시지왔는지 확인하는 변수 

//////////////////////////////////////////////////////////
/***********************************/
///////////////////  쓰레드관련 ==> //////////////////

pthread_t accth;					//움직임 측정 쓰레드
pthread_t failtxt;				//시간초과되거나 잘못입력하면 실행되는 쓰레드
pthread_t tiiid;					//자이로뒤집어졌는지판단하는쓰레드
pthread_t tiid1;					//버튼눌린지 확인하는 쓰레드
pthread_t tiid3;					//부저카운트다운쓰레드
pthread_t tiid2;					//정답 ->초록불깜빡이는 쓰레드
   int errr11;
   int errr22;
   int errr33;
   int err11; 					// button thread
   int err22; 					// acc thread
   int err33; 					// 성공시 buzzer
   int err44; 					// 성공시 txt_lcd
   int err55;
//////////////////////////////////////////////////////////
/***********************************/
//////////////369숫자관련 변수///////////////////////

   int Bcount=0; 			// Bcount1, Bcount10으로부터 count값을 정하는 변수( 각 숫자의 3 6 9의 개수 )
   int Bcount1; 				// %10을 해서 일의 자리 숫자 3, 6, 9를 찾아 저장하는 변수
   int Bcount10;				// /10을 해서 십의 자리 숫자 3, 6, 9를 찾아 저장하는 변수
   int Bigcount = 20; 	// FND에 표시될 숫자
   
//////////////////////////////////////////////////////////
/***********************************/
////////////////// 각 종 변 수 //////////////////////////

int timeover=3;				//시간이 초과되면 1로 바뀜(시간재는 for문에서 for문끝나면 1로바뀐다)
int check=0;						//옳은선택을하면 1로 바뀜(시간재는 for문에서 옳은선택하면 1로바뀐다)
int retthval=0;				//시간초과되거나 오답입력하면 쓰레드로 들어가서 알맞은 event실행시킨뒤 값을 1로 바뀌어 메인문 종료시킴
int upsidedown=0;			//자이로 센서가 뒤집어진 상태 확인 후 뒤집어지면 1로 바꾸고 종료 event 실행시킴
int Acount=3; 					// 버튼, 가속도 쓰레드로 부터 0 또는 1 또는 2값을 받아 저장하는 변수( Bcount와 비교함 )
int result1;						//1번째 2번째 가속도 차이 입력 변수
int pass = 1;						//**움직임이 다음 count에 영향미치지 않게 해주는 변수

//////////////////////////////////////////////////////////
/***********************************/

void MainInit(void)	//메인문 Init함수
{
	buzzerInit();			//부저 이닛
	buzzerEnable(0);		//부저 OFF
	ledLibInit();			//led 이닛
	pwmLedInit();			//colorled 이닛
			pwmSetPercent(20,2);		//R 
			pwmSetPercent(20,1);    	//G ==>약한 하얀색 출력 
			pwmSetPercent(20,0);		//B
	fndOff();					//fnd OFF
}
/***********************************/
void* Blinkcolorled(void *arg){				//정답일경우 초록불 잠깐 점등하는 쓰레드
		    pwmLedInit();
			pwmSetPercent(0,2);		//R
			pwmSetPercent(50,1);    //G ==>초록색 ON  (빛성분몇퍼센트, 123중하나)
			pwmSetPercent(0,0);		//B
			
				usleep(250000);		//잠시동안 불켜짐
			
			pwmSetPercent(0,2);		//R
			pwmSetPercent(0,1);    	//G ==>초록색 ON  (빛성분몇퍼센트, 123중하나)
			pwmSetPercent(0,0);		//B
}
/***********************************/
void* returnaccres(void *arg){				//키트 툭치는 이벤트 측정하는 가속도측정 쓰레드
	
//int result1;				//1번째 2번째 가속도 차이 입력 변수
int acc1 ;					//1번째 가속도
int  acc2;					//2번째 가속도

if((Acount==1)||(Acount==2))
{
	return 0;
}

while(1)
{
	
result1=0;					//결과값 초기화
acc1 = curAcel();		//1번째 가속도값 읽어온다.
usleep(300000);		//시간차
acc2 = curAcel();    	//2번째 가속도값을 읽어옴

result1 = acc2-acc1;		//1번째 2번째값의 차를 구한다.

if((result1>2000)&&(pass==1))				//가속도값의 차가 2000이 넘으며 pass가 1인경우에만(pass가 0이면 정답처리중)
	{
		printf("result = %d\n",result1); //구한 차이값 출력
		printf("moved\n");  			//움직임 출력
		Acount=0;								//**Acount를 0으로 바꿈 ==> 메인문에서 바뀐값으로 이벤트 출력
		usleep(350000);
		return NULL;							//쓰레드 종료
	}

}
}
/***********************************/
void* returntxt(void *arg){   //시간초과되거나 잘못입력되거나 키트 뒤집히면 실행되는 쓰레드
	if(timeover==1)					//시간초과로 게임종료
	{
	fndOff();								//fnd OFF
	lcdtextwrite("  time over !       ", 1);
	lcdtextwrite(" game over o_o      ", 2);
	buzzerEnable(1);
	setFrequency(dolemi(7));   //땡~~
	usleep(1000000);				
	buzzerEnable(0);					//부저 OFF
	retthval=1;							//리턴밸 1로 바꿈					
	return NULL;							//쓰레드 종료
	}
	/////////////잘못된입력 or 키트뒤집힘///////////
	fndOff();								//fnd OFF
   lcdtextwrite("     wrong !       ", 1);
   lcdtextwrite("   game over o_o   ", 2);
   	buzzerEnable(1);
	setFrequency(dolemi(7));	//땡
	usleep(1000000);
	buzzerEnable(0);					//부저OFF
   //pwmInactiveAll();// 칼라led끄기
	retthval=1;							//리턴밸 1로 바꿈	

}
/***********************************/
void* WhatGyro(void *arg)							//자이로센서를이용한 뒤집힘판단하는 쓰레드
{
	int result2;												//측정값의 차를 저장	
	int Gyro1 ;													//측정값1
	int Gyro2;													//측정값2

while(1)
	{
		Gyro1 = curGyro();			//1번째값을 읽어온다.
		usleep(1000000);				//시간차
		Gyro2 = curGyro();			//2번째값을 읽어옴
		result2 =Gyro2-Gyro1;	//값의 차를 구한다.

	if(result2>350)					//차이가 350이 넘으면
		{

			upsidedown=1;				//upsidedown변수 1로 바꿈
			printf("뒤 집 어 짐 \n");  //움직임 출력
			
			pwmSetPercent(0,0);		//
			pwmSetPercent(0,1);    	//  colorled 끄기
			pwmSetPercent(0,2);		//
			fndOff();	
			result2=0;						//자이로값 차를 0으로 초기화
			
			break;					//뒤집어지면 쓰레드 종료
		}
	}
}
/***********************************/
void* Buzzercount(void *arg)  //3..2...1. GO
{
	buzzerEnable(1);
	setFrequency(dolemi(0));
	usleep(500000);
	buzzerEnable(0);
	usleep(500000);
	//////////////////////////////삐--
	buzzerEnable(1);
	setFrequency(dolemi(0));
	usleep(500000);
	buzzerEnable(0);
	usleep(500000);
	//////////////////////////////삐--
	buzzerEnable(1);
	setFrequency(dolemi(0));
	usleep(500000);
	buzzerEnable(0);
	usleep(500000);
	//////////////////////////////삐--
	buzzerEnable(1);
	setFrequency(dolemi(7));
	usleep(1000000);
	buzzerEnable(0);
	//////////////////////////////삐이이이이--
}
/***********************************/
void* WhatButton(void *arg)						//버튼처리쓰레드
{
	//if(Acount=0)
	//{
	//	return NULL;
	//}
	buttonInit(); 											//버튼관련*설정*메시지*쓰레드 
	BUTTON_MSG_T msgRx;     							//메시지 recieve구조체
	int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);//메시지큐
	
while(1)															//반복문에서 메시지 처리
{
	if(Acount==0)											//버튼이 입력되지않더라도
	{																	//가속도쓰레드에서 Acount가 
		break; 													//바뀌면 같이 쓰레드 종료
	}

returnValue=msgrcv(msgID,&msgRx,8,0,IPC_NOWAIT);//메시지 recieve

if(returnValue)											//버튼눌리면 => 메시지 받음
{

			if (msgRx.keyInput ==KEY_MENU )	//메뉴버튼이 눌리면 게임시작
			{
				Acount=5;										//Acount =5
				printf("pushed MenuKey\n");
				break;	//쓰레드 종료
				 

			} 
			else if (msgRx.keyInput == KEY_HOME) //HOME키 눌리면 3 6 9 한개
			{
				Acount=1;										//Acount =1
				printf("pushed HomeKey\n");
				break;	//쓰레드 종료
				 
			} 
			else if (msgRx.keyInput == KEY_BACK) //Back키 눌리면 3 6 9 두개
			{
				Acount=2;										//Acount =2
				printf("pushed KEY_BACK\n");
				break;	//쓰레드종료
				 
			} 
			else
			;
}
			
	returnValue=0;		//리턴밸류 초기화
	msgRx.keyInput=0;//키입력값 초기화
}
} 
/*************************************************************/
/************************* 메 인 문 시 작 *************************/
/*************************************************************/

int main(void)
{
	// 여기서부터 lcd 관련 선언
	int screen_width;
	int screen_height;
	int bits_per_pixel;
	int line_length;
	int cols = 0, rows = 0;
	char *data;

	MainInit();			//모든 장치 INIT			//프로그램 실행

	// 여기서부터 lcd 관련
	if( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0){
		printf("fb init failed\r\n");
		return 0;
	}

	fb_clear();
	fb_write(data, cols, rows);
	close_bmp();
	fb_close();
	
	char aa[16]={ "welcome to 369"};
	char bb[16]={"                  "};
	lcdtextwrite(aa, 1);								//textlcd에 환영합니다 출력
	lcdtextwrite(bb, 2);								//두번째칸에는 빈칸 출력
	
	ledLibRaw(0xFF);
    sleep(1);
    ledLibRaw(0xAA);
    sleep(1);													 //led환영모드
    ledLibRaw(0x55);
    sleep(1);
    ledLibRaw(0x00);                             
    
    buzzerEnable(1);
	setFrequency(dolemi(0));
	usleep(500*500);
	setFrequency(dolemi(2));
	usleep(500*500);
	setFrequency(dolemi(4));						//환영buzzer
	usleep(500*500);
	setFrequency(dolemi(7));
	usleep(900*500);								
	buzzerEnable(0);
	

///////////////////////////////////////////////////////////////////////////////////////////////																			
		errr11 = pthread_create(&tiiid, NULL, &WhatGyro, NULL);		//	 
		if (errr11 != 0)																					//	=> 자이로뒤집어졌는지판단하는쓰레드
		{																											//			게임도중 키트 뒤집어질 시 게임종료시킴
			printf ("Thread Create Error: [%d]\n", errr11);				//
		}																											//
///////////////////////////////////////////////////////////////////////////////////////////////	
	
   sleep(1);														//환영 이후 잠깐의 대기
	
	char cc[16]={ "PUSH MENU=>START"};
	lcdtextwrite(cc, 1);
																		//시작하시겠습니까?
	//char dd[16]={ "Then push Menu."};
	//lcdtextwrite(dd, 2);
	
	char a[16]={ };													// 변환한 문자열을 저장할 배열
	double curtemp=temperatureout();  				//더블형 온도를 리턴함
	sprintf(a, "curtemp is %lf", curtemp);   	//실수를 문자열로 저장
	lcdtextwrite(a, 2);
	
///////////////////////////////////////////////////////////////////////////////////////////////	
		errr22 = pthread_create(&tiid1, NULL, &WhatButton, NULL);	//
		if (errr22 != 0)																					//
		{																											// => 버튼눌린지 확인하는 쓰레드
			printf ("Thread Create Error: [%d]\n", errr22);				//			메뉴버튼 눌림여부 판단위한
		}																											//				버튼쓰레드생성
///////////////////////////////////////////////////////////////////////////////////////////////
		
while(!(Acount==5))										//메뉴버튼입력 => 게임 실행
			{
					;													//버튼이 눌리면 탈출 
			}          
		
	printf("___________Game started______________\n");
	
////////////////////////게임이 시작되고 카운트다운 시작///////////////////////////////
	
	
///////////////////////////////////////////////////////////////////////////////////////////////
	errr33 = pthread_create(&tiid3, NULL, &Buzzercount, NULL); 	//
	if( err33!=0)																							//
	{																												// =>카운트다운 부저 실행
		printf("Thread Create Error: [%d]\n", err33);						// 	=>FND와 함께카운트 하기위해사용
	} 																												//
///////////////////////////////////////////////////////////////////////////////////////////////

	for(int NUMBER=3; NUMBER>0;NUMBER--)
		{
			fndDisp( NUMBER, 0);   						//카운트다운 3....2.....1...... 
			sleep(1);
		}
		
		fndOff();												//fnd OFF한뒤
		sleep(1);												//게임시작전 살짝 대기

		
////////////////////////////// 정 말 게 임 이 시 작 됨 /////////////////////////////////////

    while(1){
	
	lcdtextwrite("  **In game**   ", 1);
	lcdtextwrite("U r playing now ", 2);

        fndDisp(Bigcount, 0);							//현재 숫자 FND에 출력
        printf(" current number is %d.\n",Bigcount);
       
        Bcount1 = Bigcount%10; 					// 일의 자리 3, 6, 9 찾기
        Bcount10 = Bigcount/10; 					// 십의 자리 찾기
        
				if((Bcount1==3)||(Bcount1==6)||(Bcount1==9))
				{
						Bcount++;  									// 초기값 0  1의자리수에 3 6 9 있으면 1증가
				}
        
				if((Bcount10==3)||(Bcount10==6)||(Bcount10==9))
				{
						Bcount++;  									// 10의 자리수에 3 6 9 있으면 1증가
				}
					printf("Bcount is %d\n",Bcount);	//결정된 Bcount 값 출력
        
			int nm=0;
			int s;
			
			
///////////////////////////////////////////////////////////////////////////////////////////////--->버튼확인쓰레드
	errr11 = pthread_create(&tiid1, NULL, &WhatButton, NULL); 		//
	if( err11!=0)																							//				Acount
	{																												// =>키Home=1,Bakc=2
		printf("Thread Create Error: [%d]\n", err11);						//
	} 																												//
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////--->가속도측정쓰레드
	errr22 = pthread_create(&accth, NULL, &returnaccres, NULL); 	//
	if( err22!=0)																							//
	{																												// => 툭치면 ==> Acount=0
		printf("Thread Create Error: [%d]\n", err22);						//
	} 																												//
///////////////////////////////////////////////////////////////////////////////////////////////

			//for(nm=0;nm<50000000;nm++) //대략 2sec 간의 입력 제한 시간
				for(nm=0;nm<200000000;nm++)		//연습용 제한시간 길게
				{ s=s+1;								//의미없는계산
					timeover=0;
					if((Acount==0)||(Acount==1)||(Acount==2))
					{
					    check=1;									//옳은선택 ==> check가 1
						break;
					}
						timeover=1;
				}
				usleep(200000);
				
	if( (!(check==1))|| !( Acount == Bcount)||(upsidedown==1))//check이 0이거나 || A와 B다르거나 || 키트가뒤집어지면
	{//게임오버
			printf("game over ~~\n");
/////////////////////////////////////////////////////////////////////////////////////////////////////--->시간초과 or 오답 or 뒤집히면 실행되는 쓰레드
			err44 = pthread_create(&failtxt, NULL, &returntxt, NULL); 		//
			if( err44!=0)																							//
			{																												// => 오답 이벤트 처리 (부저,텍스트LCD)
				printf("Thread Create Error: [%d]\n", err44);						//
			} 																												//
/////////////////////////////////////////////////////////////////////////////////////////////////////

			pwmLedInit();
			pwmSetPercent(100,2);		//R ==>빨강색 ON
			pwmSetPercent(0,1);    		//G   
			pwmSetPercent(0,0);			//B

			usleep(250000);				//잠시동안 불켜짐
			
			pwmSetPercent(0,2);			//R
			pwmSetPercent(0,1);			//G ==>colorled OFF
			pwmSetPercent(0,0);			//B
			
			while(!(retthval==1))
			{
					;									//이 while문은 오답 이벤트처리 쓰레드와 함께 종료된다.
			}
			//모든것이종료   
			return 0;   
		}
        
	if( Acount == Bcount){								// ****성공시****
		
			pass = 0;
			Acount = 3;											//Acount 초기화
			Bcount =0;											//Bcount 초기화
			check = 0;											//check 초기화 
		
           Bigcount++;										//Bigcount값 증가
           printf("ok next\n");
/////////////////////////////////////////////////////////////////////////////////////////////////////
			err55 = pthread_create(&tiid2, NULL, &Blinkcolorled, NULL); 	//
			if( err55!=0)																							//
			{																												// --->초록led껏다키는 쓰레드
				printf("Thread Create Error: [%d]\n", err55);						//
			} 																												//
/////////////////////////////////////////////////////////////////////////////////////////////////////           						
			usleep(200000);
			pass=1;												//다음 가속도값 받기위한 준비완료
			result1 = 0;										//가속도차이값 초기화
			
        }
}
	return 0;
}
