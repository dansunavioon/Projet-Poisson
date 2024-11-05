#include <SDL2/SDL.h>
#include <vector>
#include "Poisson/Poisson.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int NUM_POISSONS = 100;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simulation de Poissons", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<Poisson> poissons;
    for (int i = 0; i < NUM_POISSONS; ++i) {
        poissons.emplace_back(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update and draw
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

    // Libération des ressources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}