#ifndef _TOOLS_MATH_VEC2_H
#define _TOOLS_MATH_VEC2_H

#include <cmath>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>

namespace tools::math {

template <typename T>
class Vec2 {
public:
    Vec2() : i(0), j(0) {}
    explicit Vec2(T iValue, T jValue) : i(iValue), j(jValue) {}
    explicit Vec2(Vec2<T>& other) : i(other.i), j(other.j) {}
    Vec2(const Vec2<T>& other) : i(other.i), j(other.j) {}
    explicit Vec2(Vec2<T>&& other) = default;

    T i;
    T j;

    T mag() const {
        return std::sqrt(i * i + j * j);
    }

    T dot(const Vec2<T>& other) {
        return i * other.i + j * other.j;
    }

    T cross(const Vec2<T>& other){
        return i * other.j - j * other.i;
    }

    Vec2<T> scalarMultiply(T multiple) {
        return Vec2<T>(i * multiple, j * multiple);
    }

    Vec2<T> rotate(T radians) {
        T sinVal = sin(radians);
        T cosVal = cos(radians);
        return Vec2<T>(i * cosVal - j * sinVal, i * sinVal + j * cosVal);
    }

    constexpr T& operator[](std::size_t pos);
    constexpr Vec2<T> operator*(T multiple) {
        return scalarMultiply(multiple);
    }
    constexpr Vec2<T> operator+(const Vec2<T>& other) {
        return Vec2<T>(i + other.i, j + other.j);
    }
    constexpr void operator+=(const Vec2<T>& other) {
        i += other.i;
        j += other.j;
    }

    constexpr const Vec2<T> operator-(const Vec2<T>& other) {
        return Vec2<T>(i - other.i, j - other.j);
    }
    Vec2<T> operator-(Vec2<T>& other) const {
        return Vec2<T>(i - other.i, j - other.j);
    }

    constexpr Vec2<T> operator-() {
        return scalarMultiply(-1);
    }

    bool operator==(const Vec2<T>& other) const {
        return i == other.i && j == other.j;
    }

    bool operator!=(const Vec2<T>& other) const {
        return !(*this == other);
    }

    bool operator<(const Vec2<T>& other) const {
        if (i < other.i) {
            return true;
        } else if (i == other.i) {
            return j < other.j;
        }
        return false;
    }

    Vec2<T>& operator=(const Vec2<T>& other) {
        i = other.i;
        j = other.j;
        return *this;
    }

    std::string toString() {
        std::ostringstream stream;
        stream << *this;
        return stream.str();
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& outs, const Vec2<T>& vec) {
  return outs << "(" << vec.i << ", " << vec.j << ")";
}

template <typename T>
constexpr Vec2<T> operator*(T multiple, const Vec2<T>& vec) {
    return vec.scalarMultiply(multiple);
}

template <typename T>
struct Vec2Hasher {
    std::size_t operator()(const Vec2<T>& k) const {
        using std::hash;
        return hash<T>()(k.i) ^ (hash<T>()(k.j) << 1);
    }
};

}

#endif