#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  //char *empty[] =  {NULL };
  int argc=*args;
  char** argv=(char**)(args+1);
  //printf("%s\n",argv);
  char** envp=(char**)(args+argc+2);
  environ = envp;
  exit(main(0, NULL, NULL));
  assert(0);
}
