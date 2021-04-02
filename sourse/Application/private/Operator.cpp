
#include "Operator.h"

Operator::Operator() : Obj(this, "Operator") {        
    AddMethod("Poll") = Poll;
    AddMethod("Invoke") = Invoke;
    AddMethod("Modal") = Modal;
}

void Operator::Poll(Method* me) {

}

void Operator::Invoke(Method* me) {

}

void Operator::Modal(Method* me) {

}