
#pragma once
#include "KeyMap.h"
#include "UILayout.h"
#include "Rect.h"
#include "Timer.h"

struct Window {

  vec2<SCR_UINT> scr_size;

  // UI layout of window
  UIItem *UIroot;
  UserInputs user_inputs;

  // Creates empty window
  Window(std::string* configfolder, List<Operator>* operators);

  // Closes the window
  ~Window();

  // Save UI of window
  void OnWrite();

  // Restore saved UI
  void OnRead();

  // Maps user inputs into an executing commands
  void ProcessEvents(List<struct OpThread>* exec_queue);

  // Draws the UI only!! (Buttons, menus...)
  void Draw();

  // Just shows the buff to the user
  void SendBuffToSystem();

  // window processing methods
  bool IsActive();

  // toggles window conole
  void ToggleConsole();

  void getRect(Rect<SCR_UINT>& rect);

  void setRect(Rect<SCR_UINT>& rect);


 private:
  // This is where interactin with the system happends
  // Only things we need from the system are Fbuffer & user inputs
  class SystemHandler* SysH;
  CompiledKeyMap compiled_key_map;
};