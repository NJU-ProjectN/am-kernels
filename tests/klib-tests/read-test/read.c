#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <assert.h>

int main() {
  char str1[15];
  char str2[15];
  int ret;


  memcpy(str1, "abcdef", 6);
  memcpy(str2, "ABCDEF", 6);

  // 比较前5个字节
  ret = memcmp(str1, str2, 5);

  // 使用 assert() 断言
  assert(ret > 0);

  return 0;
}
