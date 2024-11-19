#include <SDL.h>
#include "Poisson/Poisson.h"
#include <vector>
#include <ctime>
#include <iostream>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simulation de poissons", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Erreur de création de la fenêtre : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Erreur de création du renderer : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    srand(static_cast<unsigned int>(time(0)));
    std::vector<Poisson> poissons;
    int nombrePoissons = 50;
    for (int i = 0; i < nombrePoissons; ++i) {
        float randomX = static_cast<float>(rand() % 800);
        float randomY = static_cast<float>(rand() % 600);
        bool independent = (rand() % 4 == 0);
        poissons.emplace_back(renderer, randomX, randomY, independent);
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        for (auto& poisson : poissons) {
            poisson.update(poissons);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (const auto& poisson : poissons) {
            poisson.draw(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}