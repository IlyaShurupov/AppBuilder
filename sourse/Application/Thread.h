#pragma once

#include "Object.h"
#include "Primitives.h"

struct Operator : ObjBasedClass<Operator> {

    Operator() { InitData(); }
    Operator(Obj* prnt) : ObjBasedClass (prnt) { InitData(); }
    
    bool parallel = false; 

    void InitData() {
        ObDict::Add(this, "Invoke Args").Assign("Obj", true);
        ObDict::Add(this, "RunTime Args").Assign("Obj", true);
    }

    virtual bool Poll() { return false; }
    virtual void Invoke() { }
    virtual void Modal() { }

    virtual ~Operator() {}

    Str GetState() {
        return state;
    }

    private:
    friend struct Application;
    Str state = "INVOKE";
};

struct Requester : ObjBasedClass<Requester> {
    
    Requester() {
        Link::Add(this, "Target Op").Init("Operator", true);
    }

    void CreateRequest(ObList* requests) {
        if (Link::Get(this, "Target Op").modified) {
            UpdateTargetOp();
        }

        if (!exec_op->parallel || NotRuning()) {
            requests->AddObj(exec_op);
        }
    }

    virtual ~Requester() {}
    
    private:
    Operator* exec_op = nullptr;

    void UpdateTargetOp() {
        Operator* op = (Operator*)Link::Get(this, "Target Op").GetLink();

        if (op->parallel) {

            if (!exec_op) {
                exec_op = &(Operator&)op->Instance();

            } else if (exec_op->type.idname != op->type.idname) {
                delete (Obj*)exec_op;
                exec_op = &(Operator&)op->Instance();
            }

        } else {
            exec_op = op;
        }
    }

    bool NotRuning() {
        return exec_op->GetState() != "RUNNING";
    }
};