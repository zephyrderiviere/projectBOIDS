#ifndef RANDOM
#define RANDOM

#include "Position.hpp"
#include <climits>
#include <cmath>
#include <cstdlib>

/*
    Used to generate chunks pseudo-randomly
    Insures that if a chunk is loaded twice, it will contain the same obstacles at the same places
*/
struct randomGenerator {
    private:
        unsigned value;
        inline void update() {
            value = (3453101*value + 56453393) % INT_MAX;
        }

    public:

    randomGenerator(std::pair<int, int> seed, unsigned n) {
        int p = (seed.first < 0) ? (-2 * seed.first) + 1 : 2*seed.first;
        int q = (seed.second < 0) ? (-2 * seed.second) + 1 : 2*seed.second;

        value = ((p+q) * (p+q+1) / 2) + n;
    }

    


    inline int randomInt() {
        update();
        return value;
    }

    inline int randomInt(int const min, int const max) {
        update();
        return min + value % (max + 1 - min);
    }

    inline float randomFloat() {
        update();
        return (float) value / INT_MAX;
    }

    inline float randomFloat(float const min, float const max) {
        return min + (max - min) * randomFloat();
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