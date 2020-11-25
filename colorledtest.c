#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "colorled.h"

int main(void)
{
/*int main(int argc, char *argv[]) 
{
if (argc != 4)
{ printf ("colorledtest.elf 0-100 0-100 0-100\r\n");
printf ("ex) colorledtest.elf 100 100 100 ==> full white color\r\n");
return 0;
 }*/
pwmLedInit();
pwmSetPercent(100,0);//R
pwmSetPercent(0,1);    //G  (빛성분몇퍼센트, 123중하나)
pwmSetPercent(100,2);//B
pwmInactiveAll();
return 0;
}



	
