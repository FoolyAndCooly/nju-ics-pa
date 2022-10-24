#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  uint32_t cnt=0;
  while (*s != '\0'){
	  cnt++;
	  s++;
  }
  return cnt;
  panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  char *t=dst;
  uint32_t i=0;
  while (*(src+i) != '\0'){
	*dst = *src;
	dst++ ;
	i++ ;
  }
  *dst = '\0';
  return t;
  panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *t;
  uint32_t i=0;
  size_t cnt=0;
  t = dst;
  while (*(src+i) != '\0' && cnt < n){
	*dst = *(src+i);
	dst++ ;
	i++ ;
	cnt++ ;
  }
  *dst = '\0';
  return t;

  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
	int cnt =0,i=0;
	while(*(dst + cnt) != '\0'){cnt++;}
	while(*(src+i)!='\0'){
		*(dst+cnt+i)=*(src+i);
		i++;
	}
	*(dst+cnt+i)='\0';
	return dst;
  panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
	uint32_t i=0;
	while((*(s1+i) != '\0') && (*(s1+i) ==*(s2+i))){
	i++;
	}
	if(*(s1+i) >*(s2+i)){return 1;}
	else if(*(s1+i) < *(s2+i)){return -1;}
	else {return 0;}
  panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
	uint32_t i=0;
	if(!n) return 0;
	while( ((*(s1+i) != '\0') && (*(s1+i) ==*(s2+i)) ) && n>0 ){
	i++;
	n--;
	}
	if(*(s1+i) >*(s2+i)){return *(s1+i)-*(s2+i);}
	else if(*(s1+i) < *(s2+i)){return *(s1+i) - *(s2+i);}
	else {return 0;}

	panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  if(s == NULL || n <0){
	return NULL;
  }
  char *p=(char*)s;
  while(n>0){
	*p++=c;
	n--;
  }
  return s; 
  panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  if (dst > src && ( (char *)dst < (char *)src +n ) ){
    dst=(char*)dst+n-1;
	src=(char*)src+n-1;
	while(n--){
	*(char *)dst=*(char*)src;
	dst=(char*)dst-1;
	src=(char*)src-1;
	}
  }else{
   while(n--){
	*(char*)dst = *(char*)src;
	dst=(char *)dst+1;
	src=(char*)src +1;
   }
  }
	
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  void* p=out;
  while(n--){
	*(char*)out=*(char*)in;
	out=(char*)out+1;
	in=(char*)in+1;
  }
	return p;
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  	uint32_t i=0;
	while(n-- && (*((char*)s1+i) ==*((char*)s2+i))){
	i++;
	}
	if(*((char*)s1+i) >*((char*)s2+i)){return 1;}
	else if(*((char*)s1+i) < *((char*)s2+i)){return -1;}
	else {return 0;}

  panic("Not implemented");
}

#endif
