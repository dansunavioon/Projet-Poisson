#include "Fenetre.h"
#include <iostream>

Fenetre::Fenetre(const std::string& titre, int largeur, int hauteur)
    : window(nullptr), renderer(nullptr), valide(false)
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
