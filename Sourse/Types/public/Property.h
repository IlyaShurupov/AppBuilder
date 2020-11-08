#pragma once

#include "MathMacros.h"
#include "LinkedList.h"

// TODO: a lot to do

template <typename Type>
struct PropertyBoundaries {
	Type min, max;
};

template <typename Type>
class Property {
public:
	Type val;

	Property() {
	}
	Property(Type& val) {
	}
	~Property() {
	}

	void setVal(Type val) {
	}
	Type* getVal() {
	}
};


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

template <typename RetVal, typename Arg>
class PropertyFuncAdress
{
public:
	RetVal (*func)(Arg arg);

	PropertyFuncAdress() {
	}
	~PropertyFuncAdress() {
	}
};

//PropertyFuncAdress* FindProperty() {
//}