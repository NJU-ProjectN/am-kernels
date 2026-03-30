#include <amtest.h>
#include <limits.h>
#include <stdint.h>
// #include <stdio.h>
// #include <string.h>
// #include <assert.h>

void test_sprintf_basic() {
  char buf[128];

  // 测试int
  int data_int[] = {
    0,
    INT_MAX / 17,
    INT_MAX,
    INT_MIN,
    INT_MIN + 1,
  };
  const char *expected_int[] = {
    "0",
    "126322567",
    "2147483647",
    "-2147483648",
    "-2147483647"
  };
  int n_int = sizeof(data_int) / sizeof(data_int[0]);
  for (int i = 0; i < n_int; i++) {
    sprintf(buf, "%d", data_int[i]);
    assert(strcmp(buf, expected_int[i]) == 0);
  }

  // 测试unsigned int
  unsigned int data_uint[] = {
    UINT_MAX / 17,
    UINT_MAX
  };
  const char *expected_uint[] = {
    "252645135",
    "4294967295"
  };
  int n_uint = sizeof(data_uint) / sizeof(data_uint[0]);
  for (int i = 0; i < n_uint; i++) {
    sprintf(buf, "%u", data_uint[i]);
    assert(strcmp(buf, expected_uint[i]) == 0);
  }
}

void test_sprintf_width_precision() {
  char buf[128];

  // 宽度、精度
  sprintf(buf, "%5d", 42);       // 宽度
  assert(strcmp(buf, "   42") == 0);

  sprintf(buf, "%.4d", 42);      // 精度
  assert(strcmp(buf, "0042") == 0);

  sprintf(buf, "%7.3d", 888);    // 宽度+精度
  assert(strcmp(buf, "    888") == 0);

  sprintf(buf, "%-5d", 7);       // 左对齐
  assert(strcmp(buf, "7    ") == 0);

  sprintf(buf, "%+d", -123);     // 显示符号
  assert(strcmp(buf, "-123") == 0);

  sprintf(buf, "%+d", 123);      // 显示正号
  assert(strcmp(buf, "+123") == 0);

  sprintf(buf, "%04d", 7);       // 宽度、补零
  assert(strcmp(buf, "0007") == 0);
}

void test_sprintf_string() {
  char buf[128];

  sprintf(buf, "%.5s", "abcdefg");
  assert(strcmp(buf, "abcde") == 0);

  sprintf(buf, "%10s", "test");
  assert(strcmp(buf, "      test") == 0);

  sprintf(buf, "%-10s", "test");
  assert(strcmp(buf, "test      ") == 0);
}

void test_string_3() {
  test_sprintf_basic();
  test_sprintf_width_precision();
  test_sprintf_string();
}