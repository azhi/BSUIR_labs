#include "main_controller.h"
#include <time.h>

int main(int argc, const char *argv[])
{
  srand(time(NULL));
  Main_controller* mc = new Main_controller();
  delete mc;
  return 0;
}
