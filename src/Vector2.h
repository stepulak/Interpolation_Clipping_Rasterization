#pragma once

#include <SDL2/SDL_rect.h>
#include <sstream>
#include <string>
#include <type_traits>

namespace Utils {

template <typename T>
struct Vector2 {

    static_assert(std::is_arithmetic<T>::value, "Vector2 musts have arithmetic members!");

    T x;
    T y;

    constexpr Vector2()
        : Vector2(0, 0)
    {
    }

    constexpr Vector2(T x_, T y_)
        : x(x_)
        , y(y_)
    {
    }

    constexpr Vector2(const SDL_Point& p)
        : Vector2(static_cast<T>(p.x), static_cast<T>(p.y))
    {
    }

    // Create vector from substracting p1 from p2
    Vector2(const Vector2<T>& v1, const Vector2<T>& v2)
        : Vector2(v2 - v1)
    {
    }

    Vector2(const SDL_Point& p1, const SDL_Point& p2)
        : Vector2(static_cast<T>(p2.x - p1.x), static_cast<T>(p2.y - p1.y))
    {
    }

    Vector2(const Vector2<T>& v2) = default;
    Vector2(Vector2<T>&& v2) = default;

    Vector2<T> CountNormal() const
    {
        return Vector2(-y, x);
    }

    T Dot(const Vector2<T>& v2) const
    {
        return x * v2.x + y * v2.y;
    }

    T GetLength() const
    {
        return static_cast<T>(SDL_sqrt(x * x + y * y));
    }

    // "Point" function
    T GetPointDistance(const Vector2<T>& v2) const
    {
        return (*this - v2).GetLength();
    }

    Vector2<T> CountPointMiddle(const Vector2<T>& v2) const
    {
        return Vector2((x + v2.x) / 2, (y + v2.y) / 2);
    }

    SDL_Point ToPoint() const
    {
        return { static_cast<int>(x), static_cast<int>(y) };
    }

    std::string ToString() const
    {
        std::stringstream ss;
        ss << "[" << x << "; " << y << "]";
        return ss.str();
    }

    // Overloaded operators
    template <typename T2>
    operator Vector2<T2>() const
    {
        return Vector2<T2>(static_cast<T2>(x), static_cast<T2>(y));
    }

    Vector2<T>& operator=(const SDL_Point& p)
    {
        x = p.x, y = p.y;
        return *this;
    }

    Vector2<T>& operator=(const Vector2<T>&) = default;
    Vector2<T>& operator=(Vector2<T>&&) = default;

    Vector2<T> operator+(const Vector2<T>& v2) const
    {
        return Vector2(x + v2.x, y + v2.y);
    }

    Vector2<T>& operator+=(const Vector2<T>& v2)
    {
        *this = v2 + *this;
        return *this;
    }

    Vector2<T> operator-(const Vector2<T>& v2) const
    {
        return Vector2(x - v2.x, y - v2.y);
    }

    Vector2<T>& operator-=(const Vector2<T>& v2)
    {
        *this = *this - v2;
        return *this;
    }

    Vector2<T> operator*(const Vector2<T>& v2) const
    {
        return Vector2(x * v2.x, y * v2.y);
    }

    Vector2<T>& operator*=(const Vector2<T>& v2)
    {
        *this = v2 * (*this);
        return *this;
    }

    Vector2<T> operator/(const Vector2<T>& v2) const
    {
        return Vector2(x / v2.x, y / v2.y);
    }

    Vector2<T>& operator/=(const Vector2<T>& v2)
    {
        *this = *this / v2;
        return *this;
    }

    // constant multiplication
    Vector2<T> operator*(T k) const
    {
        return Vector2(static_cast<T>(x * k), static_cast<T>(y * k));
    }

    Vector2<T>& operator*=(T k)
    {
        *this = *this * k;
        return *this;
    }

    Vector2<T> operator/(T k) const
    {
        return Vector2(static_cast<T>(x / k), static_cast<T>(y / k));
    }

    Vector2<T>& operator/=(T k)
    {
        *this = *this / k;
        return *this;
    }

    bool operator==(const Vector2<T>& v2) const
    {
        return x == v2.x && y == v2.y;
    }

    bool operator!=(const Vector2<T>& v2) const
    {
        return !(*this == v2);
    }
};

using Vector22i = Vector2<int>;
using Vector22f = Vector2<float>;
using Vec2i = Vector22i;
using Vec2f = Vector22f;

} // namespace Utils
