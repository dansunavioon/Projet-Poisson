#ifndef PERSONNAGE_H
#define PERSONNAGE_H

#include <SDL.h>
#include <SDL_image.h>

enum Direction {
    GAUCHE, DROITE, HAUT, BAS
};

class Personnage {
private:
    SDL_Texture* spriteDroite;
    SDL_Texture* spriteGauche;
    SDL_Texture* spriteHaut;
    SDL_Texture* spriteBas;
    SDL_Renderer* renderer;

    int x, y;
    int currentFrame;
    Direction currentDirection;

public:
    Personnage(SDL_Renderer* renderer);
    ~Personnage();
    void handleInput(SDL_Event& e);
    void render(SDL_Renderer* renderer, SDL_Rect camera);
};

#endif