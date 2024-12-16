#include "SharkBoss.h"
#include <cmath>
#include <iostream>

SharkBoss::SharkBoss(SDL_Renderer* renderer, float x, float y): renderer(renderer), position{static_cast<int>(x), static_cast<int>(y)}, velocity{0, 0}, texture(nullptr), angle(0.0)
{
    // Chargement de la texture du requin
    SDL_Surface* tempSurface = SDL_LoadBMP("Image_SharkBoss/SharkBoss.bmp");
    if (tempSurface)
    {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        if (!texture)
        {
            std::cerr << "Erreur de création de la texture SharkBoss : " << SDL_GetError() << std::endl;
        }
    }
    else
    {
        std::cerr << "Erreur de chargement de l'image SharkBoss.bmp : " << SDL_GetError() << std::endl;
    }
}

SharkBoss::~SharkBoss()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}

void SharkBoss::update(const std::vector<Poisson>& poissons)
{
    chaseFish(poissons);

    // Mise à jour de la position
    position.x += velocity.x;
    position.y += velocity.y;

    angle = std::atan2(velocity.y, velocity.x) * 180 / M_PI;

    // Si le poisson dépasse la limite droite
    if (position.x >= MAP_WIDTH-20) {
        angle = 180.0f - angle;  // Inverser l'angle horizontal
        position.x = MAP_WIDTH - 20;  // Fixer la position au bord droit
    }
    // Si le poisson dépasse la limite gauche
    else if (position.x < 20) {
        angle = 180.0f - angle;  // Inverser l'angle horizontal
        position.x = 20;  // Fixer la position au bord gauche
    }

    // Si le poisson dépasse la limite inférieure
    if (position.y >= MAP_HEIGHT-20) {
        angle = -angle;  // Inverser l'angle vertical
        position.y = MAP_HEIGHT - 20;  // Fixer la position au bord inférieur
    }
    // Si le poisson dépasse la limite supérieure
    else if (position.y < 20) {
        angle = -angle;  // Inverser l'angle vertical
        position.y = 20;  // Fixer la position au bord supérieur
    }


    // Limiter la vitesse
    float speedLimit = 4.0f;
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > speedLimit)
    {
        velocity.x = static_cast<int>((velocity.x / speed) * speedLimit);
        velocity.y = static_cast<int>((velocity.y / speed) * speedLimit);
    }
}

void SharkBoss::draw(SDL_Renderer* renderer, const SDL_Point& cameraPosition) const
{
    if (texture)
    {
        SDL_Rect renderQuad = {
            position.x - cameraPosition.x,
            position.y - cameraPosition.y,
            175, 125 // Dimensions du requin
        };
        SDL_Point center = {16, 16}; // Point central pour la rotation

        // Déterminer si l'image doit être retournée horizontalement
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        // Retourner l'image si la vélocité x est négative (le poisson va vers la gauche)
        if (velocity.x < 0)
        {
            flip = SDL_FLIP_HORIZONTAL; // Retourner l'image horizontalement
            flip = SDL_FLIP_VERTICAL;
        }

        // Dessiner l'image avec l'angle de rotation et l'éventuel retournement
        SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, angle, &center, flip);
    }
}

void SharkBoss::chaseFish(const std::vector<Poisson>& poissons)
{
    SDL_Point target = {0, 0};
    int closestDistance = INT_MAX;

    for (const auto& poisson : poissons)
    {
        SDL_Point fishPosition = poisson.getPosition(); // Utilisation du getter
        int dx = fishPosition.x - position.x;
        int dy = fishPosition.y - position.y;
        int distanceSquared = dx * dx + dy * dy;

        if (distanceSquared < closestDistance)
        {
            closestDistance = distanceSquared;
            target.x = fishPosition.x;
            target.y = fishPosition.y;
        }
    }

    // Ajuster la vélocité pour chasser le poisson cible
    if (closestDistance != INT_MAX)
    {
        velocity.x = target.x - position.x-1.5f;
        velocity.y = target.y - position.y-1.5f;
    }
}
void SharkBoss::hunt(std::vector<Poisson>& poissons)
{
    const int captureRadius = 25; // Distance pour capturer un poisson
    for (auto& poisson : poissons)
    {
        // Vérifier la distance entre le requin et le poisson
        int dx = poisson.getPosition().x - position.x;
        int dy = poisson.getPosition().y - position.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < captureRadius && !poisson.isCaptured())
        {
            // Marquer le poisson comme capturé et le déplacer hors de la carte
            poisson.setCaptured(true);

            // Ajouter un délai pour réapparaître
            poisson.setRespawnTime(SDL_GetTicks() + 3000); // 3 secondes
        }
    }
}