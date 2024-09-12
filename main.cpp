#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Erreur SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Quit();
    std::cout << "SDL initialisé avec succès!" << std::endl;
    return 0;
}

