#pragma once

class vec3f
{
public:
	float x, y, z;

	vec3f(vec3f* vec);
	vec3f(float x, float y, float z);
	vec3f(float array[3]);
	vec3f();
	~vec3f();

	vec3f& operator*=(float val);
	vec3f& operator/=(float val);
	float operator*(vec3f& vec);
	void cross(vec3f* vec, vec3f* vec_out);
	vec3f& operator+=(const vec3f& vec);
	vec3f& operator-=(const vec3f& vec);
	vec3f& operator=(const vec3f& vec);
	void unitv(vec3f* vec);
	void assing(float v[3]);
	void assing(float v1, float v2, float v3);
	void clear();
	void print();
	void invert();
	float mod();
private:
};
