#pragma once

#include "..\objects\DT_Object.h"

class Ray
{
public:
	Ray();
	~Ray();
	void shoot(_DA_MeshObjects* DA_MeshObjects);

	vec3f Dir;
	vec3f Pos;

	bool intersect;
	_DAI_Object* obj;
	vec3f intersect_p;
	vec3f Norm;

private:
};

