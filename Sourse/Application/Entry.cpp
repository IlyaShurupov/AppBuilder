
#include "Types.h"
#include <iostream>


void test1() {
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
}

void test2() {
	Mat3f mat = Mat3f(Vec3f(1, 2, 3), Vec3f(3, 2, 1), Vec3f());
	print(mat);
	List<Mat3f> matlist;

	matlist.add(mat);
	print(matlist);

	Stack<List<Mat3f>> stack;
	stack.Add(matlist);
	print(stack);

	Color4 color;
	color.G = 255;
	color.R = 0;
	color.B = 0;

	FBuff fbuff = FBuff(400, 400);
	fbuff.DrawRect(100, 100, color, 100, 200);
	print(fbuff, 1);
}

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

	print(NewRay.HitData.HitPos - NewRay.Pos);

 
}