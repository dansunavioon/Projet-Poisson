#include "Personnage.h"

Personnage::Personnage(SDL_Renderer* renderer, Camera* camera) : renderer(renderer), camera(camera), x(100), y(100), currentFrame(0), currentDirection(DROITE) {
    // Calculer la position initiale du plongeur au centre de la carte
    x = (MAP_WIDTH / 2) - (TAILLE_PLONGEUR / 2); // Centrer horizontalement
    y = (MAP_HEIGHT / 2) - (TAILLE_PLONGEUR / 2); // Centrer verticalement

    // Charger les 4 images de sprites
    SDL_Surface* tempSurface = SDL_LoadBMP("image_perso/Nageur_Droite.bmp");
    if (!tempSurface) {
        std::cerr << "Erreur lors du chargement du sprite : " << SDL_GetError() << std::endl;
        return;
    }
    spriteDroite = SDL_CreateTextureFromSurface(renderer, tempSurface);
    if (spriteDroite == nullptr) {
        std::cerr << "Erreur lors de la création de la texture : " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(tempSurface);

    tempSurface = SDL_LoadBMP("image_perso/Nageur_Gauche.bmp");
    spriteGauche = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    tempSurface = SDL_LoadBMP("image_perso/Nageur_Haut.bmp");
    spriteHaut = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    tempSurface = SDL_LoadBMP("image_perso/Nageur_Bas.bmp");
    spriteBas = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Initialiser les clips
    setClips();
}

// Destructeur
Personnage::~Personnage() {
    SDL_DestroyTexture(spriteDroite);
    SDL_DestroyTexture(spriteGauche);
    SDL_DestroyTexture(spriteHaut);
    SDL_DestroyTexture(spriteBas);
}

// Initialiser les clips pour chaque direction
void Personnage::setClips() {
    // Clips pour aller à droite (bande horizontale)
    for (int i = 0; i < 4; i++) {
        clipsDroite[i].x = i * TAILLE_PLONGEUR;
        clipsDroite[i].y = 0;
        clipsDroite[i].w = TAILLE_PLONGEUR;
        clipsDroite[i].h = SHEET_HEIGHT_BANDE;
    }

    // Clips pour aller à gauche (bande horizontale)
    for (int i = 0; i < 4; i++) {
        clipsGauche[i].x = i * TAILLE_PLONGEUR;
        clipsGauche[i].y = 0;
        clipsGauche[i].w = TAILLE_PLONGEUR;
        clipsGauche[i].h = SHEET_HEIGHT_BANDE;
    }

    // Clips pour aller en haut (bande verticale)
    for (int i = 0; i < 4; i++) {
        clipsHaut[i].x = 0;
        clipsHaut[i].y = i * TAILLE_PLONGEUR;
        clipsHaut[i].w = SHEET_WIDTH_BATON;
        clipsHaut[i].h = TAILLE_PLONGEUR;
    }

    // Clips pour aller en bas (bande verticale)
    for (int i = 0; i < 4; i++) {
        clipsBas[i].x = 0;
        clipsBas[i].y = i * TAILLE_PLONGEUR;
        clipsBas[i].w = SHEET_WIDTH_BATON;
        clipsBas[i].h = TAILLE_PLONGEUR;
    }
}

// Gérer les entrées utilisateur
void Personnage::handleInput(SDL_Event& monevent) {
        if (monevent.type == SDL_KEYDOWN) {
            switch (monevent.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_z:
                    if (y > 0) y -= TAILLE_PLONGEUR / 2;
                    currentDirection = HAUT;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    if (y < MAP_HEIGHT - TAILLE_PLONGEUR) y += TAILLE_PLONGEUR / 2;
                    currentDirection = BAS;
                    break;
                case SDLK_LEFT:
                case SDLK_q:
                    if (x > 0) x -= TAILLE_PLONGEUR / 2;
                    currentDirection = GAUCHE;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    if (x < MAP_WIDTH - TAILLE_PLONGEUR) x += TAILLE_PLONGEUR / 2;
                    currentDirection = DROITE;
                    break;
            }
        }
}

// Mettre à jour l'animation
void Personnage::update() {
    currentFrame = (currentFrame + 1) % 4;  // 4 frames d'animation par direction
}

// Afficher le personnage
void Personnage::render() {
    SDL_Rect* currentClip = nullptr;
    SDL_Texture* currentSprite = nullptr;
    int WIDTH_QUAD = 198;
    int HEIGHT_QUAD = 69;

    switch (currentDirection) {
        case HAUT:
            currentClip = &clipsHaut[currentFrame];
            currentSprite = spriteHaut;
            WIDTH_QUAD = 69;
            HEIGHT_QUAD = 198;
            break;
        case BAS:
            currentClip = &clipsBas[currentFrame];
            currentSprite = spriteBas;
            WIDTH_QUAD = 69;
            HEIGHT_QUAD = 198;
            break;
        case GAUCHE:
            currentClip = &clipsGauche[currentFrame];
            currentSprite = spriteGauche;
            WIDTH_QUAD = 198;
            HEIGHT_QUAD = 69;
            break;
        case DROITE:
            currentClip = &clipsDroite[currentFrame];
            currentSprite = spriteDroite;
            WIDTH_QUAD = 198;
            HEIGHT_QUAD = 69;
            break;
    }

    SDL_Rect renderQuad = {x - this->camera->getX(), y - this->camera->getY(), WIDTH_QUAD, HEIGHT_QUAD};
    SDL_RenderCopy(renderer, currentSprite, currentClip, &renderQuad);
    /*
    std::cout << "renderQuad: x=" << renderQuad.x << " y=" << renderQuad.y
              << " w=" << renderQuad.w << " h=" << renderQuad.h << std::endl;
    std::cout << "currentClip: x=" << currentClip->x << " y=" << currentClip->y
              << " w=" << currentClip->w << " h=" << currentClip->h << std::endl;*/

}
