// Implementation for Grid
#include "grid.h"

GridData::GridData(unsigned int w, unsigned int h /*, PooledAllocator& allocator */) 
    : width(w), height(h), padded_width(w), padded_height(h) {
    // TODO: Initialize SoA vectors using the pooled allocator.
    // Ensure data is aligned for AVX2 if necessary (e.g., pad rows).
    // The 64KB tile size might influence how padding or allocation per tile is handled.

    // Example for one field, assuming direct allocation for now
    // u_velocity.resize(padded_width * padded_height); // Or use allocator.allocate
}
