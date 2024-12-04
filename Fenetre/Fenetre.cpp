#include "Fenetre.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

Fenetre::Fenetre(const std::string& titre, int largeur, int hauteur)
    : window(nullptr), renderer(nullptr), valide(false), height(hauteur), width(largeur)
{
    window = SDL_CreateWindow(titre.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, largeur, hauteur, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Erreur de création de la fenêtre : " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Erreur de création du renderer : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        window = nullptr;
        return;
    }

    camera = { 0, 0, largeur, hauteur }; // Initialisation de la caméra
    valide = true;
}

Fenetre::~Fenetre()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
}

bool Fenetre::estValide() const
{
    return valide;
}

SDL_Renderer* Fenetre::obtenirRenderer() const
{
    return renderer;
}

void Fenetre::effacer()
{
    if (renderer)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
}

void Fenetre::afficher()
{
    if (renderer)
    {
        SDL_RenderPresent(renderer);
    }
}

void Fenetre::updateCamera(const SDL_Rect& personnageRect)
{
    camera.x = personnageRect.x + personnageRect.w / 2 - camera.w / 2;
    camera.y = personnageRect.y + personnageRect.h / 2 - camera.h / 2;

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x > MAP_WIDTH - camera.w) camera.x = MAP_WIDTH - camera.w;
    if (camera.y > MAP_HEIGHT - camera.h) camera.y = MAP_HEIGHT - camera.h;
}

int Fenetre::display()
{
    if (!estValide()) return EXIT_FAILURE;

    srand(static_cast<unsigned int>(time(nullptr)));

    Personnage plongeur(renderer);

    std::vector<Poisson> poissons;
    int nombrePoissons = 200;
    for (int i = 0; i < nombrePoissons; ++i)
    {
        float randomX = static_cast<float>(rand() % 800);
        float randomY = static_cast<float>(rand() % 600);
        bool independent = (rand() % 4 == 0);
        poissons.emplace_back(renderer, randomX, randomY, independent);
    }

    SDL_Event events;
    bool isOpen = true;

    Uint32 animationTimer = 0;
    const Uint32 animationDelay = 100;

    while (isOpen)
    {
        while (SDL_PollEvent(&events))
        {
            if (events.type == SDL_QUIT)
            {
                isOpen = false;
            }
            else
            {
                plongeur.handleInput(events);
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - animationTimer >= animationDelay)
        {
            plongeur.update();
            animationTimer = currentTime;
        }

        for (auto& poisson : poissons)
        {
            poisson.update(poissons);
        }

        SDL_Rect personnageRect = { plongeur.getX(), plongeur.getY(), TAILLE_PLONGEUR, TAILLE_PLONGEUR };
        updateCamera(personnageRect);

        effacer();

        for (int y = 0; y < camera.h; y++)
        {
            int blueValue = 255 - ((camera.y + y) * 255 / MAP_HEIGHT);
            SDL_SetRenderDrawColor(renderer, 0, 0, blueValue, 255);
            SDL_RenderDrawLine(renderer, 0, y, camera.w, y);
        }

        plongeur.render();

        for (const auto& poisson : poissons)
        {
            poisson.draw(renderer);
        }

        afficher();
    }

    return EXIT_SUCCESS;
}