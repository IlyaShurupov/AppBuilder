#include "public\FileReader.h"
#include "Mem.h"

#include <cassert>
#include <fstream>
#include <sstream>

DataBlock* DataBlock::find(char* BlockName) {
  Str name(BlockName);
  DO_FOREACH_IF(return iter, DataBlock, list, iter->BlockName == name);
  return nullptr;
}

DataBlock::~DataBlock() {
  list.del();
}

void file_to_str(Str* filepath, Str& out) {

  std::ifstream file(std::string(filepath->str, filepath->len()));
  std::stringstream buffer;
  buffer << file.rdbuf();
  Str string(buffer.str().c_str());
  out = string;
  file.close();
}

int str_tab_size(Str* str, str_idx start) {
  int size = 0;
  while (str->str[start + size] == ' ') {
    size++;
  }
  return size;
}

void exclude_spaces(Str* str, Range* rng) {
  for (str_idx i = rng->strt; i < rng->end; i++) {
    if (str->str[i] == ' ') {
      rng->strt++;
      continue;
    }
    break;
  }
  for (str_idx i = rng->end; i > rng->strt; i--) {
    if (str->str[i] == ' ') {
      rng->end--;
      continue;
    }
    break;
  }
}

void touple_from_str(Str* str, Range& rng, int tpl[2]) {
  Range tplrng[2];
  tplrng[0].strt++;
  tplrng[1].strt--;
  tplrng[0].end = tplrng[1].strt = str->find(',', tplrng[0]);

  for (char i = 0; i < 2; i++) {
    exclude_spaces(str, &tplrng[i]);
    Str integer;
    integer.coppy(*str, tplrng[i]);
    tpl[0] = std::stoi(std::string(integer.str));
  }
}

void write_val_yml(DataBlock* db, Str* str, DBType type, Range rng) {

  str_idx assign = str->find(':', rng);
  db->BlockName.coppy(*str, Range(rng.strt, assign));
  rng.strt = assign;

  exclude_spaces(str, &rng);

  switch (db->type) {

    case DBType::STRING: {
      db->string.coppy(*str, rng);
      return;
    }

    case DBType::BOOL: {
      db->boolean = str->str[rng.strt] == 't' ? 1 : 0;
      return;
    }

    case DBType::FLOAT: {
      Str flt;
      flt.coppy(*str, rng);
      db->floating = std::stof(std::string(flt.str));
      return;
    }

    case DBType::INT: {
      Str integer;
      integer.coppy(*str, rng);
      db->integer = std::stoi(std::string(integer.str));
      return;
    }

    case DBType::TOUPLE: {
      int intgr[2]; 
      touple_from_str(str, rng, intgr);
      db->list.add(NEW_DBG(DataBlock) DataBlock());
      db->list.last().Data->integer = intgr[0];
      db->list.add(NEW_DBG(DataBlock) DataBlock());
      db->list.last().Data->integer = intgr[1];
    }
  }
}

DBType dblock_type_yml(Str* str, Range* in) {

  exclude_spaces(str, in);

  switch (str->str[in->strt]) {
    case ':':
    case '-':
      return DBType::COMPOUND;

    case 'e':
    case 's':
      return DBType::BOOL;

    case '"':
      return DBType::STRING;

    case ']':
      return DBType::TOUPLE;

    default:
      for (str_idx i = in->strt; i < in->end; i++) {
        if (str->str[i] == '.') {
          return DBType::FLOAT;
        }
      }
      return DBType::INT;
  }

  return DBType::BOOL;
}

bool find_dblock_yml(Str* str, Range in, Range& out) {

  out = in;
  bool keep = true;

  while (keep) {

    out.end = str->find('\n', in);
    if (out.end == -1) {
      return false;
    }

    if (out.end == out.strt) {
      out.strt = out.end + 1;
      in.strt = out.end + 1;
      continue;

    } else {

      bool nonempthy = false;
      for (str_idx i = out.strt; i < out.end; i++) {
        if (str->str[i] != ' ') {
          nonempthy = true;
          break;
        }
      }

      if (!nonempthy) {
        continue;
      }
    }
    keep = false;
  }
  
  return true;
}

int read_dblock_yml(DataBlock* prnt, Str* str, char coloum, Range& in) {

  Range dbrange;
  bool keep = true;
  int clm;

  while (find_dblock_yml(str, in, dbrange)) {

    in.strt = dbrange.end;

    clm = str_tab_size(str, dbrange.strt);

    if (clm < coloum) {
      return clm;
    } 

    DataBlock* newdb = NEW_DBG(DataBlock) DataBlock();
    DBType dbtype = dblock_type_yml(str, &dbrange);

    prnt->list.add(newdb);

    if (dbtype == DBType::COMPOUND) {
      if (read_dblock_yml(newdb, str, clm, in) < clm) {
        return clm;
      }
    } else {
      write_val_yml(newdb, str, dbtype, dbrange);
    }

  }

  return 0;
}

DataBlock* Read_Yaml(Str* filepath) {
  Str file;
  file_to_str(filepath, file);

  DataBlock* dblock = NEW_DBG(DataBlock) DataBlock();
  read_dblock_yml(dblock, &file, -1, Range(0, file.length));
  return dblock;
}
