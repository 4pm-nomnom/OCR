#include <stdlib.h>
#include <stdio.h>
//#include <SDL.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "image.h"
#include "file_operation.h"

SDL_Surface* picture;
SDL_Surface* screen_surface;
Uint32 pixel;
Uint8 r, g, b;

SDL_Surface* picture_grey(SDL_Surface* picture);
SDL_Surface* picture_black(SDL_Surface* picture);
SDL_Surface* load_image(char* path);

int main()
{
  init_sdl();
  picture = load_image("texte3.png");
  picture = picture_grey(picture);
  picture = picture_black(picture);
  screen_surface = display_image(picture);
  wait_for_keypressed();
  Save_image(picture, "prout.bmp");
  //printf("hihi");
  return 0;
}

SDL_Surface* picture_grey(SDL_Surface *picture)
{
  int width = picture->w;
  int height = picture->h;
  for (unsigned i = 0; i < width; i++)
       for (unsigned j = 0; j < height; j++)
    {
      pixel = get_pixel(picture, i, j); 
      SDL_GetRGB(pixel, picture->format, &r, &g, &b);
      int lum = 0.299 * ((int)r) + 0.587 *((int)g) + 0.114 * ((int)b);
      pixel = SDL_MapRGB(picture->format, lum, lum, lum);
      put_pixel(picture, i, j, pixel);
    }
  return picture;
}

SDL_Surface* picture_black(SDL_Surface *picture)
{
  int width = picture->w;
  int height = picture->h;
  for (unsigned i = 0; i < width; i++)
       for (unsigned j = 0; j < height; j++)
    {
      pixel = get_pixel(picture, i, j); 
      SDL_GetRGB(pixel, picture->format, &r, &g, &b);
      int color = r >= 127? 255 : 0; 
	  pixel = SDL_MapRGB(picture->format,color, color, color);
      put_pixel(picture, i, j, pixel);
    }
  return picture;
}

