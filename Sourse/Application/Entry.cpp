
//#include "RayCast.h"
#include "Types.h"
#include "public/Logic.h"
#include "public/Win32API.h"
#include "public/Context.h"

void test3(FBuff& buff) {
  
  List<Object> Objects;

  Object NewMeshObj;
  StaticMesh NewMesh;
  Trig NewTrig = Trig(Vec3f(), Vec3f(0.f, 0.f, 4.f), Vec3f(4.f, 0.f, 0.f));
  NewMesh.Trigs.add(&NewTrig);
  NewMeshObj.SetStaticMeshComponent(&NewMesh);

  Object NewCamObj;
  NewCamObj.Pos.y += 8;
  // NewCamObj.Pos.z += 1;
  Camera camera;
  camera.Height.setVal(buff.height);
  camera.Width.setVal(buff.width);
  camera.Lens.setVal(1);
  NewCamObj.SetCameraComponent(&camera);
  NewCamObj.TransformMat.I.assign(-1, 0, 0);
  NewCamObj.TransformMat.J.assign(0, 0, 1);
  NewCamObj.TransformMat.K.assign(0, 1, 0);
  Objects.add(&NewCamObj);
  Objects.add(&NewMeshObj);

  RayCast::RenderSettings RND_set;
  RND_set.setObjList(&Objects);
  RND_set.setCamera(&NewCamObj);
  RND_set.Resolution.setVal(0.5f);
  
  RayCast::RenderToBuff(&RND_set, &buff);

}

int main() {

  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

  if (SUCCEEDED(CoInitialize(NULL))) {
    {
      SystemHandler SysHdl = SystemHandler(800, 800);
      if (SUCCEEDED(SysHdl.Initialize())) {

        Context C =
            Context(&SysOutput, &GetEventSate, SysHdl.getFBuff(), &SysHdl);
        AppMainLoop(&C);
      }
    }
    CoUninitialize();
  }
}