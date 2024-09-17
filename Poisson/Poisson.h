#ifndef POISSON_H
#define POISSON_H

#include <vector>
#include <SDL.h>

class Poisson {
public:
    float x, y;   // Position
    float vx, vy; // Vitesse

    Poisson();       // Constructeur
    void update(const std::vector<Poisson>& poissons); // Mise à jour
    void draw(SDL_Renderer* renderer);  // Dessiner le poisson

private:
    float distance(const Poisson& other) const;     // Calcul de la distance
    void separation(const std::vector<Poisson>& poissons);  // Évite la collision
    void alignement(const std::vector<Poisson>& poissons);  // Ajuste la vitesse
    void cohesion(const std::vector<Poisson>& poissons);    // Se rapproche du centre
};

class Simulation {
public:
    Simulation(int num_poissons);            // Constructeur
    void run(int steps);                     // Exécuter la simulation

private:
    std::vector<Poisson> poissons;           // Liste des poissons
    SDL_Window* window;                      // Fenêtre SDL
    SDL_Renderer* renderer;                  // Rendu SDL
    bool init_SDL();                         // Initialiser SDL
    void init_poissons();                    // Initialise les poissons
    void update_poissons();                  // Met à jour tous les poissons
    void render();                           // Afficher les poissons
    void clean_up();                         // Nettoyer SDL
};

#endif // POISSON_H
