#pragma once

enum OpState {
	RUNNING,
	FINESHED,
	CANCELED,
};

struct Operator {
	OpState(*Invoke)(Context& C, void* CustomData) = nullptr;
	OpState(*Execute)(Context& C, void* CustomData) = nullptr;
	OpState(*Modal)(Context& C, void* CustomData) = nullptr;
	OpState(*Poll)(Context& C, void* CustomData) = nullptr;
	void* CustomData = nullptr;
};