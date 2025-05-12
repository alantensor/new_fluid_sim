#include <SFML/Graphics.hpp>
#include <iostream>

// Forward declarations or includes for other components will go here
// #include "fluid_solver/solver.h"
// #include "io/checkpoint.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Fluid Simulator");
    window.setFramerateLimit(60);

    // TODO: Initialize FluidSolver
    // FluidSolver solver(GRID_WIDTH, GRID_HEIGHT, TILE_SIZE);

    // TODO: Initialize TBB (usually done implicitly when using TBB algorithms)

    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // TODO: Handle other events (e.g., keyboard input for saving/loading, parameters)
        }

        float dt = deltaClock.restart().asSeconds();

        // TODO: Update solver
        // solver.update(dt);

        window.clear(sf::Color::Black);

        // TODO: Render solver state
        // solver.render(window);

        window.display();
    }

    return 0;
}
