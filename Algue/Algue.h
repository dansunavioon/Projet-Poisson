#ifndef ALGUE_H
#define ALGUE_H

#include <SDL_render.h>

class Algue {
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int height;
    int width;
    int x;
    int y;

    void display();

    public:
        Algue(SDL_Renderer* renderer, int height, int width, int x, int y);
};


#endif //ALGUE_H
