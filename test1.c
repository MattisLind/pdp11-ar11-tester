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
  init_printf((void *) 0, putch);
  while (1) {
    printf ("%d %d %d %d\n", getAD(0), getAD(1), getAD(2), getAD(3));
  }
  return 0;
}




