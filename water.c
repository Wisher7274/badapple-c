#include <time.h>

static struct timespec next_tick;

void set_tick(struct timespec *tick) {
  clock_gettime(CLOCK_MONOTONIC , tick);
}

void sleep_by_tick(long frame_ns) {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC , &now);
  next_tick.tv_nsec += frame_ns;

  if (next_tick.tv_nsec > 1000000000L) {
    next_tick.tv_sec += 1;
    next_tick.tv_nsec -= 1000000000L;
  }

  long sleep_ns = (next_tick.tv_sec - now.tv.sec)*1000000000L + next_tick.tv_nsec - now.tv_nsec;
  if (sleep_ns > 0) {
  struct timespec sleep_spec = {0 , sleep_ns};
  } else {
    clock_gettime(CLOCK_MONOTONIC , &next_tick);
  }
}
