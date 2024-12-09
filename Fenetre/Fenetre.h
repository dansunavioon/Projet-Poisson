#ifndef FENETRE_H
#define FENETRE_H

#include <SDL.h>
#include <string>

class Fenetre {
public:
    Fenetre(const std::string& titre, int largeur, int hauteur);
    ~Fenetre();

    bool estValide() const;
    SDL_Renderer* obtenirRenderer() const;

    void effacer();
    void afficher();
    void updateCamera(const SDL_Rect& personnageRect);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect camera;
    bool valide;

    int height;
    int width;
};

#endif