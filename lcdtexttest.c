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
const char a[16]={"l did it"};

int main(void)
{
	lcdtextwrite(a, 2);
	return 0;
}
