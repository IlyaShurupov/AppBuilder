#pragma once

#include "MathMacros.h"



template <typename Type>
class vec2
{
public:

	Type x;
	Type y;

	// Initialization

	vec2(Type x, Type y);
	vec2(Type vec[2]);
	vec2();
	vec2(vec2& vec);
	void assign(Type x, Type y);
	void assign(vec2& vec);
	void assign(Type vec[2]);
	vec2& operator=(vec2& vec);

	// Fundamental create on stack

	vec2 operator+(vec2& vec);
	vec2 operator-(vec2& vec);
	vec2 operator+(Type val);
	vec2 operator-(Type val);
	vec2 operator*(Type val);
	vec2 operator/(Type val);
	// Fundamental write

	void operator -= (Type val);
	void operator += (Type val);
	void operator -= (vec2& vec);
	void operator += (vec2& vec);
	void operator *= (Type val);
	void operator /= (Type val);

	// Vector Properties
	float Dot(vec2& vec);
	float Length();
	float LengthSqured();
	vec2 Dir();
	void Normalize();

	// Vector Transformation

	void Rot(float cosa, float sina);
	~vec2();
};

template <typename Type>
inline vec2<Type>::vec2(Type x, Type y) {
	this->x = x;
	this->y = y;
}

template <typename Type>
inline vec2<Type>::vec2(Type vec[2]) {
	x = vec[0];
	y = vec[1];
}

template <typename Type>
inline vec2<Type>::vec2() {
	x = y  = 0;
}

template <typename Type>
inline vec2<Type>::vec2(vec2& vec) {
	x = vec.x;
	y = vec.y;
}

template <typename Type>
inline void vec2<Type>::operator -= (Type val) {
	x -= val;
	y -= val;
}

template <typename Type>
inline void vec2<Type>::operator += (Type val) {
	x += val;
	y += val;
}

template <typename Type>
inline void vec2<Type>::operator -= (vec2& vec) {
	x -= vec.x;
	y -= vec.y;
}

template <typename Type>
inline void vec2<Type>::operator += (vec2& vec) {
	x += vec.x;
	y += vec.y;
}

template <typename Type>
inline void vec2<Type>::operator *= (Type val) {
	x *= val;
	y *= val;
}

template <typename Type>
inline void vec2<Type>::operator /= (Type val) {
	x /= val;
	y /= val;
}

template <typename Type>
inline void vec2<Type>::assign(Type x, Type y) {
	this->x = x;
	this->y = y;
}

template <typename Type>
inline void vec2<Type>::assign(vec2& vec) {
	x = vec.x;
	y = vec.y;
}

template <typename Type>
inline void vec2<Type>::assign(Type vec[2]) {
	x = vec[0];
	y = vec[1];
}

template <typename Type>
inline vec2<Type>& vec2<Type>::operator=(vec2& vec) {
	x = vec.x;
	y = vec.y;
	return *this;
}

// Fundamental create on stack

template <typename Type>
inline vec2<Type> vec2<Type>::operator+(vec2& vec) {
	return vec2(x + vec.x, y + vec.y);
}

template <typename Type>
inline vec2<Type> vec2<Type>::operator-(vec2& vec) {
	return vec2(x - vec.x, y - vec.y);
}

template <typename Type>
inline vec2<Type> vec2<Type>::operator+(Type val) {
	return vec2(x + val, y + val);
}

template <typename Type>
inline vec2<Type> vec2<Type>::operator-(Type val) {
	return vec2(x - val, y - val);
}

template <typename Type>
inline vec2<Type> vec2<Type>::operator*(Type val) {
	return vec2(x * val, y * val);
}

template <typename Type>
inline vec2<Type> vec2<Type>::operator/(Type val) {
	return vec2(x / val, y / val);
}

// Vector Properties
template <typename Type>
inline float vec2<Type>::Dot(vec2& vec) {
	return (x * vec.x + y * vec.y);
}

template <typename Type>
inline float vec2<Type>::Length() {
	Type sqred = (Type)(x * x + y * y);
	return sqrt(sqred);
}

template <typename Type>
inline float vec2<Type>::LengthSqured() {
	return (x * x + y * y);
}

template <typename Type>
inline vec2<Type> vec2<Type>::Dir() {
	return vec2(*this / (Type)this->Length());
}

template <typename Type>
inline void vec2<Type>::Normalize() {
	*this /= (Type)this->Length();
}

// Vector Transformation
template <typename Type>
inline void vec2<Type>::Rot(float cosa, float sina)
{
	Type tmp = x;
	x = x * cosa - y * sina;
	y = tmp * sina + y * cosa;
}

template <typename Type>
inline vec2<Type>::~vec2() {

}


template <typename Type>
struct RectPtr {
  vec2<Type>* v0 = nullptr;
  vec2<Type>* v1 = nullptr;
  vec2<Type>* v2 = nullptr;
  vec2<Type>* v3 = nullptr;
};

template <typename Type>
struct Rect {
  vec2<Type> v0, v1, v2, v3;
};

template <typename Type>
struct Edge {
  vec2<Type> v1, v2;
};