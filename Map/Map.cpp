#include "Map.h"

void Map::initPoissons(int nbPoissons) {
    for (int i = 0; i < nbPoissons; i++) {
        float x = rand() % width;
        float y = rand() % height;
        poissons.emplace_back(renderer, x, y, false);
    }
}

Map::Map(SDL_Renderer* renderer, int width, int height)
        : renderer(renderer), width(width), height(height), backgroundTexture(nullptr) {

    // Chargez une image pour l'arrière-plan de la carte
    SDL_Surface* surface = IMG_Load("image_map/map.jpg");
    if (!surface) {
        SDL_Log("Erreur : Impossible de charger l'image : %s", IMG_GetError());
        return;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(this->renderer, surface);
    SDL_FreeSurface(surface);
    if (!backgroundTexture) {
        SDL_Log("Erreur : Impossible de créer la texture : %s", SDL_GetError());
    }

    initPoissons(50);
    sharkBoss = new Requin(renderer, 100, 100);
}

Map::~Map() {
    // Libérez la texture lorsque vous détruisez la carte
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
}

void Map::render(int cameraX, int cameraY) {
    if (!backgroundTexture) return;

    // Afficher l'arrière-plan de la carte
    SDL_Rect destRect = {0, 0, width, height};
    SDL_Rect srcRect = {cameraX, cameraY, width, height};
    SDL_RenderCopy(renderer, backgroundTexture, &srcRect, &destRect);


    for (Poisson& poisson : poissons)
    {
        poisson.respawnIfNeeded(MAP_WIDTH, MAP_HEIGHT);
        poisson.update(poissons);
        poisson.draw(renderer, cameraX, cameraY);
    }

    // Mettre à jour le requin (logique de chasse)
    if (sharkBoss) {
        sharkBoss->hunt(poissons);
        sharkBoss->update(poissons);
        sharkBoss->draw(renderer, cameraX, cameraY);
    }
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

