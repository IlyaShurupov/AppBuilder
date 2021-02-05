#pragma once

typedef long long int str_idx;

struct Range {
  str_idx strt = -1;
  str_idx end = -1;

  str_idx len();
  bool valid();
  Range();
  Range(Range& rng);
  Range(str_idx strt, str_idx end);
};

struct Str {

  char* str = nullptr;
  str_idx length = 0;

  Str(const char* str);

  Str();
  ~Str();

  str_idx len();

  void alloc(str_idx len);

  void clear();

  void operator=(Str& string);

  void operator+=(const Str& string);

  void operator=(const char* string);

  bool operator==(char* cstring);

  bool operator==(Str& string);

  char operator[](str_idx idx);

  bool match(Range& range, Str& str2, Range& range2);

  void coppy(Str& str, Range& range);

  str_idx find(Str& string, Range& range);

  str_idx find(const char character, Range& range);

  str_idx rfind(const char character, Range& range);

  void trim(Range& range);
};

char* getExecutablePath();