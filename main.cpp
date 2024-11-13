#include <SDL.h>
#include "Poisson/Poisson.h"
#include <vector>
#include <ctime>
#include <iostream> // Pour le débogage

int main(int argc, char* argv[])
{
    //Initialisation de SDL
    SDL_Init(SDL_INIT_VIDEO);
    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Simulation de poissons", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
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

    srand(static_cast<unsigned int>(time(0)));

    // Création des poissons avec des positions de départ aléatoires
    std::vector<Poisson> poissons;
    int nombrePoissons = 600; // Nombre de poissons
    for (int i = 0; i < nombrePoissons; ++i)
    {
        float randomX = static_cast<float>(rand() % 800); // Position X aléatoire
        float randomY = static_cast<float>(rand() % 600); // Position Y aléatoire
        bool independent = (rand() % 4 == 0);
        poissons.emplace_back(renderer, randomX, randomY, independent);
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
        for (const auto& poisson : poissons)
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