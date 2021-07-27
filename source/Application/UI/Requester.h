#pragma once

#include "Object.h"
#include "Primitives.h"

class Requester : public Obj {

    Requester& operator = (const Requester& in);
    int instance_count = 0;

    public:

    Requester(const Requester& in);
    Requester(Obj* prnt);
    
    void CreateRequest(ObList* requests);
    void SendArg(String& rtarg);
    bool Running();

    virtual Requester& Instance() {
        return *new Requester(*this);
    }

    virtual ~Requester() {}
    
    private:
    static void TargetChanged(Obj* param, ModType type);
    static bool CanChangeTarget(Obj* param);
};