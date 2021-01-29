#pragma once

#define MEM_DEBUG _DEBUG

#ifdef MEM_DEBUG

#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DBG_NEW new
#endif
