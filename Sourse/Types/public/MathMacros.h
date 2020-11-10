#pragma once

#include <math.h>
#include <limits.h>
#include <float.h>

#define EPSILON 0.0000001
#define Pi float(3.1415926535897932384626433832795)
typedef unsigned short SCR_UINT;
#define SCR_UINT_MAX USHRT_MAX
#define ABS(val) if (val < 0) val = -val 
#define CLAMP(val, min, max) if (val > max) val = max; if (val < min) val = min