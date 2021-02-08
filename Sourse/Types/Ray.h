#pragma once

#include "Object.h"

struct RayHitData {
	bool Hit = false;
	Object* Obj = nullptr;
	Trig* trig = nullptr;
	Vec3f HitPos;
};

class Ray
{
public:
	Vec3f Dir;
	Vec3f Pos;

	RayHitData HitData;

public:
	Ray(Vec3f& Dir, Vec3f& Pos);
	Ray();
	~Ray();
	void Cast(List<Object>* objects, float ray_length);
};