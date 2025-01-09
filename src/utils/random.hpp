#ifndef RANDOM
#define RANDOM

#include "Position.hpp"
#include <climits>
#include <cstdlib>

/*
    Used to generate chunks pseudo-randomly
    Insures that if a chunk is loaded twice, it will contain the same obstacles at the same places
*/
struct randomSeed {
    unsigned value;

    randomSeed(std::pair<int, int> seed) : value(seed.first) {}
};



inline int randomInt(int const min, int const max) {
    return min + rand() % (max + 1 - min);
}

inline float randomFloat() {
    return (float) rand() / INT_MAX;
}

inline float randomFloat(float const min, float const max) {
    return min + (max - min) * (randomFloat());
}

inline Position<float> randomDiskSampling(float const diskSize) {
    float const r = std::sqrt(randomFloat()) * diskSize;
    float const theta = randomFloat(0, 2*M_PIf);
    return Position<float>(r * std::cos(theta), r * std::sin(theta));
}


inline Position<float> randomDiskSampling(float const minDiskSize, float const maxDiskSize) {
    float const r = minDiskSize + (maxDiskSize - minDiskSize) * std::sqrt(randomFloat());
    float const theta = randomFloat(0, 2*M_PIf);
    return Position<float>(r * std::cos(theta), r * std::sin(theta));
}

#endif //RANDOM