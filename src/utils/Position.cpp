#include "Position.hpp"

inline int clamp(int x, int min, int max) {
    return std::min(max, std::max(x, min));
}




template<typename T>
bool Position<T>::operator<(Position const& other) const {
    return i < other.i || (i == other.i && j < other.j);
}


template<typename T>
bool Position<T>::operator>(Position const& other) const {
    return i > other.i || (i == other.i && j > other.j);
}


