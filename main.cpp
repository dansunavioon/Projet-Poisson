#include <SDL.h>
#include "Poisson/Poisson.h"
#include <vector>
#include <ctime>
#include <iostream> // Pour le débogage

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simulation de poissons", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cerr << "Erreur de création du renderer : " << SDL_GetError() << std::endl;
        return -1;
    }

    srand(static_cast<unsigned int>(time(0)));

    // Créer une liste de poissons
    std::vector<Poisson> poissons;
    for (int i = 0; i < 10; ++i) {
        bool independent = (rand() % 2 == 0);
        poissons.emplace_back(renderer, rand() % 800, rand() % 600, independent);
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Mise à jour de chaque poisson
        for (auto& poisson : poissons) {
            poisson.update(poissons);
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dessiner chaque poisson
        for (const auto& poisson : poissons) {
            poisson.draw(renderer);
        }

        // Afficher le rendu
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Ajout d'un léger délai pour limiter la boucle de rendu
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}