#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

#define BILLION 1000000000L
#define COL 80
#define ROW 24
#define TAIL 5
#define TAIL_CHAR "@0o+|."
#define AT(grid, y, x) ((grid)[(y) * COL + (x)])

struct vec2 {
  int x, y;
};
struct drop {
  struct vec2 head;
  int tail;
};

static struct timespec next;

void on_sigint(int sig);
void init_frame(void);
void sleep_frame(long frame_ns);
void init_drops(struct drop (*p)[]);
void update_drops(struct drop (*p)[]);
void render(char *buf);
void draw(struct drop (*p)[COL] , char *buf);

int main() {
  srand(time(NULL));
  signal(SIGINT, on_sigint);
  const char CLR[] = "\033[2J\033[H";
  const char START[] = "\x1b[?1049h\x1b[?25l";
  const char END[] = "\x1b[?1049l\x1b[?25h";

  char buf[COL*ROW];
  struct drop drops[COL];
  
  write(1, START, strlen(START));
  init_frame();
  init_drops(&drops);

  for (;;) {
    update_drops(&drops);
    draw(&drops, buf);
    render(buf);
    sleep_frame(BILLION/10);
  }
  write(1, END, strlen(END));
  return 0;
}

void draw(struct drop (*p)[COL] , char *buf) {
  const char tail_char[] = TAIL_CHAR; //eg: @0o+|.  len=7
  for (int i = 0; i < ROW*COL; i++) {
    buf[i] = ' ';
  }
  for (int i = 0; i < COL; i++) {
      for (int t = 0; t <= TAIL; t++) {
          if (t > strlen(tail_char) - 2) {
          AT(buf, (*p)[i].head.y - t, (*p)[i].head.x) = '.';
        } else {
          AT(buf, (*p)[i].head.y - t, (*p)[i].head.x) = tail_char[t];
        }
      }
  }
}

void render(char *buf) {
    char frame[ROW * (COL + 2) + 256];
    int pos = 0;

    memcpy(frame + pos, "\x1b[H", 3);
    pos += 3;
    for (int y = 0; y < ROW; y++) {
        memcpy(frame + pos, buf + y * COL, COL);
        pos += COL;
        if (y < ROW - 1) {
            frame[pos++] = '\n';
        }
    }
    write(1, frame, pos);

}

void on_sigint(int sig) {
  const char END[] = "\x1b[?1049l\x1b[?25h";
  write(1, END, strlen(END));
  _exit(0);
}

void init_drops(struct drop (*p)[]) {
  for (int i = 0; i < COL; i++) {
    (*p)[i].head.x = i;
    (*p)[i].head.y = rand() % (ROW/2);
    (*p)[i].tail = TAIL;
  }
}
void update_drops(struct drop (*p)[]) {
  for (int i = 0; i < COL; i++) {
    if ((*p)[i].head.y == ROW && (*p)[i].tail == 0) {
      (*p)[i].head.y = rand() % (ROW/2);
      (*p)[i].tail = TAIL;
    } else if ((*p)[i].head.y == ROW) {
      (*p)[i].tail -= 1;
    } else {
      (*p)[i].head.y += 1;
    }
  }
}
void init_frame(void) {
  clock_gettime(CLOCK_MONOTONIC, &next);
}

void sleep_frame(long frame_ns) {
  struct timespec now;
  next.tv_nsec += frame_ns;

  if (next.tv_nsec > BILLION) {
    next.tv_nsec -= BILLION;
    next.tv_sec += 1;
  }

  clock_gettime(CLOCK_MONOTONIC, &now);
  long sleep_ns = next.tv_nsec - now.tv_nsec;

  if(sleep_ns > 0) {
    struct timespec req = {0,sleep_ns};
    nanosleep(&req, NULL);
  } else {
    clock_gettime(CLOCK_MONOTONIC, &next);
  }
}
