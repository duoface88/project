#ifndef _BUTTON_H_
#define _BUTTON_H_
#define MESSAGE_ID 125
typedef struct
{
long int messageNum;
int keyInput;
int pressed;
} BUTTON_MSG_T;

int buttonInit(void);
int probeButtonPath(char *newPath);
int buttonExit(void);
static void* buttonThFunc(void*a);
#endif
