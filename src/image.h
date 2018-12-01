#ifndef IMAGE_H
#define IMAGE_H
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdlib.h>
#include <SDL.h>

void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void wait_for_keypressed();

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);

void Surface_save_image(SDL_Surface* image, char* name);

SDL_Surface *ScaleSurface(SDL_Surface *surface, size_t width, size_t height);

/* SDL_FreeSurface(image_surface); to close
 * int width = image_surface->w;
 * int height = image_surface->h;
 * int SDL_BlitSurface(SDL_Surface*    src,
 *                   const SDL_Rect* srcrect,
 *                   SDL_Surface*    dst,
 *                   SDL_Rect*       dstrect) // used to crop/copy a surface
 *
 * int SDL_BlitScaled(SDL_Surface*    src,
 *                  const SDL_Rect* srcrect,
 *                  SDL_Surface*    dst,
 *                  SDL_Rect*       dstrect)
 */
#endif
