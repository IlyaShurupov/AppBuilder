#include "public/Logic.h"
#include "public/Operator.h"

int MainLoop(Context& C) {

	while (true) {

		C.ActiveWin->Upd();
		if (C.ActiveWin->IsEvent()) {
			continue; //TODO: go sleep
		}

		FOREACH_NODE(Operator, (&C.Operators), op_node) {
			Operator* Op = op_node->Data;
			
			switch (Op->State) {

				case FINISHED:
					Op->Poll(C, Op->CustomData);
					break;

				case RUNNING:
					Op->Modal(C, Op->CustomData);
					break;

				case EXECUTE:
					Op->Execute(C, Op->CustomData);
					break;

				case INVOKE:
					Op->Invoke(C, Op->CustomData);
					break;
			}
		}
	}
}