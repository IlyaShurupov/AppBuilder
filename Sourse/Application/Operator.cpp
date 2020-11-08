#include "public/Operator.h"
#include "public/Context.h"


void CreateOperatopRuntime_poll(Context* C, Operator* op) {
	FOREACH_NODE(Operator, (&C->Operators), op_node) {
		//args.op->Properties.;
		if (1) {

		}
	}
}

void CreateOperatopRuntime_ecec(Context* C, Operator* op) {
}

void CreateOperatopRuntime_modal(Context* C, Operator* op) {
}

void CreateOperatopRuntime_invoke(Context* C, Operator* op) {
}

void CreateOperatopRuntime(Context* C, Operator* op) {
	op->Poll = CreateOperatopRuntime_poll;
	op->Execute = CreateOperatopRuntime_ecec;
	op->Invoke = CreateOperatopRuntime_invoke;
	op->Modal = CreateOperatopRuntime_modal;
	op->State = FINISHED;

	PropertyFuncAdress* prop = new PropertyFuncAdress();
	op->FuncProps.add(prop);

}


void OpsInit(Context* C) {
	Operator* op = new Operator;
	CreateOperatopRuntime(C, op);
	C->Operators.add(op);


}