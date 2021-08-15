
#pragma once

#include "Strings/Strings.h"

class File {

public:

	char* buff = nullptr;
	int size = 0;

	File(const Str& path);
};

