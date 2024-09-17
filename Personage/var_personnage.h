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



// La structure d'événements
extern SDL_Event event;

// Les différentes parties de la feuille de sprites qui vont être blittés
extern SDL_Rect clipsDroite[ 4 ];
extern SDL_Rect clipsGauche[ 4 ];
extern SDL_Rect clipsHaut[ 4 ];
extern SDL_Rect clipsBas[ 4 ];


#endif //PROJET_POISSON_VAR_PERSONNAGE_H
