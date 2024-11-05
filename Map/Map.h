#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include "../TypeDef.h"

class Map {

    SDL_Window* window;
    SDL_Renderer* renderer;
    int height;
    int width;

    public:
        Map(SDL_Window* window, SDL_Renderer* renderer, int height, int width);
        int display();
};



#endif //MAP_H
