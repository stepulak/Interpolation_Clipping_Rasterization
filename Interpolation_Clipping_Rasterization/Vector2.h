#ifndef Vector22_H
#define Vector22_H

#include <type_traits>
#include <string>
#include <sstream>
#include <SDL2/SDL_rect.h>

namespace Utils {

	template<typename T>
	struct Vector2 {

		static_assert(std::is_arithmetic<T>::value, "Vector2 musts have arithmetic members!");

		T x;
		T y;

		Vector2() : x(0), y(0) {}
		Vector2(const Vector2<T>& v2) = default;
		Vector2(Vector2<T>&& v2) = default;

		Vector2(T x_, T y_) : x(x_), y(y_) {}
		Vector2(const SDL_Point& p) : x(static_cast<T>(p.x)), y(static_cast<T>(p.y)) {}

		// Create vector from substracting p1 from p2
		Vector2(const Vector2<T>& v1, const Vector2<T>& v2) : Vector2(v2 - v1) {}
		Vector2(const SDL_Point& p1, const SDL_Point& p2) 
			: Vector2(static_cast<T>(p2.x - p1.x), static_cast<T>(p2.y - p1.y)) {}

		inline Vector2<T> CountNormal() const { return Vector2(-y, x); }
		inline T Dot(const Vector2<T>& v2) const { return x * v2.x + y * v2.y; }
		inline T GetLength() const { return static_cast<T>(SDL_sqrt(x*x + y*y)); }

		// "Point" function
		inline T GetPointDistance(const Vector2<T>& v2) const { return (*this - v2).GetLength(); }
		inline Vector2<T> CountPointMiddle(const Vector2<T>& v2) const { return Vector2((x + v2.x) / 2, (y + v2.y) / 2); }
		
		inline SDL_Point ToPoint() const { return{ static_cast<int>(x), static_cast<int>(y) }; }

		inline std::string ToString() const {
			std::stringstream ss;
			ss << "[" << x << "; " << y << "]";
			return ss.str();
		}

		// Overloaded operators
		template<typename T2>
		inline operator Vector2<T2>() const {
			static_assert(std::is_arithmetic<T2>::value, "Unable to convert into non-arithmetic value");
			return Vector2<T2>(static_cast<T2>(x), static_cast<T2>(y));
		}

		inline Vector2<T>& operator=(const Vector2<T>& v2) = default;
		inline Vector2<T>& operator=(const SDL_Point& p) { x = p.x, y = p.y; return *this; }

		inline Vector2<T> operator+(const Vector2<T>& v2) const { return Vector2(x + v2.x, y + v2.y); }
		inline Vector2<T>& operator+=(const Vector2<T>& v2) { *this = v2 + *this; return *this; }

		inline Vector2<T> operator-(const Vector2<T>& v2) const { return Vector2(x - v2.x, y - v2.y); }
		inline Vector2<T>& operator-=(const Vector2<T>& v2) { *this = *this - v2; return *this; }

		inline Vector2<T> operator*(const Vector2<T>& v2) const { return Vector2(x * v2.x, y * v2.y); }
		inline Vector2<T>& operator*=(const Vector2<T>& v2) { *this = v2 * (*this); return *this; }

		inline Vector2<T> operator/(const Vector2<T>& v2) const { return Vector2(x / v2.x, y / v2.y); }
		inline Vector2<T>& operator/=(const Vector2<T>& v2) { *this = *this / v2; return *this; }

		// constant multiplication
		inline Vector2<T> operator*(T k) const { return Vector2(static_cast<T>(x * k), static_cast<T>(y * k)); }
		inline Vector2<T>& operator*=(T k) { *this = *this * k; return *this; }
		
		inline Vector2<T> operator/(T k) const { return Vector2(static_cast<T>(x / k), static_cast<T>(y / k)); }
		inline Vector2<T>& operator/=(T k) { *this = *this / k; return *this; }

		inline bool operator==(const Vector2<T>& v2) const { return x == v2.x && y == v2.y; }
		inline bool operator!=(const Vector2<T>& v2) const { return !(*this == v2); }
	};

	typedef Vector2<int> Vector22i;
	typedef Vector2<float> Vector22f;
	typedef Vector2<int> Vec2i;
	typedef Vector2<float> Vec2f;
}

#endif