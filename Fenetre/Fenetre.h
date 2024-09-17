#ifndef FENETRE_H
#define FENETRE_H

#include <SDL.h>

class Fenetre {

    SDL_Window* window;
    SDL_Renderer* renderer;
    int height;
    int width;

    public:
        Fenetre(SDL_Window* window, SDL_Renderer* renderer, int height, int width);
        int display();




};



#endif //FENETRE_H
