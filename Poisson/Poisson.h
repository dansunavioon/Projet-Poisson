#ifndef POISSON_H
#define POISSON_H
#include <SDL.h>
#include <vector>

class Poisson {
public:
    Poisson(SDL_Renderer* renderer, float x, float y, bool independent);
    ~Poisson();
    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer) const;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Point position;
    SDL_Point velocity;
    bool independent;
    int groupId;

    void applyBehaviors(const std::vector<Poisson>& poissons);
    SDL_Point cohesionBehavior(const std::vector<Poisson>& poissons);
    SDL_Point separationBehavior(const std::vector<Poisson>& poissons);

    static constexpr float MAX_SPEED = 2.0f;
    static constexpr float COHESION_FORCE = 0.05f;
    static constexpr float SEPARATION_FORCE = 0.1f;
    static const int MAX_GROUPS = 5;
};

#endif // POISSON_H
