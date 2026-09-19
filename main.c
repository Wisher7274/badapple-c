#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
struct vec2 {
  int x, y;
};
struct ball {
  struct vec2 pos; 
  struct vec2 velocity;
  struct vec2 edge;
};
void draw(struct ball b);
void on_sigint(int sig);

int main() {
  signal(SIGINT, on_sigint);
  const char CLR[] = "\033[2J\033[H";
  const char START[] = "\x1b[?1049h\x1b[?25l";
  const char END[] = "\x1b[?1049l\x1b[?25h";

  struct ball b = {
    .pos = { 0 , 0 },
    .velocity = { 2 , 1 },
    .edge = { 10 , 10 },
  };

  write(1, START, strlen(START));
  for (;;) {
    if (b.pos.x + b.velocity.x > b.edge.x || b.pos.x + b.velocity.x < 0) {
      b.velocity.x = -b.velocity.x;
    }
    if (b.pos.y + b.velocity.y > b.edge.y || b.pos.y + b.velocity.y < 0) {
      b.velocity.y = -b.velocity.y;
    }
    b.pos.x += b.velocity.x;
    b.pos.y += b.velocity.y;
    draw(b);
    usleep(100000);
  }
  write(1, END, strlen(END));
  return 0;
}

void draw(struct ball b) {
  char buf[64];
  sprintf(buf, "\033[2J\033[H\x1b[%d;%dHO", b.edge.y - b.pos.y + 1, b.pos.x + 1);
  write(1, buf, strlen(buf));
}

void on_sigint(int sig) {
  const char END[] = "\x1b[?1049l\x1b[?25h";
  write(1, END, strlen(END));
  _exit(0);
}
