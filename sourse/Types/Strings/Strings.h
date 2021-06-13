#pragma once

typedef long int str_idx;

struct Range {
  str_idx strt;
  str_idx end;

  str_idx len();
  bool valid();
  Range();
  Range(const Range& rng);
  Range(str_idx strt, str_idx end);
};

struct StrRef {
  struct Str *str;
  Range rng;
};

struct Str {

  Str(const char* str);
  Str();

  char* str = nullptr;
  str_idx length = 0;

  str_idx len();
  void alloc(str_idx len);
  void clear();

  void operator=(const Str& string);
  void operator=(const char* string);

  Str& operator+=(const Str& string);

  bool operator==(char* cstring);
  bool operator==(const Str& string);
  bool operator!=(char* cstring);
  bool operator!=(const Str& string);

  char operator[](str_idx idx);

  bool match(Range& range, Str& str2, Range& range2);
  
  void coppy(Str* str, Range range);

  str_idx find(Str& string, Range range);
  str_idx find(const char character, Range range);
  str_idx rfind(const char character, Range range);

  void trim(Range range);

  ~Str();
};