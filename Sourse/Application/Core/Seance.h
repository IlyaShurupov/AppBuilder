#pragma once

#include "LinkedList.h"

class Object;
struct Window;
struct Operator;
struct Str;
struct OpThread;

struct Project {
  // MeshObjects, Cameras, Renders...
  List<Object> collection;

  // UI layout
  List<Window> windows;

  // save project
  void OnWrite();

  // load project
  void OnRead();

  Window* C_actWin();
  ~Project();
};

struct Prefferences {
  //UserKeyMap key_map;
  List<Operator> operators;
  //List<Editor> editors;

  // save prefs
  void OnWrite();

  // load prefs
  void OnRead();

  ~Prefferences();
};

struct Seance {
  Seance(Str* basePath);
  ~Seance();

  Prefferences prefferences;

  // Application project
  Project project;

  // All execute commands from UI (pointers to predefined operators)
  List<OpThread> op_threads;

  // Save current context
  void OnWrite(/*file path*/);

  // load context
  void OnRead(/*file path*/);
};