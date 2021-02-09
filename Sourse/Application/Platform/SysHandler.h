
#pragma once

#include "LinkedList.h"
#include "Vec2.h"

struct Window;
struct UInputs;
struct Str;

struct SysHandler {

  SysHandler();
  ~SysHandler();

  vec2<SCR_UINT> scrsize;

  void UserInputs(UInputs& uinputs);

  void Output(List<Window>& windows);

  void* PlatformDepended = nullptr;

  void ConsoleToggle();
  void SetIcon(Str& stricon);
};