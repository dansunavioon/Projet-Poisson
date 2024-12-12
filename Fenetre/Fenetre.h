#ifndef FENETRE_H
#define FENETRE_H

#include <SDL.h>
#include "../Map/Map.h"

class Fenetre {

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        int height, width;
        // metre camera ici
        Map* map;

    public:
        Fenetre(SDL_Window* window, SDL_Renderer* renderer, int height, int width);
        int display();
        void updateCamera(const SDL_Rect& personnageRect); // Mise à jour de la camera
        void openSettingsWindow();

};



#endif //FENETRE_H
