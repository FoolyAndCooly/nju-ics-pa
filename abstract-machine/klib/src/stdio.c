#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
#include "string.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	int i;
	char c;
	char *s;
	while(*fmt){
	if(*fmt != '%'){*out++ = *fmt;}
	else{
	switch(*++fmt){
	case 'c':
	c=(char)va_arg(ap,int);
	*out++ =c;
	break;
	case 'd':
	i=va_arg(ap,int);
	*out++ =i;
	break;
	case 's':
	s=va_arg(ap,char *);
	strcat(out,s);
	out=out+strlen(s);
	}
	}
	*out='\0';
	}
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
