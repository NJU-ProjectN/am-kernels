#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#define MAXCH       256
#define FPS          60
#define CHAR_PER_SEC 10

struct character {
  char ch;
  int x, y, v;
} chars[MAXCH];
int screen_width, screen_height, char_width, char_height;

int hit, miss, wrong;

int randint(int l, int r) {
  return l + (rand() & 0x7fffffff) % (r - l + 1);
}

void new_char() {
  for (int i = 0; i < LENGTH(chars); i++) {
    struct character *c = &chars[i];
    if (c->ch == '\0') {
      c->ch = 'A' + randint(0, 25);
      c->x = randint(0, screen_width - char_width);
      c->y = 0;
      c->v = (screen_height - char_height + 1) / randint(FPS * 3 / 2, FPS * 2);
      return;
    }
  }
}

void progress(int frame) {
  if (frame % (FPS / CHAR_PER_SEC) == 0) {
    new_char();
  }
  for (int i = 0; i < LENGTH(chars); i++) {
    struct character *c = &chars[i];
    if (c->ch) {
      c->y += c->v;
      if (c->y < 0) {
        hit++;
        c->ch = '\0';
      }
      if (c->y + char_height >= screen_height) {
        miss++;
        c->ch = '\0';
      }
    }
  }
}

void check_hit(char ch) {
  int m = -1;
  for (int i = 0; i < LENGTH(chars); i++) {
    struct character *c = &chars[i];
    if (ch == c->ch && c->v > 0 && (m == -1 || c->y > chars[m].y)) {
      m = i;
    }
  }
  if (m == -1) wrong++;
  else chars[m].v = -(screen_height - char_height + 1) / (FPS);
}

uint32_t px[32 * 16], blank[32 * 16];

void render() {
  static int x[MAXCH], y[MAXCH], n;

  for (int i = 0; i < n; i++) {
    io_write(AM_GPU_FBDRAW, x[i], y[i], blank, char_width, char_height, false);
  }

  n = 0;
  for (int i = 0; i < LENGTH(chars); i++) {
    struct character *c = &chars[i];
    if (c->ch) {
      x[n] = c->x; y[n] = c->y; n++;
      io_write(AM_GPU_FBDRAW, c->x, c->y, px, char_width, char_height, false);
    }
  }
  io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
}

int main() {
  char_width  = 8;
  char_height = 16;

  for (int i = 0; i < char_width * char_height; i ++) px[i] = 0x345678;
  for (int i = 0; i < char_width * char_height; i ++) blank[i] = 0;
  ioe_init();

  screen_width  = io_read(AM_GPU_CONFIG).width;
  screen_height = io_read(AM_GPU_CONFIG).height;

  int current = 0, rendered = 0;
  while (1) {
    int frames = io_read(AM_TIMER_UPTIME).us / (1000000 / FPS);

    while (current < frames) {
      progress(current);
      current++;

      while (1) {
        AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
        if (ev.keycode == AM_KEY_NONE) break;
        if (!ev.keydown) continue;
        switch (ev.keycode) {
          case AM_KEY_A: check_hit('A'); break;
          case AM_KEY_B: check_hit('B'); break;
          case AM_KEY_C: check_hit('C'); break;
          case AM_KEY_D: check_hit('D'); break;
          case AM_KEY_E: check_hit('E'); break;
          case AM_KEY_F: check_hit('F'); break;
          case AM_KEY_G: check_hit('G'); break;
          case AM_KEY_H: check_hit('H'); break;
          case AM_KEY_I: check_hit('I'); break;
          case AM_KEY_J: check_hit('J'); break;
          case AM_KEY_K: check_hit('K'); break;
          case AM_KEY_L: check_hit('L'); break;
          case AM_KEY_M: check_hit('M'); break;
          case AM_KEY_N: check_hit('N'); break;
          case AM_KEY_O: check_hit('O'); break;
          case AM_KEY_P: check_hit('P'); break;
          case AM_KEY_Q: check_hit('Q'); break;
          case AM_KEY_R: check_hit('R'); break;
          case AM_KEY_S: check_hit('S'); break;
          case AM_KEY_T: check_hit('T'); break;
          case AM_KEY_U: check_hit('U'); break;
          case AM_KEY_V: check_hit('V'); break;
          case AM_KEY_W: check_hit('W'); break;
          case AM_KEY_X: check_hit('X'); break;
          case AM_KEY_Y: check_hit('Y'); break;
          case AM_KEY_Z: check_hit('Z'); break;
        }
      };

      for (int i = 0; i < 80; i++) putch('\b');
      printf("Hit: %d; Miss: %d; Wrong: %d", hit, miss, wrong);
    }

    if (current > rendered) {
      render();
      rendered = current;
    }
  }
}
