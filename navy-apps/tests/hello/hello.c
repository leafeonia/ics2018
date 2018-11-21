#include <unistd.h>
#include <stdio.h>

int main() {
  write(1, "Hello World!\n", 13);
  char buffer[100];
  int i = 2;
  volatile int j = 0;
  while (1) {
    j ++;
    if (j == 10000) {
      sprintf(buffer,"Hello World from Navy-apps for the %dth time!\n", i ++);
      write(1,buffer,100);
      j = 0;
    }
  }
  return 0;
}
