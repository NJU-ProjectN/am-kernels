#include <amtest.h>

#define N 32

uint8_t data[N];

void reset() {
  int i;
  for (i = 0; i < N; i ++) {
    data[i] = i + 1;
  }
}

void check_seq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val + i - l);
  }
}

// 检查[l,r)区间中的值是否均为val
void check_eq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val);
  }
}

void test_string_1() {
  int l, r;
  for (l = 0; l < N; l ++) {
    for (r = l + 1; r <= N; r ++) {
      reset();
      uint8_t val = (l + r) / 2;
      memset(data + l, val, r - l);
      check_seq(0, l, 1);
      check_eq(l, r, val);
      check_seq(r, N, r + 1);
    }
  }
  char buf[N];
  const char *srcs[] = {
    "",                           // 空字符串
    "a",
    "hello",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ", // 恰好 26 字符
    "0123456789abcdef0123456789abc", // 31 字符
    "0123456789abcdef0123456789abcd" // 32 字符（长度=N-1，最后为\0）
  };
  int num_srcs = sizeof(srcs) / sizeof(srcs[0]);
  for (int i = 0; i < num_srcs; i++) {
    // 先清空目标数组
    memset(buf, '?', sizeof(buf));
    strcpy(buf, srcs[i]);
    // 检查内容是否完全一致
    assert(strcmp(buf, srcs[i]) == 0);

    // 检查结尾'\0'
    int len = strlen(srcs[i]);
    assert(buf[len] == '\0');

    // 若拷贝内容较短，后续字节保持为'?'
    for (int j = len + 1; j < N; j++) {
      assert(buf[j] == '?');
    }
  }
}