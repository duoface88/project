#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "temp.h"

int main(void)
{
   double curtemp=temperatureout();  //더블형 온도를 리턴함
   printf("current temperature is %lf.\n",curtemp);//온도를 출력
   return 0;
}
