#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"


SDL_Surface* picture = NULL; 
Uint32 pixel;
Uint8 *r, *g, *b;

SDL_Surface* picture_grey(SDL_Surface* picture);

int main()
{		
	picture = SDL_LoadBMP("test.bmp");
	picture = picture_grey(picture);
	SDL_SaveBMP(picture, "test_grey");
	return 0;
}

Uint32 getpixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel; 
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
		case 1:
				return *p;
				break;
		case 2:
				return *(Uint16 *)p;
				break;
		case 3:
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
						return p[0] << 16 | p[1] << 8 | p[2] << 16;
				break; 
		case 4:
				return *(Uint32 *)p;
				break; 
		default:
				return 0;
				break;
	}
}


void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
		int bpp = surface->format->BytesPerPixel;
		Uint8* p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

		switch (bpp)
		{
				case 1:
						*p = pixel;
						break;
				case 2:
						*(Uint16 *)p = pixel;
						break;
				case 3:
						if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
						{
								p[0] = (pixel << 16) & 0xff;
								p[1] = (pixel >> 8) & 0xff; 
								p[2] = pixel & 0xff;
						}
						else
						{
								p[0] = pixel & 0xff;
								p[1] = (pixel >> 8) & 0xff;
								p[2] = (pixel >> 16) & 0xff;
						}
						break;
				case 4:
						*(Uint32 *)p = pixel;
						break;
		}
}


SDL_Surface* picture_grey(SDL_Surface* picture)
{
	int width = picture->w;
	int height = picture->h; 
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			pixel = getpixel(picture, i, j);
			SDL_GetRGB(pixel, picture->format, r, g, b);
			int lum = 0.299 * ((int)r) + 0.587 *((int)g) + 0.114 * ((int)b); ; 
			pixel = SDL_MapRGB(picture->format, lum, lum, lum);
			putpixel(picture, i, j, pixel); 
		}
	return picture; 
}

