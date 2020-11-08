#pragma once

#include "Matrix.h"
#include "Mesh.h"
#include "Property.h"
#include "FrameBuff.h"

struct Camera {
	PropertyInt Width;
	PropertyInt Height;
	PropertyFloat Lens;
};

struct ObjVisibility {
	bool Viewport = true;
	bool Render = true;
};

class Object
{
public:
	Mat3f TransformMat;
	Vec3f Pos;
	Object* Parent;
	List<Object> Childs;

	ObjVisibility Visibility;

public:
	Object();
	~Object();

	void SetStaticMeshComponent(StaticMesh* StaticMesh);
	StaticMesh* GetStaticMeshComponent();
	
	void SetCameraComponent(Camera* CameraComponent);
	Camera* GetCameraComponent();

	void getWorldLocation(Vec3f& out);
	void getWorldTransform(Mat3f& out);

private:
	Camera* CameraComponent;
	StaticMesh* StaticMeshComponent;
};