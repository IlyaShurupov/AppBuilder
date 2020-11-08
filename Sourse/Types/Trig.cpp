#include "public/Trig.h"
#include "public/Ray.h"

Trig::Trig() {
	V0.assign(0.f, 0.f, 0.f);
	V1.assign(0.f, 0.f, 0.f);
	V2.assign(0.f, 0.f, 0.f);
}

Trig::Trig(Vec3f& v0, Vec3f& v1, Vec3f& v2) {
	V0 = v0;
	V1 = v1;
	V2 = v2;
}

Trig::~Trig() {
}

void Trig::Normal(Vec3f& dir)
{
	dir = (V1 - V0).Cross(V2 - V0);
	dir.Normalize();
}


bool Trig::RayHit(class Ray* ray, Vec3f& HitPos)
{
	Vec3f edge1, edge2, h, s, q;

	float a, f, u, v;

	edge1 = V1 - V0;
	edge2 = V2 - V0;

	h = ray->Dir.Cross(edge2);
	a = edge1.Dot(h);

	// This ray is parallel to this triangle.
	if (a > -EPSILON && a < EPSILON) {
		return false;
	}

	f = 1.f / a;
	s = ray->Pos - V0;
	u = f * s.Dot(h);

	if (u < 0.f || u > 1.f) {
		return false;
	}

	q = s.Cross(edge1);
	v = f * ray->Dir.Dot(q);

	if (v < 0.f || u + v > 1.f) {
		return false;
	}

	// Now we can compute t to find out where the intersection point is on the line.
	float t = f * edge2.Dot(q);

	// ray intersection
	if (t > EPSILON) {
		HitPos = ray->Pos + (ray->Dir * t);
		return true;
	}
	// There is a line intersection but not a ray intersection.
	else {
		return false;
	}
}