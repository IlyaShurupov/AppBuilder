
#pragma once
#include "LinkedList.h"
#include "Rect.h"

struct Window {

  vec2<SCR_UINT> scr_size;

  // UI layout of window
  struct UIItem *UIroot;
  struct UInputs *user_inputs;
  struct KeyMap* keymap;
  // Creates empty window
  Window(struct Str* configfolder, List<struct Operator>* operators);

  // Closes the window
  ~Window();

  // Save UI of window
  void OnWrite();

  // Restore saved UI
  void OnRead();

  // Maps user inputs into an executing commands
  void ProcessEvents(List<struct OpThread>* exec_queue, struct Seance* C);

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
  struct SystemHandler* SysH;
};