#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define EPS 0.00001

const Uint32 colors[21] = {
  0xFFFFFFFF, // white
  0xFF0000FF, // red
  0x00FF00FF, // green
  0x0000FFFF, // blue
  0xFFFF00FF, // yellow
  0x00FFFFFF, // cyan
  0xA50EFFFF, // magenta
  0xFF7400FF, // orange
  0x531DFFFF, // dark blue
  0xFF0090FF, // purple
  0x1B60FFFF, // light blue

  0xFF000060, // red transp
  0x00FF0060, // green transp
  0x0000FF60, // blue transp
  0xFFFF0060, // yellow transp
  0x00FFFF60, // cyan transp
  0xA50EFF60, // magenta transp
  0xFF740060, // orange transp
  0x531DFF60, // dark blue transp
  0xFF009060, // purple transp
  0x1B60FF60, // light blue transp
};

SDL_Surface* screen;

struct Point
{
  int x, y;
  int cluster;
};

struct Cluster
{
  Point core;
  int* points_list;
  int points_count;
  int max_points_count;
  int color;
};

Point points[100000];
int countPoints = 0;

Cluster clusters[100];
int cluster_count;
int gen_points_count;

void init_sdl()
{
  if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD ) < 0 )
  {
    fprintf(stderr, "Unable to init SDL: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  
  screen = SDL_SetVideoMode(1024, 710, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if ( !screen )
  {
    fprintf(stderr, "Unable to set videomode: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }
}

void drawCircle(int x, int y, int r, Uint32 color)
{
  filledCircleColor(screen, x, y, r, color);
}

void putPoint(int x, int y, int color)
{
  drawCircle(x, y, 3, colors[color]);
}

void add_to_cluster(int point, int cluster_index)
{
  Cluster* cluster = &clusters[cluster_index];
  if ( cluster->points_count >= cluster->max_points_count )
  {
    cluster->max_points_count *= 2;
    cluster->points_list = (int*) realloc( cluster->points_list, sizeof(int) * cluster->max_points_count );
  }
  cluster->points_list[ cluster->points_count++ ] = point;
}

void init_clusters(int count)
{ 
  for ( int i = 0; i < count; i++ )
  {
    clusters[i].core = points[i];
    clusters[i].max_points_count = 100;
    clusters[i].points_list = (int*) malloc( sizeof(int) * clusters[i].max_points_count );
    clusters[i].points_count = 0;
    clusters[i].color = i;
  }
}

void change_core(int cluster_index, int point_index)
{
  Cluster* cluster = &clusters[cluster_index];
  int core_index = 0;
  for (int i = 0; i < countPoints; i++)
  {
    Point p = points[i];
    if ( ( p.x - cluster->core.x ) == 0 && ( p.y - cluster->core.y ) == 0 )
    {
      core_index = i;
      break;
    }
  }
  add_to_cluster(core_index, cluster_index);

  cluster->core = points[ cluster->points_list[ point_index ] ];
  for (int i = point_index; i < cluster->points_count - 1; i++)
    cluster->points_list[i] = cluster->points_list[i+1];
  cluster->points_count--;
}

void draw_cluster(Cluster* cluster)
{
  for (int i = 0; i < cluster->points_count; i++)
  {
    Point p = points[ cluster->points_list[i] ];
    putPoint(p.x, p.y, cluster->color + 1); 
  }
  putPoint(cluster->core.x, cluster->core.y, 0);
}

void render()
{
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  for ( int i = 0; i < cluster_count; i++ )
    draw_cluster( &clusters[i] );
}

void genPoints()
{
  srand(time(NULL));
  for ( int i=0; i<gen_points_count; i++ )
  {
    int x = rand()%1024 + 1;
    int y = rand()%710 + 1;
    points[countPoints++] = {x, y};
  }
}

void regenerate_clusters()
{
  for ( int i = 0; i < cluster_count; i++ )
    clusters[i].points_count = 0;

  for ( int i = 0; i < countPoints; i++ )
  {
    Point p = points[i];
    double min_distance = 9999999;
    int min_distance_index = 0;
    for ( int j = 0; j < cluster_count; j++ )
    {
      double distance = ( clusters[j].core.x - p.x ) * ( clusters[j].core.x - p.x ) + ( clusters[j].core.y - p.y ) * ( clusters[j].core.y - p.y );
      if ( distance < min_distance )
      {
        min_distance = distance;
        min_distance_index = j;
      }
    }
    if ( min_distance > EPS )
      add_to_cluster(i, min_distance_index);
  }
}

bool recalculate_cores()
{
  bool res = false;
  for ( int i = 0; i < cluster_count; i++ )
  {
    double sumx = 0.0f, sumy = 0.0f;
    Cluster* cluster = &clusters[i];
    for ( int j = 0; j < cluster->points_count; j++ )
    {
      sumx += points[ cluster->points_list[j] ].x;
      sumy += points[ cluster->points_list[j] ].y;
    }
    sumx += cluster->core.x; sumy += cluster->core.y;
    double avx = sumx / (cluster->points_count + 1), avy = sumy / (cluster->points_count + 1);
    
    double min_distance = 9999999;
    int min_distance_index = 0;
    for ( int j = 0; j < cluster->points_count; j++ )
    {
      double distance = ( points[ cluster->points_list[j] ].x - avx ) * ( points[ cluster->points_list[j] ].x - avx ) + 
        ( points[ cluster->points_list[j] ].y - avy ) * ( points[ cluster->points_list[j] ].y - avy );
      if ( distance < min_distance )
      {
        min_distance = distance;
        min_distance_index = j;
      }
    }
    double core_distance = ( cluster->core.x - avx ) * ( cluster->core.x - avx ) + 
      ( cluster->core.y - avy ) * ( cluster->core.y - avy );
    if ( min_distance < core_distance )
    {
      change_core(i, min_distance_index);
      res = true;
    }
  }
  return !res;
}

int main(int argc, char** argv)
{
  gen_points_count = atoi(argv[1]);
  cluster_count = atoi(argv[2]);
  init_sdl();
  bool quit = false, done = false;
  int iterations = 0;
  genPoints();
  init_clusters(cluster_count);
  regenerate_clusters();
  while ( !quit )
  {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch ( event.type )
    {
      case SDL_QUIT:
        quit = true;
        break;
    }
    if ( !done )
    {
      done = recalculate_cores();
      regenerate_clusters();

      if ( done )
        printf("Done! %d iterations\n", iterations);
      if ( iterations >= countPoints )
      {
        printf("reached limit, stopping\n");
        done = true;
      }
      iterations++;
      render();
      SDL_Flip(screen);
    }
  }
  return 0;
}
