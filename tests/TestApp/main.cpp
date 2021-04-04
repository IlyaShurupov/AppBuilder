
#include "App.h"

#include "Primitives.h"

int main () {
    // Application programm = Application();
    // Func::call(programm, "Run");
    Obj obj("d", nullptr);

    Link::Add(&obj, "dd", "l1");
    Link::Add(&obj, "aa", "l2");

    *obj.props.Get("l1") = *obj.props.Get("l2");

    obj.props.Put("l3=l1", &Obj::Instantiate(*obj.props.Get("l1")));

    Link* l1 = (Link*)obj.props.Get("l1");
    Link* l3 = (Link*)obj.props.Get("l3=l1");
    Link* l2 = (Link*)obj.props.Get("l2");

    
}