
#include "Attribute.h"

#include "Object.h"

#include <string.h>

bool Value::IsVal() { return type != NONE; };
Obj* Value::AsLink() { return data.link_data.link; };
aligned Value::AsInt() { return data.integer; };
float Value::AsFloat() { return data.floating; } ;
bool Value::AsBool() { return data.boolean; };

Value::Value(ValType p_type) {
    type = p_type;
}

Value::~Value() {
    Free();
}

Value::ValueData::ValueData(){
    memset((void *)&integer, 0, sizeof(ValueData));
}

Value::ValueData::~ValueData() {}

void Value::Free()  {
    switch (type) {
        case STRING:
            data.string.~Str();
            break;
            
        case LIST:
            data.list.~List();
            break;
    }
    memset((void *)&data, 0, sizeof(ValueData));
}

Value& Value::operator = (const Value& in) {

    Free();

    type = in.type;
    
    switch (in.type) {

        case INT:
            data.integer = in.data.integer;
            break;
            
        case LINK:
            data.link_data = in.data.link_data;
            break;
            
        case FLOAT:
            data.floating = in.data.floating;
            break;
            
        case STRING:
            data.string = in.data.string;
            break;
        
        case BOOL:
            data.boolean = in.data.boolean;
            break;
        
        case LIST:
            data.list = in.data.list;
            break;
        
    }
    
    return *this;
}

Value& Value::operator = (const aligned& in) {
    Free();
    type = INT;
    data.integer = in;
    return *this;
}

Value& Value::operator = (const float& in) {
    Free();
    type = FLOAT;
    data.floating = in;
    return *this;
}

Value& Value::operator = (const bool& in) {
    Free();
    type = BOOL;
    data.boolean = in;
    return *this;
}

Value& Value::operator = (Obj* in) {
    Free();
    type = LINK;
    data.link_data.link = in;
    data.link_data.link_type = in->type;
    return *this;
}

Value& Value::operator = (const Str& in) {
    Free();
    type = STRING;
    data.string = in;
    return *this;
}

Value& Value::operator = (const char* in) {
    Free();
    type = STRING;
    data.string = in;
    return *this;
}