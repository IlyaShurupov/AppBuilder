#pragma once

#include "Object.h"

struct RayHitData {
	bool Hit;
	Object* Obj;
	Trig* trig;
	Vec3f HitPos;
};

class Ray
{
public:
	Vec3f Dir;
	Vec3f Pos;

	RayHitData HitData;

public:
	Ray();
	~Ray();
	void Cast(List<Object>* objects, float ray_length);
};