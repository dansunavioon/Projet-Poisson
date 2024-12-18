#ifndef FENETRE_H
#define FENETRE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "../Map/Map.h"

class Fenetre {

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        int height, width;
        // metre camera ici
        bool isMusicOn; // Indique si la musique est activée
        Map* map;

    public:
        Fenetre(SDL_Window* window, SDL_Renderer* renderer, int height, int width);
        int display();
        void updateCamera(const SDL_Rect& personnageRect); // Mise à jour de la camera
        void openVolumeSettings();
        bool isMouseInsideButton(int x, int y, int width, int height);
        void drawButton(SDL_Renderer* renderer, const char* text, TTF_Font* font, SDL_Color textColor, SDL_Color normalColor, SDL_Color hoverColor, int x, int y, int width, int height);

};



#endif //FENETRE_H
