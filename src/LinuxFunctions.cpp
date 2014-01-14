#include "LinuxFunctions.h"
void initTermios(int echo)
{
  tcgetattr(0, &_old); /* grab old terminal i/o settings */
  _new = _old; /* make new settings same as old settings */
  _new.c_lflag &= ~ICANON; /* disable buffered i/o */
  _new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &_new); /* use these new terminal i/o settings now */
}

void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &_old);
}

char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

char _getch(void)
{
  return getch_(0);
}

void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y+1,x+1);
}
