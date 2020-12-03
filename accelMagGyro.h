#ifndef __ACCELMAGGYRO_H__
#define __ACCELMAGGYRO_H__
#define MAX_FND_NUM 6
#define FND_DATA_BUFF_LEN (MAX_FND_NUM + 2)

	int curAcel(void);
	int curMag(void);
	int curGyro(void);
	
typedef struct FNDWriteDataForm_tag
{
char DataNumeric[FND_DATA_BUFF_LEN]; //숫자 0-9
char DataDot[FND_DATA_BUFF_LEN]; //숫자 0 or 1
char DataValid[FND_DATA_BUFF_LEN]; //숫자 0 or 1
}stFndWriteForm,*pStFndWriteForm;
int fndDisp(int num, int dotflag);
#endif __ACCELMAGGYRO_H__
