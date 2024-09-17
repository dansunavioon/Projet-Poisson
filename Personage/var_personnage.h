#ifndef PROJET_POISSON_VAR_PERSONNAGE_H
#define PROJET_POISSON_VAR_PERSONNAGE_H

#include <SDL_surface.h>
#include <SDL.h>

// Les attributs de l'écran (800 * 600)
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Les attributs de la feuille de sprites
const int SHEET_WIDTH_BANDE = 794;
const int SHEET_HEIGHT_BANDE = 69;

const int SHEET_WIDTH_BATON = 69;
const int SHEET_HEIGHT_BATON = 794;

const int TAILLE_PLONGEUR = 198;

// Les surfaces
SDL_Surface *faces = NULL;
SDL_Surface *screen = NULL;

// La structure d'événements
SDL_Event event;

// Les différentes parties de la feuille de sprites qui vont être blittés
SDL_Rect clipsDroite[ 4 ];
SDL_Rect clipsGauche[ 4 ];
SDL_Rect clipsHaut[ 4 ];
SDL_Rect clipsBas[ 4 ];


#endif //PROJET_POISSON_VAR_PERSONNAGE_H
