#include "public/Operator.h"
#include "public/Context.h"

struct OperetorArgs {
	Context* C;
	Operator* op;

	OperetorArgs(Context* C, Operator* op) {
		this->C = C;
		this->op = op;
	}
};

void AddOperator(Context& C, void(*op_head)(OperetorArgs& args)) {

}


void CreateOperatopRuntime_poll(OperetorArgs& args) {
	FOREACH_NODE(Operator, (&args.C->Operators), op_node) {
		//args.op->Properties.;
		if (1) {

		}
	}
}

void CreateOperatopRuntime_ecec(OperetorArgs& args) {
}

void CreateOperatopRuntime_modal(OperetorArgs& args) {
}

void CreateOperatopRuntime_invoke(OperetorArgs& args) {
}

void CreateOperatopRuntime(OperetorArgs& args) {
	op->Poll = CreateOperatopRuntime_poll;
	op->Execute = CreateOperatopRuntime_ecec;
	op->Invoke = CreateOperatopRuntime_invoke;
	op->Modal = CreateOperatopRuntime_modal;
	op->State = FINISHED;

	PropertyFuncAdress<void, OperetorArgs> prop =
		PropertyFuncAdress<void, OperetorArgs>();
	op->Properties->add(prop);
}


void OpsInit(Context& C) {
	Operator* op = new Operator;
	CreateOperatopRuntime(OperetorArgs(&C, op));
	C.Operators.add(op);


}