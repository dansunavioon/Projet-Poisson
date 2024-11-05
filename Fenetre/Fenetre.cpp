#include "Fenetre.h"
#include "../Personage/Personnage.h"
#include "../Personage/var_personnage.h"


/**
 * Constructeur
 * @param window
 * @param renderer
 * @param height
 * @param width
 */
Fenetre::Fenetre(SDL_Window* window, SDL_Renderer* renderer, const int height, const int width){
    this->window = window;
    this->renderer = renderer;
    this->height = height;
    this->width = width;
    // Initialisation de la caméra
    camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

void Fenetre::updateCamera(const SDL_Rect& personnageRect) {
    // Centrer la caméra sur le personnage
    camera.x = personnageRect.x + (TAILLE_PLONGEUR / 2) - (SCREEN_WIDTH / 2);
    camera.y = personnageRect.y + (TAILLE_PLONGEUR / 2) - (SCREEN_HEIGHT / 2);

    // Empêcher la caméra de sortir des limites de la carte
    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x > MAP_WIDTH - camera.w) camera.x = MAP_WIDTH - camera.w;
    if (camera.y > MAP_HEIGHT - camera.h) camera.y = MAP_HEIGHT - camera.h;
}

int Fenetre::display(){

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

    // Timer pour l'animation
    Uint32 animationTimer = 0; // Temps d'animation
    const Uint32 animationDelay = 100; // Délai en millisecondes pour la mise à jour de l'animation

    while (isOpen) {
        // Gérer les événements utilisateur
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

        // Temps actuel
        Uint32 currentTime = SDL_GetTicks();

        // Vérifie si le temps écoulé depuis la dernière mise à jour de l'animation est supérieur au délai
        if (currentTime - animationTimer >= animationDelay) {
            plongeur.update(); // Mettre à jour l'animation
            animationTimer = currentTime; // Réinitialiser le timer
        }

        // Mettre à jour l'état du personnage (animation, position, etc.)
        // NB : Penser à faire en sorte que la boucle de calcul ait un petit wait (à limiter, essayer de le faire pour que chaque frame de calcul dure le même temps)
        // NB : pour les animations : là ça doit être obligatoire de limiter les updates mais bien distinguer du calcul (vitesse des animations potentiellement très différentes des calculs)

        // Mettre à jour la caméra en fonction de la position du personnage
        SDL_Rect personnageRect = {plongeur.getX(), plongeur.getY(), TAILLE_PLONGEUR, TAILLE_PLONGEUR};
        updateCamera(personnageRect);

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