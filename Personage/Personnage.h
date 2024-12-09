#ifndef PROJET_POISSON_PERSONNAGE_H
#define PROJET_POISSON_PERSONNAGE_H

#include <SDL.h>
#include <iostream>
#include "var_personnage.h"
#include "../TypeDef.h"
#include "../Camera/Camera.h"

enum Direction {
    DROITE,
    GAUCHE,
    HAUT,
    BAS
};

class Personnage {

    SDL_Texture* spriteDroite;  // Feuille de sprites pour aller à droite
    SDL_Texture* spriteGauche;  // Feuille de sprites pour aller à gauche
    SDL_Texture* spriteHaut;    // Feuille de sprites pour aller en haut
    SDL_Texture* spriteBas;     // Feuille de sprites pour aller en bas
    SDL_Renderer* renderer;

    int x, y;                 // Position du personnage
    int currentFrame;         // Frame actuelle de l'animation
    Direction currentDirection;  // Direction actuelle

    Camera* camera;            // Caméra pour suivre le personnage

    void setClips();          // Initialiser les clips de sprites

    public:
        Personnage(SDL_Renderer* renderer, Camera* camera);
        ~Personnage();

        // Gérer les mouvements
        void handleInput(SDL_Event& monevent);

        // Mettre à jour le personnage
        void update();

        // Afficher le personnage
        void render();

        // Accéder à la position pour la caméra
        int getX() const { return x; }
        int getY() const { return y; }
};


#endif //PROJET_POISSON_PERSONNAGE_H
