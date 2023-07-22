#include <am.h>
#include <klib.h>
#include <klib-macros.h>

void ant();
void galton();
void hanoi();
void game_of_life();
void aclock();
void cmatrix();
void donut();

int main(const char *args) {
  ioe_init();

  switch (args[0]) {
    case '1': ant(); break;
    case '2': galton(); break;
    case '3': hanoi(); break;
    case '4': game_of_life(); break;
    case '5': aclock(); break;
    case '6': cmatrix(); break;
    case '7': donut(); break;
    default:
      printf("Usage: make run mainargs=*\n");
      printf("  1: ant\n");
      printf("  2: galton\n");
      printf("  3: hanoi\n");
      printf("  4: game of life\n");
      printf("  5: aclock\n");
      printf("  6: cmatrix\n");
      printf("  7: donut\n");
  }

  printf("Press Q to Exit\n");
  while (1) {
    AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
    if (ev.keydown && ev.keycode == AM_KEY_Q) break;
  }
  return 0;
}
