#include "Map.h"
#include "../Personage/var_personnage.h"

Map::Map(SDL_Renderer* renderer, int width, int height)
        : renderer(renderer), width(width), height(height) {}

void Map::render(const SDL_Rect& camera) {
    // Dessiner le fond avec un dégradé basé sur la position de la caméra
    for (int y = camera.y; y < camera.y + camera.h; y++) {
        int blueValue = 255 - (y * 255 / height); // Calcul du dégradé
        SDL_SetRenderDrawColor(renderer, 0, 0, blueValue, 255);
        SDL_RenderDrawLine(renderer, camera.x, y - camera.y, camera.x + camera.w, y - camera.y);
    }
    // Dessiner d'autres éléments (ex: poissons ou obstacles)
}

void Map::update() {
    // Logique pour mettre à jour les éléments de la carte
}
