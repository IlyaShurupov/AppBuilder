#pragma once

#include <float.h>
#include <limits.h>
#include <math.h>
//memory debug
#include <crtdbg.h>
#include <cstdlib>
#include <string>
#include <map>

#define EPSILON 0.0000001
#define Pi float(3.1415926535897932384626433832795)
typedef unsigned short SCR_UINT;
#define SCR_UINT_MAX USHRT_MAX
#define ABS(val) \
  if (val < 0) val = -val
#define CLAMP(val, min, max) \
  if (val > max) val = max;  \
  if (val < min) val = min

#define _CRTDBG_MAP_ALLOC
#define NAME(v) #v
#ifdef _DEBUG
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

