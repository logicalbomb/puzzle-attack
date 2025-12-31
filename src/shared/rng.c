#include "rng.h"
#include <time.h>

void RNG_Init(RNG* rng, uint64_t seed)
{
    if (seed == 0) {
        seed = (uint64_t)time(NULL);
    }
    // Ensure non-zero state
    rng->seed = seed ? seed : 1;
    rng->state = rng->seed;
}

uint64_t RNG_Next(RNG* rng)
{
    // xorshift64 algorithm
    uint64_t x = rng->state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    rng->state = x;
    return x;
}

int RNG_Range(RNG* rng, int max)
{
    if (max <= 0) return 0;
    return (int)(RNG_Next(rng) % (uint64_t)max);
}

uint64_t RNG_GetSeed(const RNG* rng)
{
    return rng->seed;
}
