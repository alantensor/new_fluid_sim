# Fluid Simulator

A 2D fluid simulator using C++20, SFML, TBB, and AVX2.

## Features

*   Navier-Stokes solver
*   Grid partitioning and work-stealing with TBB
*   AVX2 vectorized updates
*   Structure of Arrays (SoA) memory layout
*   Pooled memory allocator
*   Checkpointing with Zstd compression

## Building

This project uses CMake. 

1.  Ensure SFML, TBB, and Zstd development libraries are installed.
2.  Create a build directory: `mkdir build && cd build`
3.  Run CMake: `cmake ..`
4.  Build: `make` (or your chosen build system's command)

## Running

The executable will be in `build/FluidSimulator` (or `build/bin/FluidSimulator` after install).
