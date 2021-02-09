
#pragma once
#include "LinkedList.h"
#include "Rect.h"

struct Window {

  // UI layout of window
  struct UIItem *UIroot;

  // Creates empty window
  Window();

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

  // window processing methods
  bool IsActive();
};