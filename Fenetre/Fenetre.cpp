#include "Fenetre.h"
#include "../Personage/Personnage.h"
#include "../Poisson/Poisson.h"
#include "../Personage/var_personnage.h"
#include <SDL_mixer.h>


Mix_Music* backgroundMusic = nullptr;

/**
 * Constructeur
 * @param window
 * @param renderer
 * @param height
 * @param width
 */

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// -------------- Constructeur
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

Fenetre::Fenetre(SDL_Window* window, SDL_Renderer* renderer, int height, int width) : window(window), renderer(renderer), height(height), width(width) {

    // Initialisation de la caméra
    camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    map = new Map(this->renderer, MAP_WIDTH, MAP_HEIGHT);

    // Initialisation du son
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Erreur : Impossible d'initialiser SDL_mixer : %s", Mix_GetError());
    }

    // Chargement de la musique de fond
    Mix_Music* backgroundMusic = Mix_LoadMUS("son_map/fond_poisson.mp3");
    if (!backgroundMusic) {
        SDL_Log("Erreur : Impossible de charger la musique : %s", Mix_GetError());
    }

    // Lecture de la musique en boucle
    Mix_PlayMusic(backgroundMusic, -1);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// -------------- Camera
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// -------------- Bouton Paramètre
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void Fenetre::drawButton(SDL_Renderer* renderer, const char* text, TTF_Font* font, SDL_Color textColor, SDL_Color normalColor, SDL_Color hoverColor) {
    // Vérifie si la souris est sur le bouton
    bool isHovered = isMouseInsideButton(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);

    // Choisir la couleur selon l'état du bouton
    SDL_Color buttonColor = isHovered ? hoverColor : normalColor;

    // Dessiner le bouton
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, 255);
    SDL_Rect buttonRect = { BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_RenderFillRect(renderer, &buttonRect);

    // Dessiner les contours du bouton (facultatif)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &buttonRect);

    // Dessiner le texte du bouton
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Calculer la position pour centrer le texte
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect textRect = {
            BUTTON_X + (BUTTON_WIDTH - textWidth) / 2,
            BUTTON_Y + (BUTTON_HEIGHT - textHeight) / 2,
            textWidth,
            textHeight
    };

    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    // Libérer les ressources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

bool Fenetre::isMouseInsideButton(int x, int y, int width, int height) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);
}

void Fenetre::openVolumeSettings() {
    SDL_Window* volumeWindow = SDL_CreateWindow("Volume", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 200, SDL_WINDOW_SHOWN);
    SDL_Renderer* volumeRenderer = SDL_CreateRenderer(volumeWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16); // Chargez une police (assurez-vous d'installer SDL_ttf)
    if (!font) {
        SDL_Log("Erreur : Impossible de charger la police : %s", TTF_GetError());
    }

    bool isVolumeOpen = true;
    SDL_Event volumeEvent;

    // Définir les dimensions et position du bouton "OK"
    const int okButtonWidth = 80;
    const int okButtonHeight = 40;
    const int okButtonX = (400 - okButtonWidth) / 2; // Centré horizontalement
    const int okButtonY = 150;                       // Position verticale vers le bas

    // Couleurs pour le texte et le bouton
    SDL_Color buttonTextColor = {255, 255, 255}; // Texte blanc
    SDL_Color buttonNormalColor = {100, 100, 100}; // Bouton gris clair
    SDL_Color buttonHoverColor = {150, 150, 150};  // Bouton gris survolé

    while (isVolumeOpen) {
        while (SDL_PollEvent(&volumeEvent)) {
            // printf("%d", volumeEvent);
            switch (volumeEvent.type) {
                case SDL_QUIT:
                    isVolumeOpen = false;  // Fermer la fenêtre
                    // printf("fermer");
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    // Vérifier si le bouton "OK" a été cliqué
                    if (isMouseInsideButton(okButtonX, okButtonY, okButtonWidth, okButtonHeight)) {
                        isVolumeOpen = false; // Fermer la fenêtre
                    }
                    break;

                default:
                    break;
            }
        }

        // Rafraîchir la sous-fenêtre
        SDL_SetRenderDrawColor(volumeRenderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(volumeRenderer);

        // Afficher le bouton "OK"
        drawButton(volumeRenderer, "SDL2 n'est pas assez performant pour gérer deux fenêtre xp", font, buttonTextColor, buttonNormalColor, buttonHoverColor);

        SDL_RenderPresent(volumeRenderer);
    }
    printf("net");
    // Nettoyer
    SDL_DestroyRenderer(volumeRenderer);
    SDL_DestroyWindow(volumeWindow);
}


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// -------------- Affichage
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

int Fenetre::display(){

    // Initialisation
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    this->window = SDL_CreateWindow("Poisson", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN);
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Création d'un bouton
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16); // Chargez une police (assurez-vous d'installer SDL_ttf)
    if (!font) {
        SDL_Log("Erreur : Impossible de charger la police : %s", TTF_GetError());
    }


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
                case SDL_MOUSEBUTTONDOWN:
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (isMouseInsideButton(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        // Ouvrir la sous-fenêtre de gestion du volume
                        openVolumeSettings();
                    }
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

        // Mettre à jour la caméra en fonction de la position du personnage
        SDL_Rect personnageRect = {plongeur.getX(), plongeur.getY(), TAILLE_PLONGEUR, TAILLE_PLONGEUR};
        updateCamera(personnageRect);

        // Rafraîchir l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir par défaut pour effacer
        SDL_RenderClear(renderer);

        // Dessiner la carte
        map->render(camera);

        // Dessin du fond bleu avec un dégradé vertical en fonction de la caméra
        for (int y = 0; y < camera.h; y++) {
            int blueValue = 255 - ((camera.y + y) * 255 / MAP_HEIGHT); // Dégradé en fonction de la position sur la map
            SDL_SetRenderDrawColor(renderer, 0, 0, blueValue, 255);
            SDL_RenderDrawLine(renderer, 0, y, camera.w, y);
        }

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

        SDL_Color normalColor = {128, 128, 128, 255}; // Gris
        SDL_Color hoverColor = {160, 160, 160, 255};  // Gris clair
        SDL_Color textColor = {255, 255, 255, 255};   // Blanc

        // Pendant le rendu
        drawButton(renderer, "Parametre", font, textColor, normalColor, hoverColor);


        // Afficher le rendu à l'écran
        SDL_RenderPresent(renderer);
    }

    // Quitter proprement
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
    TTF_Quit();
    delete map;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    return EXIT_SUCCESS;
}