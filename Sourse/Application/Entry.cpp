
#include "Types.h"
#include <iostream>

int main() {
	Vec3f v1;
	Vec3f v2;

	v1.assign(1.f, 2.f, 0.f);
	v2.assign(0.f, 2.f, 1.f);

	Vec3f v3 = v1 + v2;
	std::cout << v3.x << v3.y << v3.z;
}