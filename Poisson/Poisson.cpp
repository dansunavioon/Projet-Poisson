#include "poisson.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600
#define MAX_SPEED 4.0

// Constructeur Poisson
Poisson::Poisson() {
    x = rand() % WIDTH;
    y = rand() % HEIGHT;
    vx = ((float)rand() / RAND_MAX) * MAX_SPEED * 2 - MAX_SPEED;
    vy = ((float)rand() / RAND_MAX) * MAX_SPEED * 2 - MAX_SPEED;
}

// Calcul de la distance entre deux poissons
float Poisson::distance(const Poisson& other) const {
    return sqrtf((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}

// Règles : séparation, alignement, cohésion
void Poisson::separation(const std::vector<Poisson>& poissons) { /*...*/ }
void Poisson::alignement(const std::vector<Poisson>& poissons) { /*...*/ }
void Poisson::cohesion(const std::vector<Poisson>& poissons) { /*...*/ }

// Mise à jour de la position des poissons
void Poisson::update(const std::vector<Poisson>& poissons) { /*...*/ }

// Dessiner le poisson à l'écran
void Poisson::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);  // Orange pour les poissons
    SDL_Rect rect = {(int)x, (int)y, 5, 5};            // Un rectangle pour chaque poisson
    SDL_RenderFillRect(renderer, &rect);
}

// Constructeur Simulation
Simulation::Simulation(int num_poissons) : window(nullptr), renderer(nullptr) {
    poissons.resize(num_poissons);
    init_poissons();
    if (!init_SDL()) {
        std::cerr << "Erreur d'initialisation de SDL\n";
    }
}

// Initialisation SDL
bool Simulation::init_SDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erreur d'initialisation SDL: " << SDL_GetError() << "\n";
        return false;
    }
    window = SDL_CreateWindow("Simulation de Poissons", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Erreur de création de la fenêtre SDL: " << SDL_GetError() << "\n";
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Erreur de création du renderer: " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

// Initialisation des poissons
void Simulation::init_poissons() {
    for (auto& poisson : poissons) {
        poisson = Poisson();
    }
}

// Mettre à jour les poissons
void Simulation::update_poissons() {
    for (auto& poisson : poissons) {
        poisson.update(poissons);
    }
}

// Affichage des poissons
void Simulation::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Noir pour le fond
    SDL_RenderClear(renderer);

    for (auto& poisson : poissons) {
        poisson.draw(renderer);  // Dessiner chaque poisson
    }

    SDL_RenderPresent(renderer);
}

// Nettoyer SDL
void Simulation::clean_up() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Exécuter la simulation
void Simulation::run(int steps) {
    bool running = true;
    SDL_Event event;

    for (int step = 0; step < steps && running; ++step) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        update_poissons();
        render();
        SDL_Delay(16);  // Pour une cadence de ~60 FPS
    }

    clean_up();
}
