#pragma once

#include <crtdbg.h>
#include <cstdlib>

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DBG_NEW new
#endif
