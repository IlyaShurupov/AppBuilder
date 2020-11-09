
#include "RayCast.h"
#include "Types.h"
#include "public/Logic.h"
#include "public/Win32API.h"

void test1() {
  Object NewObj;
  StaticMesh NewMesh;
  NewObj.SetStaticMeshComponent(&NewMesh);

  Vec3f v0 = Vec3f();
  Vec3f v1 = Vec3f(0.f, 0.f, 4.f);
  Vec3f v2 = Vec3f(4.f, 0.f, 0.f);

  Trig NewTrig = Trig(v0, v1, v2);

  NewMesh.Trigs.add(&NewTrig);

  List<Object> Objects;
  Objects.add(&NewObj);

  Ray NewRay;
  NewRay.Dir.y = -1.f;
  NewRay.Pos.z = 1.f;
  NewRay.Pos.x = 1.f;
  NewRay.Pos.y = 1.f;

  NewRay.Cast(&Objects, 1000000.f);
}

void test2() {
  Mat3f mat = Mat3f(Vec3f(1, 2, 3), Vec3f(3, 2, 1), Vec3f());
  print(mat);
  List<Mat3f> matlist;

  matlist.add(&mat);
  print(matlist);

  Stack<List<Mat3f>> stack;
  stack.Add(matlist);
  print(stack);

  Color4 color;
  color.G = 255;
  color.R = 0;
  color.B = 0;

  FBuff fbuff = FBuff(400, 400);
  fbuff.DrawRect(100, 3, color, 100, 200);
  print(fbuff, 1);
}

void test3(int w, int h, FBuff& buff) {
  

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
  camera.Height.setVal(h);
  camera.Width.setVal(w);
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

  print(buff, 1);
}

int main() {
  int h, w;
  h = 400;
  w = 400;
  FBuff buff = FBuff(w, h);
  //test3(w, h, buff);
  //return 0;

  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

  if (SUCCEEDED(CoInitialize(NULL))) {
    {
      DemoApp app;
      if (SUCCEEDED(app.Initialize())) {

        app.RunMessageLoop();
      }
    }
    CoUninitialize();
  }
}