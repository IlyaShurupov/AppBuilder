#pragma once

#include "MathMacros.h"

// TODO: make all private

class PropertyInt
{
public:
	PropertyInt();
	PropertyInt(int val, int min, int max);
	~PropertyInt();

	void setVal(int val);
	void setMin(int min);
	void setMax(int max);
	int get();

	int val;
	int min, max;
};

class PropertyFloat
{
public:
	PropertyFloat();
	PropertyFloat(float val, float min, float max);
	~PropertyFloat();

	void assign(float val, float min, float max);

	void setVal(float val);
	void setMin(float min);
	void setMax(float max);
	float get();

	float val;
	float min, max;
};