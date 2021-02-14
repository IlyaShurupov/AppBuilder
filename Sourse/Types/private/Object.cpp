#include "Object.h"
#include "Mesh.h"

Object::Object() {
  StaticMeshComponent = nullptr;
  CameraComponent = nullptr;
  RenderSettingsComponent = nullptr;
}

Object::~Object() {
  if (StaticMeshComponent) {
    DEL(StaticMesh, StaticMeshComponent);
  }
  if (CameraComponent) {
    DEL(Camera, CameraComponent);
  }
  if (RenderSettingsComponent) {
    DEL(RenderSettings, RenderSettingsComponent);
  }
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

Camera* Object::GetCameraComponent() {
  return CameraComponent;
}

void Object::SetRenderComponent(RenderSettings* RenderComponent) {
  RenderSettingsComponent = RenderComponent;
}

RenderSettings* Object::GetRenderComponent() {
  return RenderSettingsComponent;
}

void Object::getWorldLocation(Vec3f& out) {
  out.assign(Pos);
  Object* parent = hrchy.prnt;
  while (parent) {
    out += parent->Pos;
    parent = parent->hrchy.prnt;
  }
}

void Object::getWorldTransform(Mat3f& out) {}
