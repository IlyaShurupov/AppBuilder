
#include "Types.h"
#include "public/Context.h"
#include "public/Logic.h"
#include "public/Win32API.h"

// TODO: keymap & draw system,  CleanUps
int main() {

  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
  if (!SUCCEEDED(CoInitialize(NULL))) {
    return -1;
  }
  SystemHandler SysHdl = SystemHandler(800, 800);
  if (SUCCEEDED(SysHdl.Initialize())) {
    int* aray = new int[21];
    Context C = Context(&SysOutput, &GetEventSate, SysHdl.getFBuff(), &SysHdl);
    AppMainLoop(&C);
  }
  CoUninitialize();

  _CrtDumpMemoryLeaks();
}