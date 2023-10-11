/* from http://rosettacode.org/wiki/Towers_of_Hanoi#C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#define H 7

typedef struct { int *x, n; } tower;
static tower *new_tower(int cap) {
  int size = sizeof(tower) + sizeof(int) * cap;
  tower *t = malloc(size);
  memset(t, 0, size);
  t->x = (int*)(t + 1);
  return t;
}

static tower *t[3];

static void text(int y, int i, int d, const char *s) {
  int yy = H - y + 1;
  int xx = (H + 1) * (2 * i + 1) - d;
  while (d--) {
    for (const char *p = s; *p; p ++) {
      print_char(*p, yy, xx ++);
    }
  }
}

static void add_disk(int i, int d) {
  t[i]->x[t[i]->n++] = d;
  text(t[i]->n, i, d, "==");
  screen_refresh();
  usleep(100000);
}

int remove_disk(int i) {
  int d = t[i]->x[--t[i]->n];
  text(t[i]->n + 1, i, d, "  ");
  return d;
}

void move(int n, int from, int to, int via) {
  if (!n) return;

  move(n - 1, from, via, to);
  add_disk(to, remove_disk(from));
  move(n - 1, via, to, from);
}

void hanoi() {
  screen_clear();

  int c;
  for (c = 0; c < 3; c++)	 t[c] = new_tower(H);
  for (c = H; c; c--) add_disk(0, c);

  move(H, 0, 2, 1);
}
