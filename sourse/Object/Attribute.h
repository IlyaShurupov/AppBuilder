
#pragma once

#include "Types.h"

struct Obj;

using ObjList = List<Obj>;
using ValList = List<struct Value>;

enum ValType {
    NONE, 
    BOOL, 
    INT, 
    FLOAT,
    LINK,
    STRING,
    LIST,
};

struct Value {

    ValType type;

    union ValueData { 
        
        ValueData();
        ~ValueData();

        aligned integer; 
        float floating; 
        bool boolean;
        Str string;
        ValList list;
        
        struct LinkData {
            Obj* link;
            Str link_type;
        } link_data;
        
    } data;

    Value(ValType p_type);  

    void Free();

    Value& operator = (const Value& in);
    Value& operator = (const aligned& in);
    Value& operator = (const float& in);
    Value& operator = (const bool& in);
    Value& operator = (Obj* in);
    Value& operator = (const Str& in);
    Value& operator = (const char* in);
    Value& operator = (const ValList& in);

    bool IsVal();
    Obj* AsLink();
    aligned AsInt();
    float AsFloat();
    bool AsBool();
    bool AsString();

    ~Value();
};
