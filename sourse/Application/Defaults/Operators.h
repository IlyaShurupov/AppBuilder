#pragma once

#include "Thread/Thread.h"

class QuitProgramm : public Operator {

    QuitProgramm& operator = (const QuitProgramm& in);
    QuitProgramm(const QuitProgramm& in) : Operator(in.prnt) {} 

    public:

    QuitProgramm(Obj* prnt) : Operator(prnt) { 
        ObDict& args = GETOBJ(ObDict, this, Interface);
        args.AddObj(new Bool(nullptr), "Save");
    }

    virtual QuitProgramm& Instance() {
        return *new QuitProgramm(*this);
    }
    
    bool Poll() { return true; }
    
    void Invoke() { 
        DictObj* args = &((ObDict*)GETOBJ(Link, this, Args).GetLink())->GetDict();
        bool save = ((Bool *)args->Get("Save"))->GetVal();
        exit(0);
    }
};