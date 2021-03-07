#pragma once

#include "Geometry/Matrix.h"
#include "Geometry/Mesh.h"
#include "Property.h"
#include "Hierarchy.h"

struct Camera {
	PropertyInt Width;
	PropertyInt Height;
	PropertyFloat Lens;
};

class RenderSettings {
	public:
	PropertyFloat Resolution;

	public:
	RenderSettings();
	~RenderSettings();
	void setObjList(List<class Object>* list);
	void setCamera(class Object* camera);
	class Object* getCamera();
	List<Object>* getObjList();

	private:
	List<class Object>* ObjList;
	class Object* Camera;
};

struct ObjVisibility {
	bool Viewport = true;
	bool Render = true;
};

class Object
{
public:
  Hierarchy<Object, List<Object>, 0> hrchy;

	Mat3f TransformMat;
	Vec3f Pos;

	ObjVisibility Visibility;

public:
	Object();
	~Object();

	void SetStaticMeshComponent(Mesh* StaticMesh);
	Mesh* GetStaticMeshComponent();
	
	void SetCameraComponent(Camera* CameraComponent);
	Camera* GetCameraComponent();

	void SetRenderComponent(RenderSettings* renderComponent);
	RenderSettings* GetRenderComponent();

	void getWorldLocation(Vec3f& out);
	void getWorldTransform(Mat3f& out);

private:
	Camera* CameraComponent;
	Mesh* StaticMeshComponent;
	RenderSettings* RenderSettingsComponent;
};