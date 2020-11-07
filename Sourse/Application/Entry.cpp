
#include "Types.h"
#include <iostream>

int main() {
	Object NewObj;
	StaticMesh NewMesh;
	NewObj.SetStaticMeshComponent(&NewMesh);

	Vec3f v0 = Vec3f();
	Vec3f v1 = Vec3f(0.f, 0.f, 4.f);
	Vec3f v2 = Vec3f(4.f, 0.f, 0.f);

	Trig NewTrig = Trig(v0, v1, v2);

	NewMesh.Trigs.add(NewTrig);

	List<Object> Objects;
	Objects.add(NewObj);

	Ray NewRay;
	NewRay.Dir.y = -1.f;
	NewRay.Pos.z = 1.f;
	NewRay.Pos.x = 1.f;
	NewRay.Pos.y = 1.f;

	NewRay.Cast(&Objects, 1000000.f);

	std::cout << NewRay.HitData.HitPos.x << NewRay.HitData.HitPos.y << NewRay.HitData.HitPos.z;
}