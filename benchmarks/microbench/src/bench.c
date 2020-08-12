#include <am.h>
#include <benchmark.h>
#include <limits.h>
#include <klib-macros.h>

Benchmark *current;
Setting *setting;

static char *hbrk;

static uint32_t uptime_ms() { return io_read(AM_TIMER_UPTIME).us / 1000; }

// The benchmark list

#define ENTRY(_name, _sname, _s, _m, _l, _desc) \
  { .prepare = bench_##_name##_prepare, \
    .run = bench_##_name##_run, \
    .validate = bench_##_name##_validate, \
    .name = _sname, \
    .desc = _desc, \
    .settings = {_s, _m, _l}, },

Benchmark benchmarks[] = {
  BENCHMARK_LIST(ENTRY)
};

// Running a benchmark
static void bench_prepare(Result *res) {
  res->msec = uptime_ms();
}

static void bench_reset() {
  hbrk = (void *)ROUNDUP(heap.start, 8);
}

static void bench_done(Result *res) {
  res->msec = uptime_ms() - res->msec;
}

static const char *bench_check(Benchmark *bench) {
  uintptr_t freesp = (uintptr_t)heap.end - (uintptr_t)heap.start;
  if (freesp < setting->mlim) {
    return "(insufficient memory)";
  }
  return NULL;
}

static void run_once(Benchmark *b, Result *res) {
  bench_reset();       // reset malloc state
  current->prepare();  // call bechmark's prepare function
  bench_prepare(res);  // clean everything, start timer
  current->run();      // run it
  bench_done(res);     // collect results
  res->pass = current->validate();
}

static unsigned long score(Benchmark *b, unsigned long tsc, unsigned long msec) {
  if (msec == 0) return 0;
  return (REF_SCORE / 1000) * setting->ref / msec;
}

int main(const char *args) {
  const char *setting_name = args;
  if (args == NULL || strcmp(args, "") == 0) {
    printf("Empty mainargs. Use \"ref\" by default\n");
    setting_name = "ref";
  }
  int setting_id = -1;

  if      (strcmp(setting_name, "test" ) == 0) setting_id = 0;
  else if (strcmp(setting_name, "train") == 0) setting_id = 1;
  else if (strcmp(setting_name, "ref"  ) == 0) setting_id = 2;
  else {
    printf("Invalid mainargs: \"%s\"; "
           "must be in {test, train, ref}\n", setting_name);
    halt(1);
  }

  ioe_init();

  printf("======= Running MicroBench [input *%s*] =======\n", setting_name);

  unsigned long bench_score = 0;
  int pass = 1;
  uint32_t t0 = uptime_ms();

  for (int i = 0; i < LENGTH(benchmarks); i ++) {
    Benchmark *bench = &benchmarks[i];
    current = bench;
    setting = &bench->settings[setting_id];
    const char *msg = bench_check(bench);
    printf("[%s] %s: ", bench->name, bench->desc);
    if (msg != NULL) {
      printf("Ignored %s\n", msg);
    } else {
      unsigned long msec = ULONG_MAX;
      int succ = 1;
      for (int i = 0; i < REPEAT; i ++) {
        Result res;
        run_once(bench, &res);
        printf(res.pass ? "*" : "X");
        succ &= res.pass;
        if (res.msec < msec) msec = res.msec;
      }

      if (succ) printf(" Passed.");
      else printf(" Failed.");

      pass &= succ;

      unsigned long cur = score(bench, 0, msec);

      printf("\n");
      if (setting_id != 0) {
        printf("  min time: %d ms [%d]\n", (unsigned int)msec, (unsigned int)cur);
      }

      bench_score += cur;
    }
  }
  uint32_t t1 = uptime_ms();

  bench_score /= LENGTH(benchmarks);

  printf("==================================================\n");
  printf("MicroBench %s", pass ? "PASS" : "FAIL");
  if (setting_id == 2) {
    printf("        %d Marks\n", (unsigned int)bench_score);
    printf("                   vs. %d Marks (%s)\n", REF_SCORE, REF_CPU);
  } else {
    printf("\n");
  }
  printf("Total time: %d ms\n", t1 - t0);
  return 0;
}

// Libraries

void* bench_alloc(size_t size) {
  size  = (size_t)ROUNDUP(size, 8);
  char *old = hbrk;
  hbrk += size;
  assert((uintptr_t)heap.start <= (uintptr_t)hbrk && (uintptr_t)hbrk < (uintptr_t)heap.end);
  for (uint64_t *p = (uint64_t *)old; p != (uint64_t *)hbrk; p ++) {
    *p = 0;
  }
  assert((uintptr_t)hbrk - (uintptr_t)heap.start <= setting->mlim);
  return old;
}

void bench_free(void *ptr) {
}

static uint32_t seed = 1;

void bench_srand(uint32_t _seed) {
  seed = _seed & 0x7fff;
}

uint32_t bench_rand() {
  seed = (seed * (uint32_t)214013L + (uint32_t)2531011L);
  return (seed >> 16) & 0x7fff;
}

// FNV hash
uint32_t checksum(void *start, void *end) {
  const uint32_t x = 16777619;
  uint32_t h1 = 2166136261u;
  for (uint8_t *p = (uint8_t*)start; p + 4 < (uint8_t*)end; p += 4) {
    for (int i = 0; i < 4; i ++) {
      h1 = (h1 ^ p[i]) * x;
    }
  }
  int32_t hash = (uint32_t)h1;
  hash += hash << 13;
  hash ^= hash >> 7;
  hash += hash << 3;
  hash ^= hash >> 17;
  hash += hash << 5;
  return hash;
}
