#include "public\FileReader.h"
#include "LinkedList.h"
#include "Strings.h"
#include "Mem.h"

#include <sstream>
#include <fstream>
#include <cassert>

enum class DBType {
  INT, 
  FLOAT, 
  BOOL, 
  STRING,
  BLOCKLIST,
};

struct DataBlock {
  Str BlockName;
  DBType type = DBType::INT;

  int integer = 0;
  float floating =0;
  bool bollean = 0;
  Str string;
  List<DataBlock> list;
};

void file_to_str(Str* filepath, Str& out) {

  std::ifstream file(std::string(filepath->str, filepath->len()));
  std::stringstream buffer;
  buffer << file.rdbuf();
  Str string(buffer.str().c_str());
  out = string;
  file.close();  
}

void write_val_yml(DataBlock* newdb, DBType type, Range& valrange) {

}

DBType dblock_type_yml(Str* str, char coloum, Range* in, Range& out) {
  return DBType::BOOL;
}

bool find_dblock_yml(Str* str, char coloum, Range* in, Range& out) {
  return false;
}

void read_dblock_yml(DataBlock* prnt, Str* str, char coloum, Range& in) {

  Range findrange(in);
  Range dbrange;

  while (find_dblock_yml(str, coloum, &findrange, dbrange)) {

    DataBlock* newdb = NEW_DBG(DataBlock) DataBlock();
    prnt->list.add(newdb);

    Range valrange;
    DBType dbtype = dblock_type_yml(str, coloum, &dbrange, valrange);

    if (dbtype == DBType::BLOCKLIST) {
      read_dblock_yml(newdb, str, coloum + 1, valrange);
    } else {
      write_val_yml(newdb, dbtype, valrange);
    }
  }
}

DataBlock* Read_Yaml(Str* filepath) {
  Str file;
  file_to_str(filepath, file);

  DataBlock* dblock = NEW_DBG(DataBlock) DataBlock();
  read_dblock_yml(dblock, &file, 0, Range(0, file.length));
  return dblock;
}
