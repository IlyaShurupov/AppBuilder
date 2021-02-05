#pragma once
#include "Strings.h"
#include "LinkedList.h"

enum class DBType {
  FLOAT,
  INT,
  BOOL,
  STRING,
  COMPOUND,
  TOUPLE,
};

struct DataBlock {
  Str BlockName;
  DBType type = DBType::COMPOUND;

  int integer = 0;
  float floating = 0;
  bool bollean = 0;
  Str string;
  List<DataBlock> list;

  DataBlock* find(Str& BlockName);
};

struct DataBlock* Read_Yaml(struct Str* filepath);