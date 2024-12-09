#include "Map.h"

#include "../Camera/Camera.h"


Map::Map(SDL_Window* window, SDL_Renderer* renderer, const int height, const int width)
{
    this->window = window;
    this->renderer = renderer;
    this->height = height;
    this->width = width;
}


void Map::genererRoches() {
    // Dessin des petits rectangles en bas pour représenter des obstacles
    int numRoches = 10; // Nombre d'obstacles
    int minWidth = 50, maxWidth = 150; // Largeur minimale et maximale des obstacles
    int minHeight = 10, maxHeight = 75;

    SDL_SetRenderDrawColor(this->renderer, 139, 69, 19, 255); // Couleur marron pour les obstacles (RVB: brun)

    for (int i = 0; i < numRoches; i++) {
        // Taille aléatoire des obstacles
        int obstacleWidth = minWidth + rand() % (maxWidth - minWidth);
        int obstacleHeight = minHeight + rand() % (maxHeight - minHeight);
        int xPosition = rand() % (this->width - obstacleWidth); // Position horizontale aléatoire

        // Calcul de la position du bas de l'écran
        new Roche(this->renderer, obstacleHeight, obstacleWidth, xPosition, this->height - obstacleHeight);
    }
}

void Map::genererAlgues() {

    int numAlgues = 20;

    for (int i = 0; i < numAlgues; i++) {
        // Taille aléatoire des obstacles
        int obstacleWidth = 50;
        int obstacleHeight = 50;
        int xPosition = rand() % (this->width - obstacleWidth); // Position horizontale aléatoire

        // Calcul de la position du bas de l'écran
        new Algue(this->renderer, obstacleHeight, obstacleWidth, xPosition, this->height - obstacleHeight);
    }
}

// Linear interpolation function for color blending
Uint8 lerp(Uint8 a, Uint8 b, float t) {
    return static_cast<Uint8>(a + t * (b - a));
}

void Map::genererFond() {

    SDL_Color shallowColor = {64, 164, 223}; // Light blue
    SDL_Color deepColor = {0, 25, 51};

    // Dessin du fond bleu avec un dégradé vertical
    for (int y = 0; y < this->height; ++y) {
        float t = static_cast<float>(y) / this->height; // Normalized depth (0.0 at the top, 1.0 at the bottom)

        // Interpolate colors
        Uint8 r = lerp(shallowColor.r, deepColor.r, t);
        Uint8 g = lerp(shallowColor.g, deepColor.g, t);
        Uint8 b = lerp(shallowColor.b, deepColor.b, t);

        // Set the color for the current line
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);

        // Draw the line
        SDL_RenderDrawLine(renderer, 0, y, width, y);
    }
}



int Map::generer(){

    // Initialisation
    SDL_Init(SDL_INIT_EVERYTHING);


    if (!this->window)
        this->window = SDL_CreateWindow("Poisson", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!this->renderer)
        this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Initialisation de la camera
    Camera *camera = new Camera(0,0);


    this->genererFond();
    this->genererRoches();
    this->genererAlgues();

    SDL_RenderPresent(this->renderer);

    // Créer un objet Personnage avec le renderer
    Personnage plongeur(this->renderer, camera);

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
        camera->updateCamera(personnageRect);

        // Rafraîchir l'écran
        SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // Noir par défaut pour effacer
        SDL_RenderClear(this->renderer);

        this->genererFond();
        this->genererRoches();
        this->genererAlgues();

/*
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
*/

        // Rendre le personnage à sa nouvelle position
        plongeur.render();

        std::cout << "Personnage: (" << plongeur.getX() << ", " << plongeur.getY() << ")" << std::endl;
        std::cout << "Camera: (" << camera->getX() << ", " << camera->getY() << ")" << std::endl;


        // Dessine les poissons
        for (const auto& poisson : poissons)
        {
            poisson.draw(this->renderer);
        }

        // Afficher le rendu à l'écran
        SDL_RenderPresent(this->renderer);
    }

    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    return EXIT_SUCCESS;
}