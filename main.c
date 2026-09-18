#include <unistd.h>
#include <stdio.h>
#include <string.h>
struct vec2 {
  int x, y;
};
struct ball {
  struct vec2 pos; 
  struct vec2 velocity;
  struct vec2 edge;
};
int main(int argc, char* argv[]) {
  const char* CLR = "\033[2J\033[H";
  struct ball b = {
    .pos = { 0 , 0 },
    .velocity = { 0 , 1 },
    .edge = { 10 , 10 },
  };
  for (;;) {
    if (b.pos.x == b.edge.x) {
      b.velocity.x = -b.velocity.x;
    }
    if (b.pos.y == b.edge.y) {
      b.velocity.y = -b.velocity.y;
    }
    b.pos.x += b.velocity.x;
    b.pos.y += b.velocity.y;
    char hint[50];
    sprintf(hint, "X: %d, Y: %d", b.pos.x, b.pos.y);
    printf("%s\n", hint);
    usleep(100000);
  }
  return 0;
}

