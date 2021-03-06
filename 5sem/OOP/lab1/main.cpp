#include "main_controller.h"
#include <iostream>
#include <dlfcn.h>

int main(int argc, const char *argv[])
{
  Main_controller* mc = new Main_controller();
  FILE* lib_paths = popen("find $(pwd) -name \"*.so\"", "r");
  if( !lib_paths )
  {
    perror("popen");
    exit(-1);
  }
  char name[1024];
  char in_buf[1024];
  while( fgets(in_buf, 1024, lib_paths) )
  {
    char *ws = strpbrk(in_buf, " \t\n");
    if (ws)
      *ws = '\0';

    // sprintf(name, "./%s", in_buf);
    void* dlib = dlopen(in_buf, RTLD_NOW);
    if( dlib == NULL )
      cerr << dlerror() << endl;
    void* maker_func = dlsym(dlib, "maker");
    void* mode_func = dlsym(dlib, "draw_mode");
    if ( maker_func == NULL  || mode_func == NULL )
      fprintf(stderr, "Can't resolve 'maker' or 'draw_mode' in %s\n", in_buf);
    else
    {
      mc->add_figure_creator((maker_function) maker_func, (draw_mode_getter) mode_func);
      printf("adding %s\n", in_buf);
    }

  }
  pclose(lib_paths);

  mc->main_loop();
  delete mc;
  return 0;
}
