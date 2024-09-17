#include "Fenetre.h"
#include "../Personage/Personnage.h"
#include "../Personage//var_personnage.h"


/**
 * Constructeur
 * @param window
 * @param renderer
 * @param height
 * @param width
 */
Fenetre::Fenetre(SDL_Window* window, SDL_Renderer* renderer, const int height, const int width)
{
 this->window = window;
 this->renderer = renderer;
 this->height = height;
 this->width = width;
}


int Fenetre::display()
{
 // Initialisation
 SDL_Init(SDL_INIT_EVERYTHING);


 this->window = SDL_CreateWindow("Poisson", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN);
 this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


 SDL_SetRenderDrawColor(this->renderer, 0, 0, 150, 100);
 SDL_RenderClear(this->renderer);


 SDL_RenderPresent(this->renderer);

// Créer un objet Personnage avec le renderer
Personnage plongeur(renderer);

 SDL_Event events;

// Boucle infinie qui empeche la destruction de la fenetre tant que la fenetre n'est pas fermée
bool isOpen = true;

while (isOpen) {
    // Gérer les événements utilisateur (déplacements du personnage et fermeture de la fenêtre)
    while (SDL_PollEvent(&events)) {
        switch (events.type) {
            case SDL_QUIT:
                isOpen = false;  // Quitter l'application
                break;
            default:
                plongeur.handleInput(events);  // Gérer les entrées pour déplacer le personnage
                break;
        }
    }

    // Mettre à jour l'état du personnage (animation, position, etc.)
    // NB :  Penser à faire en sorte que la boucle de calcul ait un petit wait (à limiter, essayer de le faire pour que chaque frame de calcul dure le même temps)
    // NB : pour les animations : là ça doit être obligatoire de limiter les updates mais bien distinguer du calcul (vitesse des animations potentiellement très différentes des calculs)
    plongeur.update();

    // Rafraîchir l'écran
    SDL_RenderClear(renderer);

    // Rendre le personnage à sa nouvelle position
    plongeur.render();

    // Afficher le rendu à l'écran
    SDL_RenderPresent(renderer);
}

// Quitter proprement
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();


SDL_DestroyRenderer(this->renderer);
SDL_DestroyWindow(this->window);
SDL_Quit();
return EXIT_SUCCESS;

}
