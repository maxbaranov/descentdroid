#include <stdio.h>
#include <assert.h>

int  gameloop_init(int argc, char *argv[]);
int  gameloop_tick(void);
void gameloop_cleanup(void);

int main(int argc, char *argv[]) {
  int r;

  r = gameloop_init(argc,argv);
  assert(r == 0);

  while( gameloop_tick() == 0 ) {
    printf("Tick...\n");
  }
  
  gameloop_cleanup();

  return 0;
}
