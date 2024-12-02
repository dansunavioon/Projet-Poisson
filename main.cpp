#include <SDL.h>
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
    for (auto& poisson : poissons)
    {
        poisson.update(poissons);
    }
    return interval;
}

// Dimensions de la carte
const int MAP_WIDTH = 3000;
const int MAP_HEIGHT = 2000;

// Dimensions de la caméra
const int CAMERA_WIDTH = 800;
const int CAMERA_HEIGHT = 600;

// Position initiale de la caméra
SDL_Point cameraPosition = {0, 0};

int SDL_main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Erreur lors de l'initialisation de SDL : " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Simulation de poissons",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,CAMERA_WIDTH, CAMERA_HEIGHT,SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Erreur lors de la création de la fenêtre : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Erreur lors de la création du renderer : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    srand(static_cast<unsigned int>(time(0)));

    int nombrePoissons = 200;
    for (int i = 0; i < nombrePoissons; ++i)
    {
        float randomX = static_cast<float>(rand() % 800);
        float randomY = static_cast<float>(rand() % 600);
        bool independent = (rand() % 6 == 0);
        poissons.emplace_back(renderer, randomX, randomY, independent);
    }

    const int updateInterval = 16;
    SDL_TimerID timerID = SDL_AddTimer(updateInterval, updatePoissons, nullptr);
    if (!timerID)
    {
        std::cerr << "Erreur de création du timer : " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool running = true;
    SDL_Event event;
    const int cameraSpeed = 8;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Contrôle de la caméra avec les touches fléchées
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_UP]) cameraPosition.y -= cameraSpeed;
        if (keystate[SDL_SCANCODE_DOWN]) cameraPosition.y += cameraSpeed;
        if (keystate[SDL_SCANCODE_LEFT]) cameraPosition.x -= cameraSpeed;
        if (keystate[SDL_SCANCODE_RIGHT]) cameraPosition.x += cameraSpeed;

        // S'assurer que la caméra reste dans les limites de la carte
        if (cameraPosition.x < 0) cameraPosition.x = 0;
        if (cameraPosition.y < 0) cameraPosition.y = 0;
        if (cameraPosition.x > MAP_WIDTH - CAMERA_WIDTH) cameraPosition.x = MAP_WIDTH - CAMERA_WIDTH;
        if (cameraPosition.y > MAP_HEIGHT - CAMERA_HEIGHT) cameraPosition.y = MAP_HEIGHT - CAMERA_HEIGHT;

        for (auto& poisson : poissons)
        {
            poisson.update(poissons);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Bleu pour la mer
        SDL_RenderClear(renderer);
        {
            std::lock_guard<std::mutex> lock(poissonsMutex);
            for (const auto& poisson : poissons)
            {
                poisson.draw(renderer, cameraPosition);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_RemoveTimer(timerID);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
