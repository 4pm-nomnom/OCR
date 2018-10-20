#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

void Save_image(SDL_Surface* image, char* name)
{
		if((SDL_SaveBMP(image, name)) != 0)
			exit(EXIT_FAILURE); 
}

 
