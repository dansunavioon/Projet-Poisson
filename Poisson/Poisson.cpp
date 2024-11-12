#include "Poisson.h"
#include <cmath>
#include <cstdlib>
#include <SDL_image.h>

#define MAX_GROUPS 5  // Nombre maximum de groupes

Poisson::Poisson(float x, float y, SDL_Renderer* renderer)
{
    position.x = static_cast<int>(x);
    position.y = static_cast<int>(y);
    velocity.x = (rand() % 3 - 1);
    velocity.y = (rand() % 3 - 1);

    independent = (rand() % 2 == 0);
    groupId = rand() % MAX_GROUPS;

    // Charger l'image du poisson en tant que texture
    SDL_Surface* surface = IMG_Load("poisson.png");
    if (!surface) {
        SDL_Log("Erreur de chargement de l'image poisson.png : %s", SDL_GetError());
        poissonTexture = nullptr;
    } else {
        poissonTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

Poisson::~Poisson()
{
    if (poissonTexture) {
        SDL_DestroyTexture(poissonTexture);
    }
}

void Poisson::update(const std::vector<Poisson>& poissons)
{
    applyBehaviors(poissons);

    velocity.x += (rand() % 3 - 1) * 0.1f;
    velocity.y += (rand() % 3 - 1) * 0.1f;

    position.x += velocity.x;
    position.y += velocity.y;

    if (position.x > 800) position.x = 0;
    else if (position.x < 0) position.x = 800;
    if (position.y > 600) position.y = 0;
    else if (position.y < 0) position.y = 600;
}

void Poisson::draw(SDL_Renderer* renderer) const
{
    if (poissonTexture) {
        SDL_Rect dstRect = { position.x, position.y, 20, 20 };  // Ajustez la taille en fonction de votre image
        SDL_RenderCopy(renderer, poissonTexture, nullptr, &dstRect);
    }
}

void Poisson::applyBehaviors(const std::vector<Poisson>& poissons)
{
    if (!independent) {
        SDL_Point cohesion = cohesionBehavior(poissons);
        SDL_Point separation = separationBehavior(poissons);

        velocity.x += cohesion.x + separation.x;
        velocity.y += cohesion.y + separation.y;
    }

    float speedLimit = 3.0f;
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > speedLimit) {
        velocity.x = (velocity.x / speed) * speedLimit;
        velocity.y = (velocity.y / speed) * speedLimit;
    }
}

SDL_Point Poisson::cohesionBehavior(const std::vector<Poisson>& poissons)
{
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 50.0f;

    for (const auto& other : poissons) {
        if (&other != this && other.groupId == groupId) {
            float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
            if (distance < perceptionRadius) {
                steering.x += other.position.x;
                steering.y += other.position.y;
                total++;
            }
        }
    }

    if (total > 0) {
        steering.x /= total;
        steering.y /= total;

        steering.x -= position.x;
        steering.y -= position.y;

        float speedLimit = 4.0f;
        float speed = std::sqrt(steering.x * steering.x + steering.y * steering.y);
        if (speed > speedLimit) {
            steering.x = (steering.x / speed) * speedLimit;
            steering.y = (steering.y / speed) * speedLimit;
        }
    }

    return steering;
}

SDL_Point Poisson::separationBehavior(const std::vector<Poisson>& poissons)
{
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 60.0f;

    for (const auto& other : poissons) {
        float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
        if (&other != this && distance < perceptionRadius) {
            float diff_x = position.x - other.position.x;
            float diff_y = position.y - other.position.y;

            steering.x += diff_x / (distance * distance + 0.1f);
            steering.y += diff_y / (distance * distance + 0.1f);

            total++;
        }
    }

    if (total > 0) {
        steering.x /= total;
        steering.y /= total;

        float speedLimit = 4.0f;
        float speed = std::sqrt(steering.x * steering.x + steering.y * steering.y);
        if (speed > speedLimit) {
            steering.x = (steering.x / speed) * speedLimit;
            steering.y = (steering.y / speed) * speedLimit;
        }
    }

    return steering;
}