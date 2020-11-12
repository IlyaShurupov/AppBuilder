
#pragma once
#include "UILayout.h"
#include "KeyMap.h"

struct Window {
  // UI layout of window
  List<UIItem> areas;

  // Creates empty window 
  Window(UserKeyMap* usr_key_map, List<Operator>* operators);

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

 private:
  // This is where interactin with the system happends
  // Only things we need from the system are Fbuffer & user inputs
  class SystemHandler* SysH;
  CompiledKeyMap compiled_key_map;
  UserInputs user_inputs;
};