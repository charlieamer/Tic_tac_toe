#ifndef LINUXFUNCTIONS_INCLUDED
#define LINUXFUNCTIONS_INCLUDED

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
static struct termios _old, _new;
#define Sleep(n) usleep((n)*1000);cout.flush()
void initTermios(int echo);

/* Restore old terminal i/o settings */
void resetTermios(void);

/* Read 1 character - echo defines echo mode */
char getch_(int echo);

/* Read 1 character without echo */
char _getch(void);

#endif // LINUXFUNCTIONS_INCLUDED
