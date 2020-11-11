#pragma once
#include "public/Context.h"

Seance::Seance(/*project file*/) {

  if (/*file specified*/ false) {

    // load project
    OnRead(/*file path*/);
  } else {
    // Create dummy

    /*
    OpsInit(C);
    UI_Init(C);

    RectPtr<SCR_UINT> rect;
    vec2<SCR_UINT>* v0 = DBG_NEW vec2<SCR_UINT>(0, 0);
    vec2<SCR_UINT>* v1 = DBG_NEW vec2<SCR_UINT>(0, 800);
    vec2<SCR_UINT>* v2 = DBG_NEW vec2<SCR_UINT>(800, 800);
    vec2<SCR_UINT>* v3 = DBG_NEW vec2<SCR_UINT>(800, 0);
    rect.v0 = v0;
    rect.v1 = v1;
    rect.v2 = v2;
    rect.v3 = v3;

    ScrArea* area = DBG_NEW ScrArea(C->editors[0], rect);

    C->windows[0]->scrAreas.add(area);

    Object& NewMeshObj = *(DBG_NEW Object());
    StaticMesh& NewMesh = *(DBG_NEW StaticMesh());
    Trig& NewTrig =
        *DBG_NEW Trig(Vec3f(), Vec3f(0.f, 0.f, 4.f), Vec3f(4.f, 0.f, 0.f));
    NewMesh.Trigs.add(&NewTrig);
    NewMeshObj.SetStaticMeshComponent(&NewMesh);

    Object& NewCamObj = *DBG_NEW Object();
    NewCamObj.Pos.y += 8;
    // NewCamObj.Pos.z += 1;
    Camera& camera = *DBG_NEW Camera();
    camera.Lens.setVal(1);
    NewCamObj.SetCameraComponent(&camera);
    NewCamObj.TransformMat.I.assign(-1, 0, 0);
    NewCamObj.TransformMat.J.assign(0, 0, 1);
    NewCamObj.TransformMat.K.assign(0, 1, 0);
    C->scenes[0]->collection.add(&NewCamObj);
    C->scenes[0]->collection.add(&NewMeshObj);

    C->scenes[0]->RndrSets.setObjList(&C->scenes[0]->collection);
    C->scenes[0]->RndrSets.setCamera(&NewCamObj);
    C->scenes[0]->RndrSets.Resolution.setVal(1.f);
    */
  }
}

Seance::~Seance() {}

void Seance::OnWrite(/*file path*/) {}

void Seance::OnRead(/*file path*/) {}