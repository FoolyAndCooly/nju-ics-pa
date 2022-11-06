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
  while (*src != '\0'){
	*dst = *src;
	dst++ ;
	src++ ;
  }
  *dst = '\0';
  return t;
  panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *t=dst ;
  while (*src != '\0' && n--){
	*dst = *src;
	dst++ ;
	src++ ;
  }
  *dst = '\0';
  return t;

  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
	char* p=dst;
	while(*dst != '\0'){dst ++;}
	while(*src!='\0'){
		*dst=*src;
		dst ++;
		src ++;
	}
	*dst ='\0';
	return p;
  panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
	uint32_t i=0;
	while((*(s1+i) != '\0') && (*(s1+i) ==*(s2+i))){
	i++;
	}
	return *(s1+i)-*(s2+i);
  panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
	if(!n) return 0;
	while( ((*s1 != '\0') && (*s1 ==*s2) ) && n-- ){
	s1++;
	s2++;
	}
	return *(s1-1)-*(s2-1);

	panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  if(s == NULL || n <0){
	return NULL;
  }
  void *p = s;
  while(n--){
	*(char*)s=c;
	s=(char*)s+1;
  }
  *(char*)s='\0';
  return p; 
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
  for(int i=0;i<n;i++){
	*(char*)out=*(char*)in;
	out=(char*)out+1;
	in=(char*)in+1;
  }
	return p;
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
	for(int i=0; i<n;i++){
	if (*((char*)s1) ==*((char*)s2)){
	s1=(char*)s1+1;
	s2=(char*)s2+1;
	}
	else{break;}
	}
	return *((char*)s1)-*((char*)s2);

  panic("Not implemented");
}

#endif
