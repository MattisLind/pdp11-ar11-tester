#include "printf.h"

#define CONSOLE_RX_CS 0177560
#define CONSOLE_RX_DB 0177562
#define CONSOLE_TX_CS 0177564
#define CONSOLE_TX_DB 0177566
#define AR11_AD_CS    0170400
#define AR11_AD_DB    0170402
#define AR11_AD_CS_UNIPOLAR    1 << 13
#define AR11_AD_CS_START       1 << 0
#define AR11_AD_CS_DONE        1 << 7
#define AR11_DP_CS    0170410
#define AR11_DP_DX    0170412
#define AR11_DP_DY    0170414
#define AR11_DP_CS_INTENS      1 << 0
#define AR11_DP_CS_DONE        1 << 7

int putchar (int ch)
{
  while (*((volatile char *) (CONSOLE_TX_CS)) >= 0); // wait for TX ready
  *((volatile int *) (CONSOLE_TX_DB)) = ch;
  return 0;
}

void putch (void * p, char c) {
  putchar (c);
}


int getAD (int channel) {
  *((volatile int *) (AR11_AD_CS)) = ((channel & 0xf) << 8) | AR11_AD_CS_UNIPOLAR | AR11_AD_CS_START;
  while (*((volatile char *) (AR11_AD_CS)) >= 0); // wait for AD Ready
  return (*((volatile int *) (AR11_AD_DB)));
}

void putXY (int x, int y) {
  while (*((volatile char *) (AR11_DP_CS)) >= 0);
  (*((volatile int *) (AR11_DP_DX))) = x;
  (*((volatile int *) (AR11_DP_DY))) = y;
  *((volatile int *) (AR11_DP_CS)) = AR11_DP_CS_INTENS;
}


int puts (const char * str) {
  char ch;
  while ((ch=*str++)) {
    putchar(ch);
  }
}

char getchar () {
  while (*((volatile char *) (CONSOLE_RX_CS))>=0); // Wait for character received
  return (*((volatile int *) (CONSOLE_RX_DB))) & 0x7f;
}

int main ()
{
  int x1,y1,x2,y2;
  init_printf((void *) 0, putch);
  while (1) {
    x1=getAD(0);
    y1=getAD(1);
    putXY(x1,y1);
    x2=getAD(2);
    y2=getAD(3);
    putXY(x2,y2);
    printf ("%d %d %d %d\n", x1,y1,x2,y2 );
  }
  return 0;
}




