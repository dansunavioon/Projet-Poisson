#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib> // Pour rand() et RAND_MAX
#include "Poisson/Poisson.h"

int main(int argc, char* argv[])
{
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Erreur d'initialisation de SDL : " << SDL_GetError() << std::endl;
        return -1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Simulation de Poissons", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Erreur de création de la fenêtre : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Création du renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Erreur de création du renderer : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Création des poissons avec des positions de départ aléatoires
    std::vector<Poisson> poissons;
    int nombrePoissons = 600; // Nombre de poissons
    for (int i = 0; i < nombrePoissons; ++i)
    {
        float randomX = static_cast<float>(rand() % 800); // Position X aléatoire
        float randomY = static_cast<float>(rand() % 600); // Position Y aléatoire
        poissons.emplace_back(randomX, randomY);
    }

    // Boucle principale
    bool running = true;
    SDL_Event event;

    while (running)
    {
        // Gestion des événements
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Mise à jour des poissons
        for (auto& poisson : poissons)
        {
            poisson.update(poissons);
        }

        // Efface l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dessine les poissons
        for (auto& poisson : poissons)
        {
            poisson.draw(renderer);
        }

        // Présente le rendu
        SDL_RenderPresent(renderer);
    }

    // Libération des ressources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
