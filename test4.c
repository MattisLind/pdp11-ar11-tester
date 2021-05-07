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
#define AR11_DP_CS_ERASE       1 << 12

void inline zero () {
  (*((volatile int *) (AR11_DP_DX))) = 0;
  (*((volatile int *) (AR11_DP_DY))) = 0;
}

void inline penUp ()
{
  *((volatile int *) (AR11_DP_CS)) = AR11_DP_CS_ERASE;
}

void inline penDown ()
{
  *((volatile int *) (AR11_DP_CS)) = 0;
}

void inline moveTo(int x,int y)
{
  (*((volatile int *) (AR11_DP_DX))) = x;
  (*((volatile int *) (AR11_DP_DY))) = y;
} 



#define SIZE 20

void  plotSquare(int x, int y) {
  moveTo(x-SIZE, y-SIZE);
  penDown();
  moveTo(x, y-SIZE);
  moveTo(x+SIZE, y-SIZE);
  moveTo(x+SIZE, y);
  moveTo(x+SIZE, y+SIZE);
  moveTo(x, y+SIZE);
  moveTo(x-SIZE, y+SIZE);
  moveTo(x-SIZE, y);
  moveTo(x-SIZE, y-SIZE);
  penUp();
}

void plotStar(int x, int y) {

  moveTo(x-SIZE,y);
  penDown();
  moveTo(x,y);
  moveTo(x+SIZE,y);
  moveTo(x,y+SIZE);
  moveTo(x,y);
  moveTo(x,y-SIZE);
  penUp();
}

int main ()
{
  int x1=200,y1=200,x2=400,y2=400;
  while (1) {
    plotStar(x1,y1);
    plotSquare(x2,y2);
  }
  return 0;
}




