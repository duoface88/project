#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include "accelMagGyro.h"

int main(void)
{
	while(1)
	{
	curAcel();
	curMag();
	curGyro();
	sleep(1);
	
}
	return 0;
}
