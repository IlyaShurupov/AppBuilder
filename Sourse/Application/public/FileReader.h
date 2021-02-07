#pragma once
#include "Strings.h"
#include "LinkedList.h"

enum class DBType {
  FLOAT,
  INT,
  BOOL,
  STRING,
  COMPOUND,
  ARRAY,
};

struct DataBlock {
  Str BlockName;
  DBType type = DBType::COMPOUND;

  int integer = 0;
  float floating = 0;
  bool boolean = 0;
  Str string;
  List<DataBlock> list;

  DataBlock* find(char* BlockName);
  ~DataBlock();
};

struct DataBlock* Read_Yaml(struct Str* filepath);