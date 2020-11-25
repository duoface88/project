#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include "lcdtext.h"
#define TEXTLCD_DRIVER_NAME "/dev/peritextlcd"

//int lcdtextwrite(int argc , char **argv)
//int lcdtextwrite(int argc , char **argv)
int lcdtextwrite(const char *str1, unsigned int lineFlag)
{
unsigned int linenum = 0;
stTextLCD stlcd; // stTextLCD 구조체를 가지고 드라이버와 인터페이스
int fd;
int len;
memset(&stlcd,0,sizeof(stTextLCD)); // 구조체 초기화
if (lineFlag >=3 ) { // line 정보와 쓸 정보를 확인
perror(" Args number is less than 2\n");
//doHelp();
return 1;
}
linenum = lineFlag;
printf("linenum :%d\n", linenum );
if ( linenum  == 1)
stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
else if ( linenum == 2)
stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
else {
printf("linenum : %d wrong . range (1 ~ 2)\n", linenum );
return 1; }
len = strlen(str1);//문자열 길이구하기
//printf("문자열길이는:%d\n",len);
//printf("문자는 %s\n",str1);
if ( len > COLUMN_NUM)
memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, COLUMN_NUM);
else
memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, len);
stlcd.cmd = CMD_WRITE_STRING;
fd = open(TEXTLCD_DRIVER_NAME,O_RDWR); // open driver
if ( fd < 0 ) {
perror("driver (//dev//peritextlcd) open error.\n");
return 1;
}
write(fd,&stlcd,sizeof(stTextLCD));
close(fd);
return 0;
}

