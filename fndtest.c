#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include "fnd.h"
int num ;
int dotflag;

int main(int argc , char **argv)
{
   num = atoi(argv[1]); //입력받은 숫자 정수형으로 바꿈
   if(argc>1){dotflag = atoi(argv[2]);}  // 닷까지 입력받으면 정수형으로 변환
   fndDisp( num , dotflag);    // 출력하고싶은 수 점 입력
   sleep(1);//1초있다가
   fndOff();//fnd 끄는 함수
   return 0;
}