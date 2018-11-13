#include "klib.h"
#include <stdarg.h>
#include <stdlib.h>
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#include <stdarg.h>
#include <stdio.h>

static char buffer_hex[100];
void inttohex(int aa)
{
    static int i = 0;
    if (aa < 16)            //递归结束条件 
    {
        if (aa < 10)        //当前数转换成字符放入字符串 
            buffer_hex[i] = aa + '0';
        else
            buffer_hex[i] = aa - 10 + 'A';
        buffer_hex[i+1] = '\0'; //字符串结束标志 
    }
    else
    {
        inttohex(aa / 16);  //递归调用 
        i++;                //字符串索引+1 
        aa %= 16;           //计算当前值
        if (aa < 10)        //当前数转换成字符放入字符串 
            buffer_hex[i] = aa + '0';
        else
            buffer_hex[i] = aa - 10 + 'A';
    }
}


int vsprintf(char *out, const char *fmt, va_list ap) {
    int ret = 0;
    int cnt,num;
    char a[100];
    char* p;
    while(*fmt){
        if(*fmt == '%'){
            char op = *++fmt;
            while('0' <= op && op <= '9') op = *++fmt;
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
                    if(num < 0){
                        *out++ = '-';
                        num = -num;
                    }
                    if(!num) {
                        *out++ = '0';
                    }
                    else
                        {
                        while(num)
                            {
                                a[++cnt] = num%10 + '0';
                                num /= 10;
                            }
                        }

                    while(num){
                        a[++cnt] = num%10 + '0';
                        num /= 10;
                    }
                    
                    while(cnt){
                        *out++ = a[cnt];
                        cnt--;
                        ret++;
                    }
                    break;
                case 'x':
                	num = va_arg(ap,int);
                	inttohex(num);
             		char* temp = buffer_hex;
             		while(*temp++){
             			*out++ = *temp++;
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

int sprintf(char *out, const char *fmt, ...) {
    va_list aptr;
    va_start(aptr, fmt);
    int ret = vsprintf(out, fmt, aptr);
    va_end(aptr);
    return ret;
}

int printf(const char *fmt, ...) {
    char temp[1000];
    char* s = temp;
    va_list aptr;
    va_start(aptr, fmt);
    int ret = vsprintf(s, fmt, aptr);
    va_end(aptr);
    for (; *s; s ++) {
        _putc(*s);
    }
    return ret;
}

/*
int printf(const char *fmt, ...) {
  char temp[1000];
  char* s = temp;
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
}*/

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
