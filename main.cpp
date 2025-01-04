#include "SDL.h"
#include "Fenetre/Fenetre.h"

int main(int argc, char* argv[]) {

    SDL_Window* window = SDL_CreateWindow("Poisson", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Créer et afficher une fenêtre

    Map* map = new Map(renderer, MAP_WIDTH, MAP_HEIGHT);

    Fenetre* fenetre = new Fenetre(window, renderer, map, SCREEN_HEIGHT, SCREEN_WIDTH);

    fenetre->display();

    return 0;
}

