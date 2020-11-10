#pragma once
#include "public/Context.h"
#include "public/UI.h"
#include "public/Operator.h"

AppWindow::AppWindow(FBuff* SysBuff) {
  fbuff = SysBuff;
  // this->SysOutput = SysOutput;
}

AppWindow::~AppWindow() {}

void AppWindow::Draw(class Context* C) {
  FOREACH_NODE(ScrArea, (&scrAreas), area_node) {
    Editor* edt = area_node->Data->editor;
    
    area_node->Data->rect.v0->assign(0, 0);
    area_node->Data->rect.v1->assign(0, fbuff->height);
    area_node->Data->rect.v2->assign(fbuff->width, fbuff->height);
    area_node->Data->rect.v3->assign(fbuff->width, 0);

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

  RectPtr<SCR_UINT> rect;
  vec2<SCR_UINT>* v0 = DBG_NEW vec2<SCR_UINT>(0, 0);
  vec2<SCR_UINT>* v1 = DBG_NEW vec2<SCR_UINT>(0, 800);
  vec2<SCR_UINT>* v2 = DBG_NEW vec2<SCR_UINT>(800, 800);
  vec2<SCR_UINT>* v3 = DBG_NEW vec2<SCR_UINT>(800, 0);
  rect.v0 = v0;
  rect.v1 = v1;
  rect.v2 = v2;
  rect.v3 = v3;

  ScrArea* area = DBG_NEW ScrArea(editors[0], rect);

  window.scrAreas.add(area);


  Object& NewMeshObj = *(DBG_NEW Object());
  StaticMesh& NewMesh = *(DBG_NEW StaticMesh());
  Trig& NewTrig = *DBG_NEW Trig(Vec3f(), Vec3f(0.f, 0.f, 4.f), Vec3f(4.f, 0.f, 0.f));
  NewMesh.Trigs.add(&NewTrig);
  NewMeshObj.SetStaticMeshComponent(&NewMesh);

  Object& NewCamObj = *DBG_NEW Object();
  NewCamObj.Pos.y += 8;
  // NewCamObj.Pos.z += 1;
  Camera& camera = *DBG_NEW Camera();
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