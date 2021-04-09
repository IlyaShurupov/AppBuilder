#pragma once

#include "Object.h"
#include "Primitives.h"

enum class OpState {
    NONE,
    INVOKED,
    RUNNING,
    FINISHED,
};

class Operator : public Obj {

    Operator& operator = (const Operator& in);
    Operator(const Operator& in) : Obj(in) {
        instance_count = in.instance_count;
        parallel = in.parallel; 
        state = in.state;
    } 

    public:

    int* instance_count = nullptr;
    bool parallel = false; 
    OpState state = OpState::NONE;

    Operator(Obj* prnt) : Obj(prnt) { 
        RegisterType(ObjType("Operator"));
        
        ObDict& args = ADDOBJ(ObDict, Interface, *this, (this)).Assign("Obj", true);
        args.AddObj(new String(&args), "Run Time Arg");
        
        ADDOBJ(Link, Args, *this, (this)).Init("Obj", true);
    }

    virtual Operator& Instance() {
        return *new Operator(*this);
    }

    virtual bool Poll() { return false; }
    virtual void Invoke() {}
    virtual void Modal() {}

    virtual ~Operator() {}

    bool Finished() { return state == OpState::FINISHED; }
};

class OpHolder : public Obj {

    OpHolder& operator = (const OpHolder& in);
    OpHolder(const OpHolder& in) : Obj(in) {
        op = in.op;
        threads = in.threads;
    } 

    public:

    OpHolder(Obj* prnt, Operator* _op) : Obj(prnt) {
        RegisterType(ObjType("OpHolder"));
        op = _op;
    }
    
    virtual OpHolder& Instance() {
        return *new OpHolder(*this);
    }

    Operator* op;
    List<Operator> threads;
    
    Operator* GetInstance(ObDict* args_link, int* _instance_count) {
        UpdateThreads();

        if (op->parallel || !threads.Len()) {
            Operator* instance = &op->Instance();

            *_instance_count += 1;
            instance->instance_count = _instance_count;
            GETOBJ(Link, instance, Args).SetLink(args_link);

            threads.PushBack(instance);
            return instance;
        }

        return nullptr;
    }
    
    void GetInterface(ObDict* args) {
        delete args;
        args = &GETOBJ(ObDict, op, Interface).Instance();
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

class Requester : public Obj {

    Requester& operator = (const Requester& in);

    public:

    Requester(const Requester& in) : Obj(in) {
        instance_count = in.instance_count;
    } 

    Requester(Obj* prnt) : Obj(prnt) { 

        RegisterType(ObjType("Requester"));
        
        ADDOBJ(ObDict, Op Args, *this, (this)).Assign("Obj", true);

        Link& target_link = ADDOBJ(Link, Target Op, *this, (this));
        target_link.Init("OpHolder", true);
        target_link.BindModPoll(this, CanChangeTarget);
        target_link.AddOnModCallBack(this, TargetChanged);
    }

    virtual Requester& Instance() {
        return *new Requester(*this);
    }
    
    void CreateRequest(ObList* requests) {      
        OpHolder* holder = (OpHolder*)GETOBJ(Link, this, Target Op).GetLink();
        Operator* instance = holder->GetInstance(&GETOBJ(ObDict, this, Op Args), &instance_count);
        if (instance) {
            requests->AddObj(instance);
        }
    }

    void SendArg(String& rtarg) {
        GETOBJ(ObDict, this, Op Args).AddObj(&rtarg, "Run Time Arg");
    }

    bool Running() {
        return instance_count;
    }

    virtual ~Requester() {}
    
    private:

    int instance_count = 0;

    static void TargetChanged(Obj* param, ModType type) {
        Requester * ths = (Requester *)param;
        OpHolder* holder = (OpHolder*)GETOBJ(Link, ths, Target Op).GetLink();
        holder->GetInterface(&GETOBJ(ObDict, ths, Op Args));
    }

    static bool CanChangeTarget(Obj* param) {
        return ((Requester *)param)->instance_count == 0;
    }
};