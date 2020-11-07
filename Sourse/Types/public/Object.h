#pragma once

#include "Matrix.h"
#include "Mesh.h"

class Object
{
public:
	Mat3f TransformMat;
	Vec3f Pos;
	Object* Parent;
	List<Object> Childs;

public:
	Object();
	~Object();

	void SetStaticMeshComponent(StaticMesh* StaticMesh);
	StaticMesh* GetStaticMeshComponent();
	
private:

	StaticMesh* StaticMeshComponent;
};