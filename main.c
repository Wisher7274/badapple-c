#include <unistd.h>
int main(void) {
  const char *CR = "\r";
  char *frames[10];
  char ch[] = {'0','1','2','3','4','5','6','7','8','9'};
  
  for (int i = 0;i < 10; i++) {
    frames[i] = &ch[i];
  }
  for (int i = 0;;i++) {
    if (10 == i) {i = 0;}
    write(1,CR,1);
    write(1,frames[i],1);
    usleep(100000);
  }
  return 0;
}
