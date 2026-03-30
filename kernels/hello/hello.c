#include <am.h>
#include <stdio.h>
// #include <klib.h>
#include <klib-macros.h>


int main(const char *args) {
  const char *fmt =
    "Hello, AbstractMachine!\n"
    "mainargs = '%'.\n";

  for (const char *p = fmt; *p; p++) {
    (*p == '%') ? putstr(args) : putch(*p);
  }
  printf("xxx123\n");
  
  int *badptr = (int*)0x12345678;   //测试iringbuf
  int x = *badptr;
  putch(x);
  return 0;
}
