
#include "UI/Requester.h"
#include "Thread/ThreadManager.h"

Requester::Requester(const Requester& in) : Obj(in) {
	instance_count = in.instance_count;
}

Requester::Requester(Obj* prnt) : Obj(prnt) {

	RegisterType(ObjType("Requester"));

	ADDOBJ(ObDict, Op Args, *this, (this)).Assign("Obj", true);

	Link& target_link = ADDOBJ(Link, Target Op, *this, (this));
	target_link.Init("OpHolder", true);
	target_link.BindModPoll(this, CanChangeTarget);
	target_link.AddOnModCallBack(this, TargetChanged);
}

void Requester::CreateRequest(ObList* requests) {
	OpHolder* holder = (OpHolder*)GETOBJ(Link, this, Target Op).GetLink();

	if (holder) {

		Operator* instance = holder->GetInstance(&GETOBJ(ObDict, this, Op Args), &instance_count);
		if (instance) {
			requests->AddObj(instance);
		}
	}
}

void Requester::SendArg(String& rtarg) {
	GETOBJ(ObDict, this, Op Args).AddObj(&rtarg, "Run Time Arg");
}

bool Requester::Running() {
	return instance_count;
}

void Requester::TargetChanged(Obj* param, ModType type) {
	Requester* ths = (Requester*)param;
	OpHolder* holder = (OpHolder*)GETOBJ(Link, ths, Target Op).GetLink();
	ths->props.Put("Op Args", holder->GetInterface());
}

bool Requester::CanChangeTarget(Obj* param) {
	return ((Requester*)param)->instance_count == 0;
}