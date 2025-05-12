cmake_minimum_required(VERSION 3.16)
project(FluidSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Dependencies ---
# SFML
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Intel TBB
find_package(TBB REQUIRED)

# Zstd
find_package(ZSTD REQUIRED)
if(NOT ZSTD_FOUND)
    message(FATAL_ERROR "Zstd library not found. Please install zstd and its development files.")
endif()

# --- Executable ---
add_executable(FluidSimulator src/main.cpp)

# --- Linking ---
target_link_libraries(FluidSimulator PRIVATE sfml-graphics sfml-window sfml-system TBB::tbb ZSTD::zstd)

# --- Include directories ---
target_include_directories(FluidSimulator PRIVATE
    ${SFML_INCLUDE_DIR}
    ${TBB_INCLUDE_DIRS}
    ${ZSTD_INCLUDE_DIRS}
)

# Optional: Add AVX2 support
# You might need to enable this explicitly depending on your compiler
# For GCC/Clang:
# target_compile_options(FluidSimulator PRIVATE -mavx2)
# For MSVC:
# target_compile_options(FluidSimulator PRIVATE /arch:AVX2)

message(STATUS "SFML libraries: ${SFML_LIBRARIES}")
message(STATUS "SFML include_dir: ${SFML_INCLUDE_DIR}")
message(STATUS "TBB libraries: ${TBB_LIBRARIES}")
message(STATUS "TBB include_dirs: ${TBB_INCLUDE_DIRS}")
message(STATUS "ZSTD library: ${ZSTD_LIBRARIES}")
message(STATUS "ZSTD include_dir: ${ZSTD_INCLUDE_DIRS}")

install(TARGETS FluidSimulator DESTINATION bin)
