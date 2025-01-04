#include "Fenetre.h"


Mix_Music* backgroundMusic = nullptr;


Fenetre::Fenetre(SDL_Window* window, SDL_Renderer* renderer, Map* map, int height, int width)
: window(window), renderer(renderer), map(map), height(height), width(width), cameraX(0), cameraY(0) {

    // Initialisation du son
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Erreur : Impossible d'initialiser SDL_mixer : %s", Mix_GetError());
    }

    // Chargement de la musique de fond
    isMusicOn = true; // La musique est activée par défaut
    Mix_Music* backgroundMusic = Mix_LoadMUS("son_map/fond_poisson.mp3");
    if (!backgroundMusic) {
        SDL_Log("Erreur : Impossible de charger la musique : %s", Mix_GetError());
    }

    // Lecture de la musique en boucle
    Mix_PlayMusic(backgroundMusic, -1);
}


int Fenetre::display() {

    // Initialisation
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // Création d'un bouton
    TTF_Font* font = TTF_OpenFont("DejaVuSans.ttf", 16); // Chargez une police
    if (!font) {
        SDL_Log("Erreur : Impossible de charger la police : %s", TTF_GetError());
    }

    Personnage plongeur(renderer);

    SDL_Event events;
    bool isOpen = true;

    // Timer pour l'animation
    Uint32 animationTimer = 0; // Temps d'animation
    const Uint32 animationDelay = 100; // Délai en millisecondes pour la mise à jour de l'animation

    while (isOpen) {
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
                if (isMouseInsideButton(MUSIC_BUTTON_X, MUSIC_BUTTON_Y, MUSIC_BUTTON_SIZE, MUSIC_BUTTON_SIZE)) {
                    isMusicOn = !isMusicOn;
                    if (isMusicOn) {
                        Mix_ResumeMusic();
                    } else {
                        Mix_PauseMusic();
                    }
                }
                break;
                default:
                    plongeur.handleInput(events);  // Gérer les entrées pour déplacer le personnage
                break;
            }
        }

        // Mettre à jour la caméra en fonction de la position du personnage
        updateCamera(plongeur.getX(), plongeur.getY());

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Rendre la carte avec les coordonnées de la caméra
        map->render(cameraX, cameraY);

        // Rendre le personnage (avec position relative à la caméra)
        plongeur.render(cameraX, cameraY);

        // Rendre la jauge de profondeur
        // Dessiner la jauge de profondeur sur le côté droit
        SDL_SetRenderDrawColor(this->renderer, 200, 200, 200, 255); // Couleur de fond de la jauge (gris clair)
        SDL_Rect gaugeBackground = { JAUGE_X, MARGE_Y, JAUGE_WIDTH, JAUGE_HEIGHT };
        SDL_RenderFillRect(this->renderer, &gaugeBackground);

        // Calcul de la position de l'indicateur en fonction de la profondeur (position Y du plongeur)
        int indicatorY = MARGE_Y + (plongeur.getY() * JAUGE_HEIGHT / MAP_HEIGHT);
        indicatorY = std::min(std::max(indicatorY, MARGE_Y), MARGE_Y + JAUGE_HEIGHT); // Clamp pour éviter les débordements

        // Dessiner l'indicateur de profondeur
        SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255); // Couleur de l'indicateur (rouge)
        SDL_Rect indicator = { JAUGE_X - 5, indicatorY - 5, JAUGE_WIDTH + 10, 10 }; // Indicateur élargi
        SDL_RenderFillRect(this->renderer, &indicator);

        // Temps actuel
        Uint32 currentTime = SDL_GetTicks();

        // Vérifie si le temps écoulé depuis la dernière mise à jour de l'animation est supérieur au délai
        if (currentTime - animationTimer >= animationDelay) {
            plongeur.update(); // Mettre à jour l'animation
            animationTimer = currentTime; // Réinitialiser le timer
        }

        SDL_Color normalColor = {128, 128, 128, 255}; // Gris
        SDL_Color hoverColor = {160, 160, 160, 255};  // Gris clair
        SDL_Color textColor = {255, 255, 255, 255};   // Blanc

        // Pendant le rendu
        drawButton(renderer, "Parametre", font, textColor, normalColor, hoverColor, BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);

        // Texte du bouton
        const char* musicText = isMusicOn ? "ON" : "OFF";

        // Dessiner le bouton carré
        drawButton(renderer, musicText, font, textColor, normalColor, hoverColor, MUSIC_BUTTON_X, MUSIC_BUTTON_Y, MUSIC_BUTTON_SIZE, MUSIC_BUTTON_SIZE);

        // Afficher à l'écran
        SDL_RenderPresent(renderer);
    }

    // Quitter proprement
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void Fenetre::drawButton(SDL_Renderer* renderer, const char* text, TTF_Font* font, SDL_Color textColor, SDL_Color normalColor, SDL_Color hoverColor, int x, int y, int width, int height) {
    // Vérifie si la souris est sur le bouton
    bool isHovered = isMouseInsideButton(x, y, width, height);

    // Choisir la couleur selon l'état du bouton
    SDL_Color buttonColor = isHovered ? hoverColor : normalColor;

    // Dessiner le bouton
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, 255);
    SDL_Rect buttonRect = { x, y, width, height };
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
            x + (width - textWidth) / 2,
            y + (height - textHeight) / 2,
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

    TTF_Font* font = TTF_OpenFont("DejaVuSans.ttf", 16); // Chargez une police
    if (!font) {
        SDL_Log("Erreur : Impossible de charger la police : %s", TTF_GetError());
    }

    bool isVolumeOpen = true;
    SDL_Event volumeEvent;

    SDL_Color textColor = {255, 255, 255, 255}; // Blanc
    const char* message = "SDL2 n'est pas assez performant\npour gerer deux fenetres apparemment.";

    while (isVolumeOpen) {
        while (SDL_PollEvent(&volumeEvent)) {
            // printf("%d", volumeEvent);
            switch (volumeEvent.type) {
                case SDL_QUIT:
                    isVolumeOpen = false;  // Fermer la fenêtre
                    // printf("fermer");
                    break;

                default:
                    break;
            }
        }

        // Rafraîchir la sous-fenêtre
        SDL_SetRenderDrawColor(volumeRenderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(volumeRenderer);

        // Dessiner le texte centré
        SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, message, textColor, 380); // Largeur max 380px
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(volumeRenderer, textSurface);

        int textWidth = textSurface->w;
        int textHeight = textSurface->h;

        SDL_Rect textRect = {
                (400 - textWidth) / 2, // Centré horizontalement
                (200 - textHeight) / 2, // Centré verticalement
                textWidth,
                textHeight
        };

        SDL_RenderCopy(volumeRenderer, textTexture, nullptr, &textRect);
        // Libérer les ressources temporaires
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        SDL_RenderPresent(volumeRenderer);
    }
    printf("net");
    // Nettoyer
    SDL_DestroyRenderer(volumeRenderer);
    SDL_DestroyWindow(volumeWindow);
}

void Fenetre::updateCamera(int personnageX, int personnageY) {
    // Centrer la caméra sur le personnage
    cameraX = personnageX - width / 2;
    cameraY = personnageY - height / 2;

    // Empêcher la caméra de dépasser les bords de la carte
    cameraX = std::max(0, std::min(cameraX, map->getWidth() - width));
    cameraY = std::max(0, std::min(cameraY, map->getHeight() - height));
}