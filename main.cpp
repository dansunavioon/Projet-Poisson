#include "Fenetre/Fenetre.h"

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Créer et afficher une fenêtre

    Fenetre* fenetre = new Fenetre(window, renderer, 600, 800);

    fenetre->display();

    return 0;
}

