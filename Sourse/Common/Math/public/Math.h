#pragma once
#include <stdio.h>

#define DLLEXPORT __declspec(dllexport)

#define Pi float(3.1415926535897932384626433832795)
#define INF long long int (4611686018427387904)

void MATH_clamp(float* val, float min, float max);
float MATH_abs(float val);
void MATH_CrossProduct(float v[3], float v1[3], float v2[3]);
float MATH_DotProduct(float v1[3], float v2[3]);
void MATH_ScalarMultiplication(float v1[3], float s);
void MATH_VectorSum(float v[3], float v1[3], float v2[3]);
void MATH_VectorDifference(float v[3], float v1[3], float v2[3]);
void MATH_RotateZAroundCenter(float v1[3], float angel);
void MATH_RotateYAroundCenter(float v1[3], float angel);
void MATH_RotateXAroundCenter(float v1[3], float angel);
float MATH_VectorMagnetude(float v[3]);
void MATH_CopyVector(float v1[3], float v2[3]);
void MATH_ScalarSum(float v[3], float s);
float MATH_AngelBetweenTwoVectors(float v1[3], float v2[3]);
int MATH_factorial(int n);
void MATH_MakeUnitVector(float v[3]);
float MATH_Power(float x, float n);
float MATH_sinf(float x);
float MATH_own_cosF(float x);
float MATH_own_tanF(float x);
double MATH_sin(float s);
double MATH_asin(float s);
double MATH_cos(float s);
double MATH_acos(float s);
double MATH_tan(float s);
double MATH_atan(float s);
double MATH_sqrt(float s);
double MATH_log(float s);
void MATH_RotateArbAxes(float RotVector[3], float axs_pos[3], float RotPoint[3], float rot_angel);