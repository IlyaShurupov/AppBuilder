#include "vec3f.h"
#include "MAth.h"

vec3f& vec3f::operator*=(float val)
{
	x *= val;
	y *= val;
	z *= val;
	return *this;
}

vec3f& vec3f::operator/=(float val)
{
	x /= val;
	y /= val;
	z /= val;
	return *this;
}

vec3f& vec3f::operator+=(const vec3f& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

vec3f& vec3f::operator-=(const vec3f& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

float vec3f::operator*(vec3f& vec) {
	return MATH_DotProduct(&(this->x), &(vec.x));
}

void vec3f::cross(vec3f* vec, vec3f* vec_out) {
	MATH_CrossProduct(&(vec_out->x), &(this->x), &(vec->x));
}

vec3f& vec3f::operator=(const vec3f& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

float vec3f::mod() {
	return MATH_VectorMagnetude(&(this->x));
}

void vec3f::unitv(vec3f* vec) {
	*vec = *this;
	*vec /= vec->mod();
}

void vec3f::assing(float v[3])
{
	x = v[0];
	y = v[1];
	z = v[2];
}

void vec3f::assing(float v1, float v2, float v3)
{
	x = v1;
	y = v2;
	z = v3;
}

void vec3f::clear()
{
	x = y = z = 0;
}

void vec3f::print()
{
	printf("floats: %9.9f %9.9f %9.9f \n", x, y, z);
}

void vec3f::invert()
{
	x = -x;
	y = -y;
	z = -z;
}

vec3f::vec3f(float x_val, float y_val, float z_val)
{
	x = x_val;
	y = y_val;
	z = z_val;
}

vec3f::vec3f(float array[3])
{
	x = array[0];
	y = array[1];
	z = array[2];
}
vec3f::vec3f()
{
	x = y = z = 0;
}

vec3f::vec3f(vec3f* vec)
{
	x = vec->x;
	y = vec->y;
	z = vec->z;
}

vec3f::~vec3f()
{
}
