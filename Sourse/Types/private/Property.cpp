#include "Property.h"
#include "MathMacros.h"

PropertyInt::PropertyInt() {
	max = INT_MAX;
	min = INT_MIN;
	val = 0;
}

PropertyInt::PropertyInt(int val, int min, int max) {
	this->max = max;
	this->min = min;
	this->val = val;
}

PropertyInt::~PropertyInt() {

}

void PropertyInt::setVal(int val) {
	this->val = val;
	CLAMP(this->val, min, max);
}

void PropertyInt::setMin(int min) {
	this->min = min;
	CLAMP(this->min, INT_MIN, max);
}

void PropertyInt::setMax(int max) {
	this->max = max;
	CLAMP(max, INT_MAX, min);
}

int PropertyInt::get()
{
	return val;
}


PropertyFloat::PropertyFloat() {
	max = FLT_MAX;
	min = FLT_MIN;
	val = 0;
}

PropertyFloat::PropertyFloat(float val, float min, float max) {
	this->max = max;
	this->min = min;
	this->val = val;
}

PropertyFloat::~PropertyFloat() {

}

void PropertyFloat::assign(float val, float min, float max) {
	this->max = max;
	this->min = min;
	this->val = val;
}

void PropertyFloat::setVal(float val) {
	this->val = val;
	CLAMP(this->val, min, max);
}

void PropertyFloat::setMin(float min) {
	this->min = min;
	CLAMP(this->min, FLT_MIN, max);
}

void PropertyFloat::setMax(float max) {
	this->max = max;
	CLAMP(max, FLT_MAX, min);
}

float PropertyFloat::get()
{
	return val;
}

void PropertyObjectPtr::assign(void* obj_ptr) {
	obj = obj_ptr;
}

PropertyObjectPtr::PropertyObjectPtr() {
	obj = nullptr;
	idname = Str();
}

PropertyObjectPtr::~PropertyObjectPtr() {
}

void PropertyBuffPtr::assign(void* buff_ptr) {
	this->buff = buff_ptr;
}

PropertyBuffPtr::PropertyBuffPtr() {
	buff = nullptr;
	idname = Str();
}

PropertyBuffPtr::~PropertyBuffPtr() {
}


PropertyFuncAdress::PropertyFuncAdress() {
	func = nullptr;
}
PropertyFuncAdress::~PropertyFuncAdress() {
}

Properties::~Properties() {
}
