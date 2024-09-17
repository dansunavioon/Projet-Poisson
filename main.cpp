#include "Poisson/Poisson.h"

int main() {
    Simulation simulation(100);  // Simulation avec 100 poissons
    simulation.run(1000);        // Exécuter la simulation pour 1000 étapes
    return 0;
}
