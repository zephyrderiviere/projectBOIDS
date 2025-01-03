#pragma once

#include <cmath>
#include <iostream>
#include <ostream>


template<typename T>
inline bool inBounds(T x, T min, T max) {
    return min <= x && x <= max;
}


template <typename T>
struct Position {
    T i, j;

    Position() : i(0), j(0) {}
    Position(T i, T j) : i(i), j(j) {}

    Position(Position const& other) {
        i = other.i;
        j = other.j;
    }

    

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
    inline void operator+=(Position const& other) {
        i += other.i; j += other.j;
    }
    inline void operator-=(Position const& other) {
        i -= other.i; j -= other.j;
    }
    inline void operator*=(T const a) {
        i *= a; j *= a;
    }
    bool operator<(Position const& other) const;
    bool operator>(Position const& other) const;

    inline T normSquared() const {
        return i*i + j*j;
    }

    inline T norm() const {
        return std::sqrt(normSquared());
    }

    inline Position<T> operator/(T a) const {
        return Position(i / a, j / a);
    }

    Position<T> normalize() const {
        return (*this) / norm();
    }


    inline T operator*(Position<T> const& other) const {
        return i * other.i + j * other.j;
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
std::ostream& operator<<(std::ostream& o, Position<T> p) {
    return o << "(" << p.i << ", " << p.j << ")";
}

template class Position<int>;
template class Position<float>;


static Position<float> const ux (1, 0);
static Position<float> const uy (0, 1);



