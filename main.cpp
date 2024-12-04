#include "Fenetre/Fenetre.h"
#include "Poisson/Poisson.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <mutex>

std::vector<Poisson> poissons;
std::mutex poissonsMutex;

Uint32 updatePoissons(Uint32 interval, void* param)
{
    std::lock_guard<std::mutex> lock(poissonsMutex);
    for (auto& poisson : poissons) {
        poisson.update(poissons);
    }
    return interval;
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cerr << "Erreur SDL_Init : " << SDL_GetError() << std::endl;
        return -1;
    }

    Fenetre fenetre("Poissons", 800, 600);
    if (!fenetre.estValide())
    {
        SDL_Quit();
        return -1;
    }

    srand(static_cast<unsigned int>(time(0)));

    int nombrePoissons = 200;
    for (int i = 0; i < nombrePoissons; ++i)
    {
        float randomX = static_cast<float>(rand() % 800);
        float randomY = static_cast<float>(rand() % 600);
        bool independent = (rand() % 4 == 0);
        poissons.emplace_back(fenetre.obtenirRenderer(), randomX, randomY, independent);
    }

    const int updateInterval = 16;
    SDL_TimerID timerID = SDL_AddTimer(updateInterval, updatePoissons, nullptr);
    if (!timerID)
    {
        std::cerr << "Erreur de création du timer : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        fenetre.effacer();

        {
            std::lock_guard<std::mutex> lock(poissonsMutex);
            for (const auto& poisson : poissons)
            {
                poisson.draw(fenetre.obtenirRenderer());
            }
        }

        fenetre.afficher();
    }

    SDL_RemoveTimer(timerID);
    SDL_Quit();
    return 0;
}