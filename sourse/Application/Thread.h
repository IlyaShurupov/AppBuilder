#pragma once

#include "Object.h"
#include "Primitives.h"

enum class OpState {
    NONE,
    INVOKED,
    RUNNING,
    FINISHED,
};

class Operator : public ObjBasedClass<Operator> {

    public:

    Operator() { 
        ADDOBJ(ObDict, Interface, *this, (this)).Assign("Obj", true);
        ADDOBJ(Link, Args, *this, (this)).Init("Obj", true);
    }

    Operator& operator = (const Operator& in) {
        return *this;
    }

    virtual bool Poll() { return false; }
    virtual void Invoke() { }
    virtual void Modal() { }

    virtual ~Operator() { 
    }

    bool Finished() { return state == OpState::FINISHED; }

    int* instance_count = nullptr;
    bool parallel = false; 
    OpState state = OpState::NONE;
};

struct OpHolder : ObjBasedClass<OpHolder> {

    OpHolder(){}
    OpHolder(Obj* prnt, Operator* _op) : ObjBasedClass (prnt) { 
        op = _op;
        op->prnt = this;
    }

    Operator* op;
    List<Operator> threads;
    
    Operator* GetInstance(ObDict* args_link, int* _instance_count) {
        UpdateThreads();

        if (op->parallel || !threads.Len()) {
            Operator* instance = new Operator();
            ((Obj*)instance)->Copy((Obj*)op);

            *_instance_count += 1;
            instance->instance_count = _instance_count;
            Link::Get(instance, "Args").SetLink(args_link);

            threads.PushBack(instance);
            return instance;
        }

        return nullptr;
    }
    
    void GetInterface(ObDict* args) {
        args->Copy(&ObDict::Get(op, "Interface"));
    }

    void UpdateThreads() {
        FOREACH(&threads, Operator, op) {
            if (op->Finished()) {
                *op->instance_count -= 1;
                threads.DelNode(op.node());
            }
        }
    }
};

struct Requester : ObjBasedClass<Requester> {
    
    Requester() {
        ADDOBJ(ObDict, Op Args, *this, (this)).Assign("Obj", true);

        Link& target_link = ADDOBJ(Link, Target Op, *this, (this));
        target_link.Init("OpHolder", true);
        target_link.BindModPoll(this, CanChangeTarget);
        target_link.AddOnModCallBack(this, TargetChanged);
    }

    void CreateRequest(ObList* requests) {      
        OpHolder* holder = (OpHolder*)Link::Get(this, "Target Op").GetLink();
        Operator* instance = holder->GetInstance(&ObDict::Get(this, "Op Args"), &instance_count);
        if (instance) {
            requests->AddObj(instance);
        }
    }

    virtual ~Requester() {}
    
    private:

    int instance_count = 0;

    static void TargetChanged(Obj* param) {
        Requester * ths = (Requester *)param;
        OpHolder* holder = (OpHolder*)Link::Get(ths, "Target Op").GetLink();
        holder->GetInterface(&ObDict::Get(ths, "Op Args"));
    }

    static bool CanChangeTarget(Obj* param) {
        return ((Requester *)param)->instance_count == 0;
    }
};