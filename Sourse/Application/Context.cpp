#pragma once
#include "public/Context.h"
#include "public/UI.h"
#include "public/Operator.h"

AppEvent::AppEvent() {
  for (int i = 0; i < KeyNum; i++) {
    (&A)[i] = KeyState::EVENT_NONE;
  }
}

AppEvent::~AppEvent() {}

bool AppEvent::IsEvent() { return true; }

AppWindow::AppWindow(FBuff* SysBuff) {
  fbuff = SysBuff;
  // this->SysOutput = SysOutput;
}

AppWindow::~AppWindow() {}

void AppWindow::Draw(class Context* C) {
  FOREACH_NODE(ScrArea, (&scrAreas), area_node) {
    Editor* edt = area_node->Data->editor;
    edt->Draw(C, &area_node->Data->rect, fbuff);
  }
}

Context::Context(void (*SysOutput)(SystemHandler* SysH),
                 void (*Inputs)(SystemHandler* SysH, AppEvent* Event),
                 FBuff* SysBuff, SystemHandler* SysH)
    : window(SysBuff) {
  this->SysH = SysH;
  this->SysOutput = SysOutput;
  this->SysInput = Inputs;
  
  OpsInit(this);
  UI_Init(this);

  RectPtr<short> rect;
  vec2<short>* v0 = new vec2<short>(0, 0);
  vec2<short>* v1 = new vec2<short>(0, 800);
  vec2<short>* v2 = new vec2<short>(800, 800);
  vec2<short>* v3 = new vec2<short>(800, 0);

  ScrArea* area = new ScrArea(editors[0], rect);
  window.scrAreas.add(area);


  Object& NewMeshObj = *(new Object());
  StaticMesh& NewMesh = *(new StaticMesh());
  Trig& NewTrig = *new Trig(Vec3f(), Vec3f(0.f, 0.f, 4.f), Vec3f(4.f, 0.f, 0.f));
  NewMesh.Trigs.add(&NewTrig);
  NewMeshObj.SetStaticMeshComponent(&NewMesh);

  Object& NewCamObj = *new Object();
  NewCamObj.Pos.y += 8;
  // NewCamObj.Pos.z += 1;
  Camera& camera = *new Camera();
  camera.Height.setVal(SysBuff->height);
  camera.Width.setVal(SysBuff->width);
  camera.Lens.setVal(1);
  NewCamObj.SetCameraComponent(&camera);
  NewCamObj.TransformMat.I.assign(-1, 0, 0);
  NewCamObj.TransformMat.J.assign(0, 0, 1);
  NewCamObj.TransformMat.K.assign(0, 1, 0);
  collection.add(&NewCamObj);
  collection.add(&NewMeshObj);

  RndrSets.setObjList(&collection);
  RndrSets.setCamera(&NewCamObj);
  RndrSets.Resolution.setVal(1.f);
}

Context::~Context() {}