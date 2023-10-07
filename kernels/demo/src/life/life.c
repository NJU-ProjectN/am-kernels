/* adapted from http://rosettacode.org/wiki/Conway%27s_Game_of_Life */

#include <stdio.h>
#include <stdlib.h>
#include <io.h>

static int w = 40, h = 25;
static unsigned **univ = NULL;
static unsigned **new = NULL;

void show(void) {
  int x,y;
  screen_clear();
  for (y=0;y<h;y++) {
    for (x=0;x<w;x++)
      print_char(univ[y][x] ? 'o' : ' ', y, x);
  }
  screen_refresh();
}

void evolve(void) {
  int x,y,x1,y1;

  for (y=0;y<h;y++) for (x=0;x<w;x++) {
    int n = 0;
    for (y1 = y - 1; y1 <= y + 1; y1++)
      for (x1 = x - 1; x1 <= x + 1; x1++)
        if (univ[(y1 + h) % h][(x1 + w) % w])
          n++;

    if (univ[y][x]) n--;
    new[y][x] = (n == 3 || (n == 2 && univ[y][x]));
  }
  for (y=0;y<h;y++) for (x=0;x<w;x++) univ[y][x] = new[y][x];
}

void game_of_life(void) {
  w = screen_tile_width();
  h = screen_tile_height();

  univ = malloc(h * sizeof(unsigned*));
  new = malloc(h * sizeof(unsigned*));
  for (int i = 0; i < h; i++) {
    univ[i] = malloc(w * sizeof(unsigned));
    new[i] = malloc(w * sizeof(unsigned));
  }
  
  int x,y;
  for (x=0;x<w;x++)
    for (y=0;y<h;y++) 
      univ[y][x] = rand() % 2;
  while (1) {
    show();
    evolve();
    usleep(200000);
  }
}
