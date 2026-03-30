#include <amtest.h>
// #include <string.h>
// #include <assert.h>

#define N 32

void test_memcmp() {
  // 完全相等
  uint8_t a[N] = {1, 2, 3, 4, 5};
  uint8_t b[N] = {1, 2, 3, 4, 5};
  assert(memcmp(a, b, 5) == 0);

  // a < b
  b[2] = 7;
  assert(memcmp(a, b, 5) < 0);

  // a > b
  a[2] = 8;
  assert(memcmp(a, b, 5) > 0);

  // 部分相等
  a[2] = 7; b[2] = 7; // 恢复相等
  assert(memcmp(a, b, 3) == 0);

  // 长度为零时总是相等
  assert(memcmp(a, b, 0) == 0);
}

void test_strlen() {
  // 空串
  assert(strlen("") == 0);

  // 普通字符串
  assert(strlen("A") == 1);
  assert(strlen("hello") == 5);

  // 包含空字符后的内容不计入长度
  char s[] = {'a', 'b', 'c', '\0', 'd', 'e'};
  assert(strlen(s) == 3);

  // 边界情况：最大长度串
  char maxs[N + 1];
  for (int i = 0; i < N; i++) maxs[i] = 'X';
  maxs[N] = '\0';
  assert(strlen(maxs) == N);
}

void test_string_2() {
  test_memcmp();
  test_strlen();
}