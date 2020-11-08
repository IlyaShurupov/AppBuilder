#include "public/Object.h"
#include "public/Mesh.h"

Object::Object() {
	StaticMeshComponent = nullptr;
	Parent = nullptr;
	CameraComponent = nullptr;
}

Object::~Object() {
}

void Object::SetStaticMeshComponent(StaticMesh* static_mesh) {
	StaticMeshComponent = static_mesh;
}

StaticMesh* Object::GetStaticMeshComponent() {
	return StaticMeshComponent;
}

void Object::SetCameraComponent(Camera* CameraComponent) {
	this->CameraComponent = CameraComponent;
}

Camera* Object::GetCameraComponent()
{
	return CameraComponent;
}

void Object::getWorldLocation(Vec3f& out) {
	out.assign(0.f, 0.f, 0.f);
	Object* parent = Parent;
	while (parent) {
		out += parent->Pos;
		parent = parent->Parent;
	}
}

void Object::getWorldTransform(Mat3f& out) {

}
