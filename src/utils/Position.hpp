#pragma once

#include <cmath>
#include <complex>
#include <iostream>
#include <vector>


template<typename T>
inline bool inBounds(T x, T min, T max) {
    return min <= x && x <= max;
}


template <typename T>
struct Position {
    T i, j;

    Position() : i(0), j(0) {}
    Position(T i, T j) : i(i), j(j) {}

    bool operator==(Position const& other) const {
        return i == other.i && j == other.j;
    }
    bool operator!=(Position const& other) const {
        return !(operator==(other));
    }

    bool inRectangle(T i_min, T i_max, T j_min, T j_max) const {
        return inBounds(i, i_min, i_max) && inBounds(j, j_min, j_max);
    }

    inline Position operator+(Position const& other) const {
        return Position(i + other.i, j+other.j);
    }
    inline Position operator-(Position const& other) const {
        return Position(i - other.i, j - other.j);
    }
    bool operator<(Position const& other) const;
    bool operator>(Position const& other) const;


    void print() const {
        std::cout << "(" << i << ", " << j << ")" << std::endl;
    }
};

template <typename T>
inline Position<T> operator*(Position<T> const& other, T a) {
    return Position(a * other.i, a * other.j);
}

template <typename T>
inline Position<T> operator*(T a, Position<T> const& other) {
    return other * a;
}

template <typename T>
inline Position<T> operator/(Position<T> const& other, T a) {
    return Position(other.i / a, other.j / a);
}



template class Position<int>;
template class Position<unsigned long long>;



