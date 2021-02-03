#pragma once

#define EPSILON 0.0000001
#define Pi float(3.1415926535897932384626433832795)
typedef int SCR_UINT;
#define SCR_UINT_MAX USHRT_MAX
#define ABS(val) \
  if (val < 0)   \
  val = -val
#define CLAMP(val, min, max) \
  if (val > max)             \
    val = max;               \
  if (val < min)             \
  val = min

#define NAME(v) #v

#define FLT_MAX 3.402823466e+38F
#define FLT_MIN 1.175494351e-38F

#define SHRT_MIN (-32768)
#define SHRT_MAX 32767
#define USHRT_MAX 0xffff
#define INT_MIN (-2147483647 - 1)
#define INT_MAX 2147483647
#define UINT_MAX 0xffffffff
#define LONG_MIN (-2147483647L - 1)
#define LONG_MAX 2147483647L
#define ULONG_MAX 0xffffffffUL
#define LLONG_MAX 9223372036854775807i64
#define LLONG_MIN (-9223372036854775807i64 - 1)
#define ULLONG_MAX 0xffffffffffffffffui64

#define OFFSET(first, offset) (*((&first) + offset))
#define DOIF(action, cond) \
  if ((cond)) {            \
    action;                \
  }

#define IF(cond, action) \
  if ((cond)) {            \
    action;                \
  }

#define INRANGE(val, bd1, bd2) ((val) >= (bd1) && (val) <= (bd2))