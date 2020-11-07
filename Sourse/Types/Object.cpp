#include "public/Object.h"
#include "public/Mesh.h"

Object::Object() {
	StaticMeshComponent = nullptr;
	Parent = nullptr;
}

Object::~Object() {
}

void Object::SetStaticMeshComponent(StaticMesh* static_mesh) {
	StaticMeshComponent = static_mesh;
}

StaticMesh* Object::GetStaticMeshComponent()
{
	return StaticMeshComponent;
}
