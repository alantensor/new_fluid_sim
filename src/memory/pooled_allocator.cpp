// Implementation for PooledAllocator
#include "pooled_allocator.h"
#include <stdexcept> // For std::bad_alloc or std::runtime_error
#include <algorithm> // For std::fill

PooledAllocator::PooledAllocator(size_t blockSize, size_t blockCount)
    : blockSize_(blockSize) {
    if (blockSize == 0 || blockCount == 0) {
        throw std::invalid_argument("Block size and count must be positive.");
    }
    try {
        memoryPool_.resize(blockSize_ * blockCount);
    } catch (const std::bad_alloc& e) {
        // Handle cases where the pool itself cannot be allocated
        throw std::runtime_error("Failed to allocate memory pool: " + std::string(e.what()));
    }
    
    // Initialize free list: add all blocks to the free list
    for (size_t i = 0; i < blockCount; ++i) {
        freeList_.push_back(memoryPool_.data() + i * blockSize_);
    }
}

PooledAllocator::~PooledAllocator() {
    // Pool memory is managed by std::vector, will be freed automatically.
    // No explicit cleanup needed for freeList_ of pointers into memoryPool_.
}

void* PooledAllocator::allocate(size_t size) {
    // This simple version assumes 'size' is <= blockSize_ and returns one block.
    // A more complex allocator might handle varying sizes or check 'size'.
    if (size > blockSize_ && blockSize_ > 0) { // blockSize_ > 0 to prevent division by zero if it was not checked in constructor
        // Option 1: Throw error if requested size is too large for a single block
        throw std::bad_alloc(); // Or a custom exception
        // Option 2: Try to allocate multiple contiguous blocks (more complex)
        // Option 3: Fallback to new (not a good practice for a pool allocator's main path)
    }

    if (freeList_.empty()) {
        // No free blocks left
        throw std::bad_alloc(); // Or handle by expanding the pool, etc.
    }

    void* block = freeList_.front();
    freeList_.pop_front();
    return block;
}

void PooledAllocator::deallocate(void* ptr, size_t size) {
    // This simple version assumes 'ptr' is a block obtained from this pool.
    // 'size' is ignored here but might be used in more complex allocators.
    if (ptr == nullptr) return;

    // Basic validation: check if ptr is within the pool's memory range
    // More robust checking would involve ensuring it's on a block boundary.
    unsigned char* char_ptr = static_cast<unsigned char*>(ptr);
    if (char_ptr < memoryPool_.data() || char_ptr >= memoryPool_.data() + memoryPool_.size()) {
        // Pointer is not from this pool, or at least not from the main buffer
        // This could be an error, or it could mean it was allocated elsewhere if there's a fallback.
        // For a strict pool, this is an error.
        throw std::runtime_error("Deallocate: pointer not in pool range.");
    }
    if ((char_ptr - memoryPool_.data()) % blockSize_ != 0) {
         throw std::runtime_error("Deallocate: pointer not on block boundary.");
    }

    // Add the block back to the free list
    // To prevent double-free, could check if it's already in freeList_ (expensive)
    // or use a debug mechanism to mark blocks as allocated/free.
    freeList_.push_back(ptr);
}
