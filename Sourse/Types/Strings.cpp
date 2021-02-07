
#include "public/Strings.h"

#include "public/Mem.h"


//#include <string.h> // get rid of
#include <cstdlib>

inline str_idx cstrlen(const char* str) {
  str_idx len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

char* getExecutablePath() {

  char* exePath;
  if (_get_pgmptr(&exePath) != 0)
    exePath = "";
  return exePath;
}


str_idx Range::len() {
  return end - strt + 1;
}

bool Range::valid() {
  return (strt > 0 && end > 0 && end >= strt);
}

Range::Range() {
  strt = end = -1;
}

Range::Range(str_idx strt, str_idx end) {
  this->strt = strt;
  this->end = end;
}

Range::Range(Range& rng) {
  strt = rng.strt;
  end = rng.end;
}


Str::Str() {}

Str::~Str() {
  clear();
}

Str::Str(const char* string) {
  alloc(cstrlen(string));
  for (str_idx i = 0; i < length; i++) {
    str[i] = string[i];
  }
}

str_idx Str::len() {
  return length;
}

void Str::alloc(str_idx len) {
  clear();

  str = NEW_DBG_AR(char, len + 1);

  str[len] = '\0';
  length = len;
}

void Str::clear() {
  if (str) {
    DELETE_DBG_AR(str);
    // DELETE_DBG()[] str;
  }
}

void Str::operator=(Str& string) {

  alloc(string.length);

  for (str_idx i = 0; i < string.length; i++) {
    str[i] = string.str[i];
  }

  length = string.length;
}

void Str::operator+=(const Str& string) {

  str_idx newlen = string.length + length;
  char* newstr = NEW_DBG_AR(char, newlen + 1);
  newstr[newlen] = '\0';

  for (str_idx i = 0; i < length; i++) {
    newstr[i] = str[i];
  }

  for (str_idx i = 0; i < string.length; i++) {
    newstr[i + length] = string.str[i];
  }

  clear();

  str = newstr;
  length = newlen;
}

void Str::operator=(const char* string) {
  alloc(cstrlen(string));
  for (str_idx i = 0; i < length; i++) {
    str[i] = string[i];
  }
}

bool Str::operator==(char* cstring) {
  if (cstrlen(cstring) != length) {
    return false;
  }
  for (long i = 0; i < length; i++) {
    if (str[i] != cstring[i]) {
      return false;
    }
  }
  return true;
}

bool Str::operator==(Str& string) {
  if (string.length != length) {
    return false;
  }
  for (long i = 0; i < length; i++) {
    if (str[i] != string.str[i]) {
      return false;
    }
  }
  return true;
}

char Str::operator[](str_idx idx) {
  return str[idx];
}

bool Str::match(Range& range, Str& str2, Range& range2) {
  if (range.len() != range2.len()) {
    return false;
  }
  for (str_idx i = 0; i < range.len(); i++) {
    if (str[range.strt + i] != str2[range2.strt + i]) {
      return false;
    }
  }
  return true;
}

void Str::coppy(Str& str, Range& range) {
  alloc(range.len());
  for (str_idx i = 0; i < length; i++) {
    this->str[i] = str[i + range.strt];
  }
}

str_idx Str::find(Str& string, Range& range) {
  for (str_idx i = range.strt; i < range.end; i++) {

    str_idx m = 0;
    while (m < string.len()) {
      if (this->str[i] != str[m]) {
        break;
      }
      m++;
    }

    if (m == string.len() - 1) {
      return i;
    }
  }
  return -1;
}

str_idx Str::find(const char character, Range& range) {
  for (str_idx i = range.strt; i <= range.end; i++) {
    if (character == str[i]) {
      return i;
    }
  }
  return -1;
}

str_idx Str::rfind(const char character, Range& range) {
  for (str_idx i = range.end; i >= range.strt; i--) {
    if (character == str[i]) {
      return i;
    }
  }
  return -1;
}

void Str::trim(Range& range) {
  str_idx newlen = range.len();
  char* newstr = NEW_DBG_AR(char, newlen + 1);
  newstr[newlen] = '\0';

  for (str_idx i = 0; i < newlen; i++) {
    newstr[i] = str[i + range.strt];
  }

  clear();
  str = newstr;
  length = newlen;
}
