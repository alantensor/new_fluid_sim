// Placeholder for Checkpoint class/functions declaration
#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <string>
// #include "../fluid_solver/grid.h" // Or solver.h
#include <zstd.h>

namespace Checkpoint {
    bool saveState(const std::string& filePath /*, const GridData& grid */ );
    bool loadState(const std::string& filePath /*, GridData& grid */);

    // Helper for zstd compression/decompression if needed directly
}

#endif // CHECKPOINT_H
