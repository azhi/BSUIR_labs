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
  bool is_core;
};

struct Cluster
{
  Point core;
  int* points_list;
  int points_count;
  int max_points_count;
  int color;
};

Point points[1000000];
int countPoints = 0;

Cluster clusters[100];
int cluster_count = 0;
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

void init_cluster(int point_index)
{
  clusters[cluster_count].core = points[point_index];
  points[point_index].is_core = true;
  clusters[cluster_count].max_points_count = 100;
  clusters[cluster_count].points_list = (int*) malloc( sizeof(int) * clusters[cluster_count].max_points_count );
  clusters[cluster_count].points_count = 0;
  clusters[cluster_count].color = cluster_count;
  cluster_count++;
}

void init_clusters()
{ 
  init_cluster(0);
  double max_distance = 0;
  int max_distance_index = 0;
  int i;
  for (i = 1; i < countPoints; i++)
  {
    double distance = ( points[i].x - points[0].x ) * ( points[i].x - points[0].x ) + ( points[i].y - points[0].y ) * ( points[i].y - points[0].y );
    if ( distance > max_distance )
    {
      max_distance = distance;
      max_distance_index = i;
    }
  }
  init_cluster(max_distance_index);
}

void draw_cluster(Cluster* cluster)
{
  for (int i = 0; i < cluster->points_count; i++)
  {
    Point p = points[ cluster->points_list[i] ];
    putPoint(p.x, p.y, cluster->color + 1); 
  }
  drawCircle(cluster->core.x, cluster->core.y, 9, colors[0]);
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

bool maximin_clusters()
{
  bool res = false;
  double maximin_distance = 0;
  int maximin_distance_index = 0;
  for ( int j = 0; j < countPoints; j++ )
    if ( !points[j].is_core )
    {
      double min_distance = 9999999;
      int min_distance_index = 0;
      for ( int i = 0; i < cluster_count; i++ )
      {
        Cluster* cluster = &clusters[i];
        double distance = ( points[j].x - cluster->core.x ) * ( points[j].x - cluster->core.x ) + 
          ( points[j].y - cluster->core.y ) * ( points[j].y - cluster->core.y );
        if ( distance < min_distance && distance > EPS ) 
        {
          min_distance = distance;
          min_distance_index = j;
        }
      }

      if ( min_distance > maximin_distance )
      {
        maximin_distance = min_distance;
        maximin_distance_index = min_distance_index;
      }
    }

  int count = 0;
  double sum = 0;
  for ( int i = 0; i < cluster_count; i++ )
    for ( int j = 0; j < cluster_count; j++ )
      if ( i != j )
      {
        count++;
        sum += sqrt( (clusters[i].core.x - clusters[j].core.x) * (clusters[i].core.x - clusters[j].core.x) + 
          (clusters[i].core.y - clusters[j].core.y) * (clusters[i].core.y - clusters[j].core.y) );
      }
  double av_distance = sum / count;

  if ( sqrt(maximin_distance) > av_distance / 2 )
  {
    res = true;
    init_cluster(maximin_distance_index);
  }
  else
  {
    res = false;
  }
  return !res;
}

int main(int argc, char** argv)
{
  gen_points_count = atoi(argv[1]);
  init_sdl();
  bool quit = false, done = false;
  int iterations = 0;
  genPoints();
  init_clusters();
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
      done = maximin_clusters();
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
