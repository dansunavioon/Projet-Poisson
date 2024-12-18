#include "Simulation.h"

int main(int argc, char* argv[]) {
    Simulation simulation;

    if (!simulation.initialize()) {
        return -1;
    }

    simulation.run();
    return 0;
}