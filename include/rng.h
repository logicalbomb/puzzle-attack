#ifndef RNG_H
#define RNG_H

#include <stdint.h>

// RNG state using xorshift64
typedef struct {
    uint64_t seed;   // Original seed for display/replay
    uint64_t state;  // Current state
} RNG;

// Initialize RNG with seed (0 uses time-based seed)
void RNG_Init(RNG* rng, uint64_t seed);

// Generate random uint64
uint64_t RNG_Next(RNG* rng);

// Generate random int in range [0, max)
int RNG_Range(RNG* rng, int max);

// Get current seed (for display/saving)
uint64_t RNG_GetSeed(const RNG* rng);

#endif // RNG_H
