#include "Fenetre.h"

#include "../Algue/Algue.h"
#include "../Personage/Personnage.h"
#include "../Poisson/Poisson.h"
#include "../Roche/Roche.h"
//#include <SDL_mixer.h>


//Mix_Music* backgroundMusic = nullptr;

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

    // Initialisation du son
    /*if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Erreur : Impossible d'initialiser SDL_mixer : %s", Mix_GetError());
    }

    // Chargement de la musique de fond
    Mix_Music* backgroundMusic = Mix_LoadMUS("son_map/fond_poisson.mp3");
    if (!backgroundMusic) {
        SDL_Log("Erreur : Impossible de charger la musique : %s", Mix_GetError());
    }

    // Lecture de la musique en boucle
    Mix_PlayMusic(backgroundMusic, -1);*/
}
/*
void Fenetre::openSettingsWindow() {
    SDL_Window* settingsWindow = SDL_CreateWindow("Paramètres", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300, SDL_WINDOW_SHOWN);
    SDL_Renderer* settingsRenderer = SDL_CreateRenderer(settingsWindow, -1, SDL_RENDERER_ACCELERATED);

    bool isSettingsOpen = true;
    SDL_Event settingsEvent;

    while (isSettingsOpen) {
        while (SDL_PollEvent(&settingsEvent)) {
            if (settingsEvent.type == SDL_QUIT || (settingsEvent.type == SDL_KEYDOWN && settingsEvent.key.keysym.sym == SDLK_ESCAPE)) {
                isSettingsOpen = false;
            }

            if (settingsEvent.type == SDL_KEYDOWN && settingsEvent.key.keysym.sym == SDLK_UP) {
                Mix_VolumeMusic(MIX_MAX_VOLUME); // Volume maximum
            }
            if (settingsEvent.type == SDL_KEYDOWN && settingsEvent.key.keysym.sym == SDLK_DOWN) {
                Mix_VolumeMusic(0); // Couper le son
            }
        }

        SDL_SetRenderDrawColor(settingsRenderer, 200, 200, 200, 255);
        SDL_RenderClear(settingsRenderer);
        SDL_RenderPresent(settingsRenderer);
    }

    SDL_DestroyRenderer(settingsRenderer);
    SDL_DestroyWindow(settingsWindow);
}
*/

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

int displayRocheAlgue(SDL_Renderer * renderer, int width, int height) {
    // Dessin des petits rectangles en bas pour représenter des obstacles
    int numObstacles = 10; // Nombre d'obstacles
    int numAlgues = 20;
    int minWidth = 50, maxWidth = 150; // Largeur minimale et maximale des obstacles
    int minHeight = 10, maxHeight = 75;

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Couleur marron pour les obstacles (RVB: brun)

    for (int i = 0; i < numObstacles; i++) {
        // Taille aléatoire des obstacles
        int obstacleWidth = minWidth + rand() % (maxWidth - minWidth);
        int obstacleHeight = minHeight + rand() % (maxHeight - minHeight);
        int xPosition = rand() % (width - obstacleWidth); // Position horizontale aléatoire

        // Calcul de la position du bas de l'écran
        new Roche(renderer, obstacleHeight, obstacleWidth, xPosition, height - obstacleHeight);
    }

    for (int i = 0; i < numAlgues; i++) {
        // Taille aléatoire des obstacles
        int obstacleWidth = 50;
        int obstacleHeight = 50;
        int xPosition = rand() % (width - obstacleWidth); // Position horizontale aléatoire

        // Calcul de la position du bas de l'écran
        new Algue(renderer, obstacleHeight, obstacleWidth, xPosition, height - obstacleHeight);
    }
}

