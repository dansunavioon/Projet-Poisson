#include "Poisson.h"

#include <SDL_image.h>


Poisson::Poisson(SDL_Renderer* renderer, float x, float y, bool independent)
    : renderer(renderer), independent(independent), angle(0.0), captured(false), respawnTime(0)
{
    position.x = static_cast<int>(x);
    position.y = static_cast<int>(y);
    velocity.x = (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1);
    velocity.y = (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1);

    SDL_Surface* tempSurface = IMG_Load("image_Poisson/poisson.png");
    if (tempSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        if (!texture) {
            std::cerr << "Erreur de création de la texture : " << SDL_GetError() << std::endl;
        }
    } else {
        std::cerr << "Erreur de chargement de l'image Poisson12.png : " << IMG_GetError() << std::endl;
        texture = nullptr;
    }

    groupId = rand() % MAX_GROUPS;
}

Poisson::~Poisson() {
    SDL_DestroyTexture(texture);
}

void Poisson::update(const std::vector<Poisson>& poissons)
{
    applyBehaviors(poissons);

    position.x += velocity.x;
    position.y += velocity.y;

    angle = std::atan2(velocity.y, velocity.x) * 180 / M_PI;

    // Corriger la position si elle dépasse les limites de la carte
    if (position.x >= MAP_WIDTH) position.x = 0;
    else if (position.x < 0) position.x = MAP_WIDTH;

    if (position.y >= MAP_HEIGHT) position.y = 0;
    else if (position.y < 0) position.y = MAP_HEIGHT;
}

void Poisson::draw(SDL_Renderer* renderer, int cameraX, int cameraY) const
{
    if (texture)
    {
        SDL_Rect renderQuad = {position.x - cameraX, position.y - cameraY, 54, 54}; // Ajustez la taille de l'image si nécessaire
        SDL_Point center = {16, 16}; // Point central pour la rotation

        // Déterminer si l'image doit être retournée horizontalement
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        // Retourner l'image si la vélocité x est négative (le poisson va vers la gauche)
        if (velocity.x < 0)
        {
            flip = SDL_FLIP_HORIZONTAL; // Retourner l'image horizontalement
        }

        // Dessiner l'image avec l'angle de rotation et l'éventuel retournement
        SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, angle, &center, flip);
    }
}

// Comportements collectifs
void Poisson::applyBehaviors(const std::vector<Poisson>& poissons)
{
    if (!independent)
    {
        SDL_Point cohesion = cohesionBehavior(poissons);
        SDL_Point separation = separationBehavior(poissons);

        velocity.x += cohesion.x + separation.x;
        velocity.y += cohesion.y + separation.y;
    }

    // Limiter la vitesse
    float speedLimit = 3.0f;
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > speedLimit)
    {
        velocity.x = (velocity.x / speed) * speedLimit;
        velocity.y = (velocity.y / speed) * speedLimit;
    }
}

SDL_Point Poisson::cohesionBehavior(const std::vector<Poisson>& poissons)
{
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 40.0f;

    for (const auto& other : poissons)
    {
        if (&other != this && other.groupId == groupId)
        {
            float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
            if (distance < perceptionRadius)
            {
                steering.x += other.position.x;
                steering.y += other.position.y;
                total++;
            }
        }
    }

    if (total > 0)
    {
        steering.x /= total;
        steering.y /= total;

        steering.x -= position.x;
        steering.y -= position.y;
    }

    return steering;
}

SDL_Point Poisson::separationBehavior(const std::vector<Poisson>& poissons)
{
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 20.0f;

    for (const auto& other : poissons)
    {
        float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
        if (&other != this && distance < perceptionRadius)
        {
            steering.x += position.x - other.position.x;
            steering.y += position.y - other.position.y;
            total++;
        }
    }

    if (total > 0)
    {
        steering.x /= total;
        steering.y /= total;
    }

    return steering;
}

void Poisson::respawnIfNeeded(int mapWidth, int mapHeight)
{
    if (captured && SDL_GetTicks() > respawnTime)
    {
        // Réapparaître en dehors des limites de la carte
        int side = rand() % 4; // 0: gauche, 1: droite, 2: haut, 3: bas
        switch (side)
        {
        case 0: // Gauche
            position.x = -50;
            position.y = rand() % mapHeight;
            break;
        case 1: // Droite
            position.x = mapWidth + 50;
            position.y = rand() % mapHeight;
            break;
        case 2: // Haut
            position.x = rand() % mapWidth;
            position.y = -50;
            break;
        case 3: // Bas
            position.x = rand() % mapWidth;
            position.y = mapHeight + 50;
            break;
        }

        // Donner une vitesse initiale vers l'intérieur de la carte
        velocity.x = (mapWidth / 2 - position.x) * 0.01f;
        velocity.y = (mapHeight / 2 - position.y) * 0.01f;

        captured = false;
    }
}