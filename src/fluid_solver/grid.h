// Placeholder for Grid class declaration (SoA, tiling)
#ifndef GRID_H
#define GRID_H

#include <vector>
// May need memory/pooled_allocator.h

struct GridData {
    // Structure of Arrays (SoA) for fluid properties
    // Example:
    // std::vector<float> u_velocity; // or use custom pooled allocator
    // std::vector<float> v_velocity;
    // std::vector<float> density;
    // std::vector<float> pressure;
    // ... other fields needed for Navier-Stokes

    unsigned int width;
    unsigned int height;
    unsigned int padded_width; // For alignment or boundary handling
    unsigned int padded_height;

    // Constructor to initialize based on dimensions and allocator
    GridData(unsigned int w, unsigned int h /*, PooledAllocator& allocator */);
};

#endif // GRID_H
