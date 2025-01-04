#ifndef FENETRE_H
#define FENETRE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "../Personage/Personnage.h"
#include "../Poisson/Poisson.h"
#include "../Personage/var_personnage.h"
#include <SDL_mixer.h>

#include "../Map/Map.h"

class Fenetre {

    SDL_Window* window;
    SDL_Renderer* renderer;
    int height, width;

    bool isMusicOn;

    int cameraX;
    int cameraY;

    Map* map;

    public:
        Fenetre(SDL_Window* window, SDL_Renderer* renderer, Map* map, int height, int width);
        int display();
        void updateCamera(int personnageX, int personnageY);
        void openVolumeSettings();
        bool isMouseInsideButton(int x, int y, int width, int height);
        void drawButton(SDL_Renderer* renderer, const char* text, TTF_Font* font, SDL_Color textColor, SDL_Color normalColor, SDL_Color hoverColor, int x, int y, int width, int height);
};



#endif //FENETRE_H
