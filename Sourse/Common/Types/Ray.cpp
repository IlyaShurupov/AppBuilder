#include "ray.h"
#include "Math.h"




float trig_area(vec3f* p1, vec3f* p2, vec3f* p3) {

	vec3f v1 = vec3f(p2);
	vec3f v2 = vec3f(p3);

	v1 -= *p1;
	v2 -= *p1;

	vec3f cross_prd;
	v1.cross(&v2, &cross_prd);
	return cross_prd.mod() / 2.f;
}

Ray::Ray()
{
	intersect = false;
	obj = nullptr;
}

Ray::~Ray()
{
}


void apply_transformation(vec3f *vec, vec3f* iv, vec3f* jv, vec3f* kv, vec3f *pos) {

	vec3f i = vec3f(iv);
	vec3f j = vec3f(jv);
	vec3f k = vec3f(kv);

	i *= vec->x;
	j *= vec->y;
	k *= vec->z;

	*vec = (k += (i += j));

	*vec += *pos;
}

void norm(vec3f* v1, vec3f* v2, vec3f* v3, vec3f* out)
{
	vec3f vec1 = vec3f(v2);
	vec3f vec2 = vec3f(v3);

	vec1 -= v1;
	vec2 -= v1;

	vec1.cross(&vec2, out);
	*out /= out->mod();
}

void Ray::shoot(_DA_MeshObjects* DA_MeshObjects) {

	float magnitude = INF;
	vec3f trig_norm = vec3f();
	vec3f intersect_point = vec3f(Dir);
	_DAI_Object* Obj = nullptr;

	int Objlen = DA_MeshObjects->Obj_GetLength();
	for (int obj_idx = 0; obj_idx < Objlen; obj_idx++) {
		_DAI_Object* tmp_Obj = DA_MeshObjects->Obj_GetItem(obj_idx);
		int triglen = tmp_Obj->Mesh.DA_Trigs->DA_Trigs_GetLength();
		for (int trig_idx = 0; trig_idx < triglen; trig_idx++) {
			_DAI_Trig* trig = tmp_Obj->Mesh.DA_Trigs->DAI_Trig_GetItem(trig_idx);
			
			vec3f v1 = vec3f(trig->v1->vec);
			vec3f v2 = vec3f(trig->v2->vec);
			vec3f v3 = vec3f(trig->v3->vec);

			apply_transformation(&v1, &tmp_Obj->unit_vx, &tmp_Obj->unit_vy, &tmp_Obj->unit_vz, &tmp_Obj->Position);
			apply_transformation(&v2, &tmp_Obj->unit_vx, &tmp_Obj->unit_vy, &tmp_Obj->unit_vz, &tmp_Obj->Position);
			apply_transformation(&v3, &tmp_Obj->unit_vx, &tmp_Obj->unit_vy, &tmp_Obj->unit_vz, &tmp_Obj->Position);
		
			vec3f tmp_trig_norm = vec3f();
			norm(&v1, &v2, &v3, &tmp_trig_norm);


			if (tmp_trig_norm * Dir >= 0) {
				tmp_trig_norm.invert();
			}

			vec3f tmp = vec3f(Pos);
			tmp -= v1;

			float tmp_magnitude = (tmp_trig_norm * tmp) / -(tmp_trig_norm * Dir);

			vec3f tmp_intrsect_p = vec3f(Dir);
			tmp_intrsect_p *= tmp_magnitude;
			tmp_intrsect_p += Pos;

			if (trig_area(&v1, &v2, &v3) >=
				trig_area(&tmp_intrsect_p, &v2, &v3) +
				trig_area(&v1, &tmp_intrsect_p, &v3) +
				trig_area(&v1, &v2, &tmp_intrsect_p) - 0.00001f) {

				if (magnitude > tmp_magnitude && tmp_magnitude > 0)
				{
					Obj = tmp_Obj;
					trig_norm = tmp_trig_norm;
					intersect_point = tmp_intrsect_p;
					magnitude = tmp_magnitude;
				}
			}	
		}
	}

	if (magnitude == INF) {
	    intersect = false;
		return;
	}

	intersect = true;
	obj = Obj;
	intersect_p = intersect_point;
	Norm = trig_norm;
}
