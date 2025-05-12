// Implementation for FluidSolver
#include "solver.h"
#include "grid.h"
// #include "../memory/pooled_allocator.h"
// #include "../utils/vector_math.h"
#include <tbb/parallel_for.h>
#include <tbb/blocked_range2d.h>
#include <iostream>

FluidSolver::FluidSolver(unsigned int gridWidth, unsigned int gridHeight, unsigned int tileSize)
    : gridWidth_(gridWidth), gridHeight_(gridHeight), tileSize_(tileSize) {
    numTilesX_ = (gridWidth_ + tileSize_ - 1) / tileSize_;
    numTilesY_ = (gridHeight_ + tileSize_ - 1) / tileSize_;
    std::cout << "FluidSolver initialized: " << gridWidth_ << "x" << gridHeight_ 
              << " grid, " << tileSize_ << "px tiles (" 
              << numTilesX_ << "x" << numTilesY_ << " tiles)." << std::endl;
    initializeGrid();
}

FluidSolver::~FluidSolver() {
    // TODO: Clean up allocated memory (especially if using custom allocator)
    std::cout << "FluidSolver destroyed." << std::endl;
}

void FluidSolver::initializeGrid() {
    // TODO: Allocate memory for grid fields (u, v, density, etc.) using SoA and pooled allocator.
    // For now, just a placeholder.
    // Example: u_velocity_ = (float*)allocator.allocate(gridWidth_ * gridHeight_ * sizeof(float));
    std::cout << "Grid initialized (placeholder)." << std::endl;
}

void FluidSolver::update(float dt) {
    // Main simulation loop steps:
    // 1. Add forces (e.g., mouse interaction, gravity)
    // 2. Advect
    // 3. Diffuse
    // 4. Project (enforce incompressibility)

    // Parallelize tile processing using TBB
    tbb::parallel_for(tbb::blocked_range2d<unsigned int>(0, numTilesY_, 0, numTilesX_),
        [=](const tbb::blocked_range2d<unsigned int>& r) {
            for (unsigned int tileY = r.rows().begin(); tileY != r.rows().end(); ++tileY) {
                for (unsigned int tileX = r.cols().begin(); tileX != r.cols().end(); ++tileX) {
                    processTile(tileX, tileY, dt);
                }
            }
        });
    // std::cout << "Solver updated with dt: " << dt << std::endl;
}

void FluidSolver::processTile(unsigned int tileX, unsigned int tileY, float dt) {
    // TODO: Implement Navier-Stokes steps for a single tile.
    // This is where AVX2 vectorization would be applied to operations within the tile.
    // Operations: advection, diffusion, projection for the cells in this tile.

    // Calculate the actual cell range for this tile
    unsigned int startX = tileX * tileSize_;
    unsigned int endX = std::min(startX + tileSize_, gridWidth_);
    unsigned int startY = tileY * tileSize_;
    unsigned int endY = std::min(startY + tileSize_, gridHeight_);

    // Placeholder: loop over cells in the tile
    for (unsigned int y = startY; y < endY; ++y) {
        for (unsigned int x = startX; x < endX; ++x) {
            // Perform calculations for cell (x,y)
            // e.g., density_[y * gridWidth_ + x] *= 0.99f; // Simple decay
        }
    }
}

void FluidSolver::render(sf::RenderTarget& target) {
    // TODO: Render the grid (e.g., density) to the SFML RenderTarget.
    // This might involve creating sf::VertexArray or sf::Image/Texture.
    // For now, draw tile outlines as a placeholder.

    sf::RectangleShape tileShape;
    tileShape.setFillColor(sf::Color::Transparent);
    tileShape.setOutlineColor(sf::Color(50, 50, 50));
    tileShape.setOutlineThickness(1.0f);

    for (unsigned int ty = 0; ty < numTilesY_; ++ty) {
        for (unsigned int tx = 0; tx < numTilesX_; ++tx) {
            tileShape.setPosition(tx * tileSize_, ty * tileSize_);
            tileShape.setSize(sf::Vector2f(tileSize_, tileSize_));
            // Adjust size for edge tiles if necessary
            unsigned int currentTileWidth = (tx == numTilesX_ - 1) ? (gridWidth_ - tx * tileSize_) : tileSize_;
            unsigned int currentTileHeight = (ty == numTilesY_ - 1) ? (gridHeight_ - ty * tileSize_) : tileSize_;
            tileShape.setSize(sf::Vector2f(currentTileWidth, currentTileHeight));
            target.draw(tileShape);
        }
    }
    // Example: render density
    /*
    sf::Image image;
    image.create(gridWidth_, gridHeight_);
    for (unsigned int y = 0; y < gridHeight_; ++y) {
        for (unsigned int x = 0; x < gridWidth_; ++x) {
            float density = density_[y * gridWidth_ + x];
            sf::Uint8 val = static_cast<sf::Uint8>(std::min(std::max(density * 255.f, 0.f), 255.f));
            image.setPixel(x, y, sf::Color(val, val, val));
        }
    }
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);
    target.draw(sprite);
    */
}
