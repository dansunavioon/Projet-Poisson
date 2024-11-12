//
// Created by Florian BOISDRON on 05/11/2024.
//

#ifndef ROCHE_H
#define ROCHE_H


#include <SDL_render.h>

class Roche {
    SDL_Renderer* renderer;
    int height;
    int width;
    int x;
    int y;

    void display() const;

    public:
        Roche(SDL_Renderer* renderer, int height, int width, int x, int y);
};

#endif //ROCHE_H
