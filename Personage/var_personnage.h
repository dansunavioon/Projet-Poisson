#ifndef PROJET_POISSON_VAR_PERSONNAGE_H
#define PROJET_POISSON_VAR_PERSONNAGE_H

#include <SDL_surface.h>
#include <SDL.h>

// Camera
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Les attributs de la feuille de sprites (794 x 69)
const int SHEET_HEIGHT_BANDE = 69;
const int SHEET_WIDTH_BATON = 69;

const int TAILLE_PLONGEUR = 198;

extern SDL_Rect camera; // Map
const int MAP_WIDTH = 1742;  // Largeur de la carte
const int MAP_HEIGHT = 980; // Hauteur de la carte

const int JAUGE_WIDTH = 5;  // Largeur réduite de la jauge
const int JAUGE_X = SCREEN_WIDTH - 20 - JAUGE_WIDTH;  // Position X de la jauge (à 20 pixels du bord droit)
const int JAUGE_HEIGHT = SCREEN_HEIGHT - 40; // Hauteur de la jauge (adaptée à l'écran)
const int MARGE_Y = 20;       // Marge supérieure et inférieure

// Position et dimensions du bouton Paramètres
const int BUTTON_X = 10;
const int BUTTON_Y = 10;
const int BUTTON_WIDTH = 100;
const int BUTTON_HEIGHT = 40;

// Position et dimensions du bouton Musique
const int MUSIC_BUTTON_X = BUTTON_X + BUTTON_WIDTH + 10; // À côté du bouton Paramètre
const int MUSIC_BUTTON_Y = BUTTON_Y;
const int MUSIC_BUTTON_SIZE = 40; // Carré de 40x40


// La structure d'événements
extern SDL_Event event;

// Les différentes parties de la feuille de sprites qui vont être blittés
extern SDL_Rect clipsDroite[ 4 ];
extern SDL_Rect clipsGauche[ 4 ];
extern SDL_Rect clipsHaut[ 4 ];
extern SDL_Rect clipsBas[ 4 ];


#endif //PROJET_POISSON_VAR_PERSONNAGE_H
