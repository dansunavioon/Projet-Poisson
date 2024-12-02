#include "Map/Map.h"

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Créer et afficher une fenêtre

    Map* map = new Map(window, renderer, MAP_HEIGHT, MAP_WIDTH);

    map->display();

    return 0;
}

