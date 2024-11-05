#include "Poisson.h"
#include <cmath>
#include <algorithm>

Poisson::Poisson(float x, float y) {
    position.x = static_cast<int>(x);
    position.y = static_cast<int>(y);
    velocity.x = 0;
    velocity.y = 0;
}

void Poisson::update(const std::vector<Poisson>& poissons) {
    applyBehaviors(poissons);
    position.x += velocity.x;
    position.y += velocity.y;

    // Wrap around screen
    if (position.x > 800) position.x = 0;
    else if (position.x < 0) position.x = 800;
    if (position.y > 600) position.y = 0;
    else if (position.y < 0) position.y = 600;
}

void Poisson::draw(SDL_Renderer* renderer) const {  // Ajout de 'const' ici
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, position.x, position.y);
}

void Poisson::applyBehaviors(const std::vector<Poisson>& poissons) {
    SDL_Point alignment = align(poissons);
    SDL_Point cohesion = cohesionBehavior(poissons);  // Utilisation du nouveau nom
    SDL_Point separation = separationBehavior(poissons);  // Utilisation du nouveau nom

    velocity.x += alignment.x + cohesion.x + separation.x;
    velocity.y += alignment.y + cohesion.y + separation.y;

    // Limit speed
    float speedLimit = 4.0f;
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > speedLimit) {
        velocity.x = (velocity.x / speed) * speedLimit;
        velocity.y = (velocity.y / speed) * speedLimit;
    }
}

SDL_Point Poisson::align(const std::vector<Poisson>& poissons) {
    // Align with nearby poissons
    // (Implementation omitted for brevity)
    return {0, 0};
}

SDL_Point Poisson::cohesionBehavior(const std::vector<Poisson>& poissons) {
    // Move towards the average position of nearby poissons
    // (Implementation omitted for brevity)
    return {0, 0};
}

SDL_Point Poisson::separationBehavior(const std::vector<Poisson>& poissons) {
    // Avoid crowding nearby poissons
    // (Implementation omitted for brevity)
    return {0, 0};
}