int Fenetre::display(){

    // Initialisation
    SDL_Init(SDL_INIT_EVERYTHING);


    if (!this->window)
        this->window = SDL_CreateWindow("Poisson", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN);

    if (!this->renderer)
        this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    // Dessin du fond bleu avec un dégradé vertical
    for (int y = 0; y < this->height; y++) {
        int blueValue = 255 - (y * 255 / this->height); // Calcul d'un dégradé du bleu (du plus clair au plus foncé)
        SDL_SetRenderDrawColor(this->renderer, 0, 0, blueValue, 255);
        SDL_RenderDrawLine(this->renderer, 0, y, this->width, y);
    }

    displayRocheAlgue(this->renderer, this->width, this->height);

    SDL_RenderClear(this->renderer);
    SDL_RenderPresent(this->renderer);

    // Créer un objet Personnage avec le renderer
    Personnage plongeur(renderer);

    // Création des poissons avec des positions de départ aléatoires
    std::vector<Poisson> poissons;
    int nombrePoissons = 600; // Nombre de poissons
    for (int i = 0; i < nombrePoissons; ++i)
    {
        float randomX = static_cast<float>(rand() % 800); // Position X aléatoire
        float randomY = static_cast<float>(rand() % 600); // Position Y aléatoire
        poissons.emplace_back(randomX, randomY);
    }

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

        // Mise à jour des poissons
        for (auto& poisson : poissons) {
            poisson.update(poissons);
        }

        // Mettre à jour la caméra en fonction de la position du personnage
        SDL_Rect personnageRect = {plongeur.getX(), plongeur.getY(), TAILLE_PLONGEUR, TAILLE_PLONGEUR};
        updateCamera(personnageRect);

        // Rafraîchir l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir par défaut pour effacer
        SDL_RenderClear(renderer);

        // Dessin du fond bleu avec un dégradé vertical en fonction de la caméra
        for (int y = 0; y < camera.h; y++) {
            int blueValue = 255 - ((camera.y + y) * 255 / MAP_HEIGHT); // Dégradé en fonction de la position sur la map
            SDL_SetRenderDrawColor(renderer, 0, 0, blueValue, 255);
            SDL_RenderDrawLine(renderer, 0, y, camera.w, y);
        }

        displayRocheAlgue(this->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Dessiner la jauge de profondeur sur le côté droit
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Couleur de fond de la jauge (gris clair)
        SDL_Rect gaugeBackground = { JAUGE_X, MARGE_Y, JAUGE_WIDTH, JAUGE_HEIGHT };
        SDL_RenderFillRect(renderer, &gaugeBackground);

        // Calcul de la position de l'indicateur en fonction de la profondeur (position Y du plongeur)
        int indicatorY = MARGE_Y + (plongeur.getY() * JAUGE_HEIGHT / MAP_HEIGHT);
        indicatorY = std::min(std::max(indicatorY, MARGE_Y), MARGE_Y + JAUGE_HEIGHT); // Clamp pour éviter les débordements

        // Dessiner l'indicateur de profondeur
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Couleur de l'indicateur (rouge)
        SDL_Rect indicator = { JAUGE_X - 5, indicatorY - 5, JAUGE_WIDTH + 10, 10 }; // Indicateur élargi
        SDL_RenderFillRect(renderer, &indicator);


        // Rendre le personnage à sa nouvelle position
        plongeur.render();

        // Dessine les poissons
        for (const auto& poisson : poissons)
        {
            poisson.draw(renderer);
        }
/*
        // Dessiner le bouton des paramètres
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Couleur du bouton (bleu)
        SDL_Rect settingsButton = { 10, 10, 100, 40 }; // Position et taille du bouton
        SDL_RenderFillRect(renderer, &settingsButton);

        // Gestion des événements pour le clic sur le bouton
        if (events.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = events.button.x;
            int mouseY = events.button.y;

            if (mouseX >= settingsButton.x && mouseX <= (settingsButton.x + settingsButton.w) &&
                mouseY >= settingsButton.y && mouseY <= (settingsButton.y + settingsButton.h)) {

                openSettingsWindow(); // Fonction pour gérer les paramètres
            }
        }
        */

        // Afficher le rendu à l'écran
        SDL_RenderPresent(renderer);
    }

    // Quitter proprement
    /*Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();*/
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    return EXIT_SUCCESS;
}