
#pragma once

class UIItem;
struct UInputs;
struct Str;

struct SysHandler {

  SysHandler();
  ~SysHandler();

  void Inputs(UInputs& uinputs);

  void Output(UIItem* UIroot);

  void* PlatformDepended = nullptr;

  void ConsoleToggle();
  void SetIcon(Str& stricon);
  bool Active();
};