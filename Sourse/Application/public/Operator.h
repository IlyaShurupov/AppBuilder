#pragma once
#include "Property.h"

enum OpState {
	RUNNING,
	FINISHED,
	CANCELED,
	EXECUTE,
	INVOKE
};

struct Operator {
	OpState State;

	void (*Invoke)(struct Context& C, Operator& op) = nullptr;
	void (*Execute)(struct Context& C, Operator& op) = nullptr;
	void (*Modal)(struct Context& C, Operator& op) = nullptr;
	void (*Poll)(struct Context& C, Operator& op) = nullptr;

	void* CustomData = nullptr;

	List<void>* Properties;
};

void OpsInit(struct Context& C);