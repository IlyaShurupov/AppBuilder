#include "public/Object.h"
#include "public/Mesh.h"

Object::Object() {
  StaticMeshComponent = nullptr;
  CameraComponent = nullptr;
  RenderSettingsComponent = nullptr;
}

Object::~Object() {
  if (StaticMeshComponent) delete StaticMeshComponent;
  if (CameraComponent) delete CameraComponent;
  if (RenderSettingsComponent) delete RenderSettingsComponent;
}

void Object::SetStaticMeshComponent(StaticMesh *static_mesh) {
  StaticMeshComponent = static_mesh;
}

StaticMesh *Object::GetStaticMeshComponent() { return StaticMeshComponent; }

void Object::SetCameraComponent(Camera *CameraComponent) {
  this->CameraComponent = CameraComponent;
}

Camera *Object::GetCameraComponent() { return CameraComponent; }

void Object::SetRenderComponent(RenderSettings* RenderComponent) {
  RenderSettingsComponent = RenderComponent;
}

RenderSettings* Object::GetRenderComponent() {
  return RenderSettingsComponent;
}

void Object::getWorldLocation(Vec3f &out) {
  out.assign(Pos);
  Object *parent = hrchy.parent;
  while (parent) {
    out += parent->Pos;
    parent = parent->hrchy.parent;
  }
}

void Object::getWorldTransform(Mat3f &out) {}
