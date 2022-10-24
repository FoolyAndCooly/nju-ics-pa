#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
	panic("Not implemented");
}
void itoa(char* str,int num){
	char buf[100]={0};
	int i=0,j=0;
	while(num){
	buf[i++]=num%10+'\0';
	num/=10;
	}
	i--;
	for(;i>=0;i--){
	str[j++]=buf[i];
	}
	str[j]='\0';
}
int vsprintf(char *out, const char *fmt, va_list ap) {
	int i;
	char *t;
	t=out;
	char c;
	char *s,buf[100]={0};
	while(*fmt++){
	if(*fmt != '%'){*out++ = *fmt;}
	else{
		switch(*++fmt){
			case 'c':
				c=(char)va_arg(ap,int);
				*out++ =c;
				break;
			case 'd':
				i=va_arg(ap,int);
				itoa(buf,i);
				strcat(out,buf);
				out=out+strlen(buf);
				break;
			case 's':
				s=va_arg(ap,char *);
				strcat(out,s);
				out=out+strlen(s);
			break;
			}
		}
	}
	*out='\0';
	return out-t-1; 
	panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
	va_list ap;
	int ret = -1;
	va_start(ap, fmt);
	ret = vsprintf(out , fmt, ap);
	va_end(ap);
	return ret;
	panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
