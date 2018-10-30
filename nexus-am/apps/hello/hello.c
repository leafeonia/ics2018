#include <am.h>
#include <klib.h>
void print(const char *s) {
  for (; *s; s ++) {
    _putc(*s);
  }
}
int main() {
  for (int i = 0; i < 10; i ++) {
    print("Hello World!\n");
  }
  for(int i = 0;i < 100;i++){
  	printf("%d\n",i);
  }
  return 0;
}
