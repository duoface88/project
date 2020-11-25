#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzer.h"

#define MAXK_SCALE_STEP 8
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME              "enable"
#define BUZZER_FREQUENCY_NAME       "frequency"
char gBuzzerBaseSysDir[128]; // /sys/bus/platform/devices/peribuzzer.XX가 결정됨
const int musicScale[MAXK_SCALE_STEP] = {
    262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};

static int fd = 0;

int findBuzzerSysPath(){
    Dir * dir_info = opendir(BUZZER_BASE_SYS_PATH);
    int ifNotFound = 1;
    if(dir_info != NULL){
        while(1){
            struct dirent *dir_entry;
            dir_entry = readdir (dir_info);
            if (dir_entry == NULL) break;
            if(strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0){
                ifNotFound = 0;
                sprintf(gBuzzerBaseSysDir, "%s%s/", BUZZER_BASE_SYS_PATH, dir_entry->d_name);
            }
        }
    }
    printf("find %s\n", gBuzzerBaseSysDir);
    return ifNotFound;
}
// 버저 경로 찾기 : /sys/bus/platform/devices/peribuzzer.XX 의 XX를 결정하는 것

void doHelp(void){
    printf("Usage : \n");
    printf("buzzer test <buzzerNo> \n");
    printf("buzzerNo: \n");
    printf("do(1), re(2), mi(3), fa(4), sol(5), ra(6), si(7), do(8) \n");
    printf("off(0)\n");
}

void buzzerEnable(int bEnable){
    char path[200];
    sprintf(path, "%s%s", gBuzzerBaseSysDir, BUZZER_ENABLE_NAME);
    int fd = open(path, O_WRONLY);
    if (bEnable) write(fd, &"1", 1);
    else              wirte(fd, &"0", 1);
    close(fd);
}

void setFrequency(int frequency)
{
    char path[200];
    sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
    int fd=open(path,O_WRONLY);
    dprintf(fd, "%d", frequency);
    close(fd);
}

int buzzerInit(){
    findBuzzerSysPath(); // findBuzzerSysPath에서 peribuzzerXX를 찾는다
    fd = open(gBuzzerBaseSysDir, O_RDONLY);
    if(fd<0)
	{
		printf("Device Open Error!\r\n");
		exit(1);
	}
    return 1;
}

int buzzerExit(){
    close(fd);
}

int buzzerPlaySong(int scale){
    buzzerEnable(1);
    setFrequency(262); // 도
    sleep(1);
    setFrequency(294); // 레
    sleep(1);
    setFrequency(330); // 미
    sleep(1);
    setFrequency(349); // 파
    sleep(1);
    setFrequency(392); // 솔
    sleep(1);
    setFrequency(440); // 라
    sleep(1);
    setFrequency(494); // 시
    sleep(1);
    setFrequency(523); // 도
    sleep(1);
}