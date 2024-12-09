#include "Map.h"
#include <cstdlib>

#include "../Algue/Algue.h"
#include "../Roche/Roche.h"

Map::Map(SDL_Window* window, SDL_Renderer* renderer, const int height, const int width)
{
    this->window = window;
    this->renderer = renderer;
    this->height = height;
    this->width = width;
}


int Map::display()
{
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Erreur d'initialisation de SDL: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Création de la fenêtre et du renderer
    this->window = SDL_CreateWindow("Poisson", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN);
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

    if (!this->window || !this->renderer) {
        SDL_Log("Erreur lors de la création de la fenêtre ou du renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Dessin du fond bleu avec un dégradé vertical
    for (int y = 0; y < this->height; y++) {
        int blueValue = 255 - (y * 255 / this->height); // Calcul d'un dégradé du bleu (du plus clair au plus foncé)
        SDL_SetRenderDrawColor(this->renderer, 0, 0, blueValue, 255);
        SDL_RenderDrawLine(this->renderer, 0, y, this->width, y);
    }

    // Dessin des petits rectangles en bas pour représenter des obstacles
    int numObstacles = 10; // Nombre d'obstacles
    int numAlgues = 20;
    int minWidth = 50, maxWidth = 150; // Largeur minimale et maximale des obstacles
    int minHeight = 10, maxHeight = 75;

    SDL_SetRenderDrawColor(this->renderer, 139, 69, 19, 255); // Couleur marron pour les obstacles (RVB: brun)

    for (int i = 0; i < numObstacles; i++) {
        // Taille aléatoire des obstacles
        int obstacleWidth = minWidth + rand() % (maxWidth - minWidth);
        int obstacleHeight = minHeight + rand() % (maxHeight - minHeight);
        int xPosition = rand() % (this->width - obstacleWidth); // Position horizontale aléatoire

        // Calcul de la position du bas de l'écran
        new Roche(this->renderer, obstacleHeight, obstacleWidth, xPosition, this->height - obstacleHeight);
    }

    for (int i = 0; i < numAlgues; i++) {
        // Taille aléatoire des obstacles
        int obstacleWidth = 50;
        int obstacleHeight = 50;
        int xPosition = rand() % (this->width - obstacleWidth); // Position horizontale aléatoire

        // Calcul de la position du bas de l'écran
        new Algue(this->renderer, obstacleHeight, obstacleWidth, xPosition, this->height - obstacleHeight);
    }



    // Mise à jour de l'affichage
    SDL_RenderPresent(this->renderer);

    SDL_Event events;
    bool isOpen = true;

    while (isOpen)
    {
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
                case SDL_QUIT:
                    isOpen = false;
                break;
            }
        }
    }

    // Nettoyage des ressources
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
