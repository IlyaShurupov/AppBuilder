
#pragma once
#include "UI.h"
#include "KeyMap.h"

struct Window {
  // UI layout of window
  List<ScrArea> scrAreas;
  List<vec2<SCR_UINT>> scrVerts;
  List<Edge<SCR_UINT>> scrEdges;

  // Creates empty window 
  Window();

  // Closes the window
  ~Window();

  // Save UI of window
  void OnWrite();

  // Restore saved UI
  void OnRead();

  // Maps user inputs into an executing commands
  void ProcessEvents(class KeyMap* key_map, List<ExecComand>* exec_queue);

  // Draws the UI only!! (Buttons, menus...)
  void Draw();

  // Just shows the buff to the user
  void SendBuffToSystem();

 private:
  // This is where interactin with the system happends
  // Only things we need from the system are Fbuffer & user inputs
  class SystemHandler* SysH;
  UserInputs user_inputs;
};