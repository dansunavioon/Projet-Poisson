#include "Personnage.h"

Personnage::Personnage(SDL_Renderer* renderer)
    : renderer(renderer), x(100), y(100), currentFrame(0), currentDirection(DROITE) {

    spriteDroite = IMG_LoadTexture(renderer, "image_perso/Nageur_Droite.png");
    spriteGauche = IMG_LoadTexture(renderer, "image_perso/Nageur_Gauche.png");
    spriteHaut = IMG_LoadTexture(renderer, "image_perso/Nageur_Haut.png");
    spriteBas = IMG_LoadTexture(renderer, "image_perso/Nageur_Bas.png");
}

Personnage::~Personnage() {
    SDL_DestroyTexture(spriteDroite);
    SDL_DestroyTexture(spriteGauche);
    SDL_DestroyTexture(spriteHaut);
    SDL_DestroyTexture(spriteBas);
}

void Personnage::handleInput(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            y -= 5;
            currentDirection = HAUT;
            break;
        case SDLK_DOWN:
            y += 5;
            currentDirection = BAS;
            break;
        case SDLK_LEFT:
            x -= 5;
            currentDirection = GAUCHE;
            break;
        case SDLK_RIGHT:
            x += 5;
            currentDirection = DROITE;
            break;
        }
    }
}

void Personnage::render(SDL_Renderer* renderer, SDL_Rect camera) {
    SDL_Rect destRect = {x - camera.x, y - camera.y, 50, 50};
    SDL_Texture* tex = nullptr;

    switch (currentDirection) {
    case GAUCHE: tex = spriteGauche; break;
    case DROITE: tex = spriteDroite; break;
    case HAUT: tex = spriteHaut; break;
    case BAS: tex = spriteBas; break;
    }

    SDL_RenderCopy(renderer, tex, nullptr, &destRect);
}