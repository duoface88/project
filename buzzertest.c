#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzer.h"
#define MAX_SCALE_STEP 8

int musicScale[MAX_SCALE_STEP] =
{
262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};

int main(int argc , char **argv)
{
    int freIndex;
    if (argc < 2 || findBuzzerSysPath() )
    {
        printf("Error!\n");
        doHelp();
        return 1;
    }
    freIndex = atoi(argv[1]);
    printf("freIndex :%d \n",freIndex);
    if ( freIndex > MAX_SCALE_STEP )
    {
        printf(" <buzzerNo> over range \n");
        doHelp();
        return 1;
    }
    if ( freIndex == 0)// disable
    {
        buzzerEnable(0);
    }
    else
    {
        setFrequency(musicScale[freIndex-1]);
        buzzerEnable(1);
    }
    return 0;
}
=======
int freIndex;
if (argc < 2 || findBuzzerSysPath() )
{
printf("Error!\n");
doHelp();
return 1;
}
freIndex = atoi(argv[1]);
printf("freIndex :%d \n",freIndex);
if ( freIndex > MAX_SCALE_STEP )
{
printf(" <buzzerNo> over range \n");
doHelp();
return 1;
}
if ( freIndex == 0)// disable
{
buzzerEnable(0);
}
else
{
	/*buzzerEnable(1);
	setFrequency(musicScale[1]);
	usleep(1000*500);
	setFrequency(musicScale[3]);
	usleep(1000*500);
	setFrequency(musicScale[5]);
	usleep(1000*500);
	setFrequency(musicScale[7]);
	usleep(1000*500);
	setFrequency(musicScale[8]);
	*/
	
setFrequency(musicScale[freIndex-1]);
buzzerEnable(1);
}
return 0;
}

