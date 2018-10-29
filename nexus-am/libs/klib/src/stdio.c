#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char* s = "";
  int ret = sprintf(s,fmt);
  for (; *s; s ++) {
    _putc(*s);
  }
  return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
    int ret = 0;
    int cnt,num;
    char a[100];
    char* p;
    va_list ap;
    va_start(ap,fmt);
    while(*fmt){
        if(*fmt == '%'){
            char op = *++fmt;
            switch(op){
                case 's':
                    p = va_arg(ap,char*);
                    while(*p){
                        *out++ = *p++;
                        ret++;
                    }
                    break;
                case 'd':
                    num = va_arg(ap,int);
                    cnt = 0;
                    while(num){
                        a[++cnt] = num%10 + '0';
                        num /= 10;
                    }
                    ;
                    while(cnt){
                        *out++ = a[cnt];
                        cnt--;
                        ret++;
                    }
                    break;
                default:break;
            }
            fmt++;
        }
        else {
            *out++ = *fmt++;
            ret++;
        }
    }
    *out = '\0';
    return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
