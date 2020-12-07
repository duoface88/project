#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "accelMagGyro.h"
#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/"
#define MAGNEPATH "/sys/class/misc/FreescaleMagnetometer/"
#define GYROPATH "/sys/class/misc/FreescaleGyroscope/"
FILE *fp = NULL;
int fd;
int curAcel(void)
{
int fd = 0;
FILE *fp = NULL;
//Accelerometer
fd = open (ACCELPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
fp = fopen (ACCELPATH "data", "rt");
int accel[3];
fscanf(fp,"%d, %d, %d",&accel[0],&accel[1],&accel[2]);
//printf ("I read Accel %d, %d, %d\r\n",accel[0],accel[1],accel[2]);
int accelnum = abs(accel[0]); //////////////절댓값으로 만들어서 리턴함
fclose(fp);
return accelnum;
}

int curMag(void)
{
//Magnetometer
fd = open (MAGNEPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
fp = fopen (MAGNEPATH "data", "rt");
int magne[3];
fscanf(fp,"%d, %d, %d",&magne[0],&magne[1],&magne[2]);
printf ("I read Magneto %d, %d, %d\r\n",magne[0],magne[1],magne[2]);
fclose(fp);
return 0;
	
}

int curGyro(void)
{
//Gyroscope
fd = open (GYROPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
fp = fopen (GYROPATH "data", "rt");
int gyro[3];
fscanf(fp,"%d, %d, %d",&gyro[0],&gyro[1],&gyro[2]);
//printf ("I read Gyroscope %d, %d, %d\r\n",gyro[0],gyro[1],gyro[2]);
int gyronum = abs(gyro[1]); //////////////절댓값으로 만들어서 리턴함
fclose(fp);
return gyronum;
}


