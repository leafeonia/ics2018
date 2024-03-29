#include <assert.h>
#include <stdlib.h>
#include <ndl.h>
#include <stdio.h>
int main() {
  //printf("enter.\n");
  NDL_Bitmap *bmp = (NDL_Bitmap*)malloc(sizeof(NDL_Bitmap));
  //printf("malloc finished\n");
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  //printf("load finished.\n");
  assert(bmp->pixels);
  NDL_OpenDisplay(bmp->w, bmp->h);
  //printf("OpenDisplay finished.\n");
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
  NDL_Render();
  NDL_CloseDisplay();
  //printf("finish\n");
  while (1);
  return 0;
}
