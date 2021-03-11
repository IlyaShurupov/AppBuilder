#include "Object/Object.h"
#include "Memory/Mem.h"

Object::Object() {
  StaticMeshComponent = nullptr;
  CameraComponent = nullptr;
  RenderSettingsComponent = nullptr;
}


Object::~Object() {
  if (StaticMeshComponent) {
    DEL(Mesh, StaticMeshComponent);
  }
  if (CameraComponent) {
    DEL(Camera, CameraComponent);
  }
  if (RenderSettingsComponent) {
    DEL(RenderSettings, RenderSettingsComponent);
  }
}

void Object::SetStaticMeshComponent(Mesh* static_mesh) {
  StaticMeshComponent = static_mesh;
}

Mesh* Object::GetStaticMeshComponent() {
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

RenderSettings::RenderSettings() {
  ObjList = nullptr;
  Camera = nullptr;
  Resolution.assign(0.5f, 0.1f, 1.f);
}

RenderSettings::~RenderSettings() {}

void RenderSettings::setObjList(List<Object>* list) {
  ObjList = list;
}

void RenderSettings::setCamera(Object* camera) {
  Camera = camera;
}

Object* RenderSettings::getCamera() {
  return Camera;
}

List<Object>* RenderSettings::getObjList() {
  return ObjList;
}