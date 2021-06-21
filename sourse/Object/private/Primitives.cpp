
#include "Primitives.h"


String::String(const String& in) : Obj(in) {
	string = in.string;
}

String::String(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("String"));
}

bool String::Assign(STRR _string) {
	if (!CanModify()) {
		return false;
	}
	string = _string;
	Modified(ModType::SET);
	return true;
}

bool String::Equal(const Obj& obj) {
	assert(type == obj.type);
	return string == ((String&)obj).GetStr();
}

Str& String::GetStr() {
	return string;
}

void String::as_string(Str* str) {
	*str = string;
}

bool String::from_string(Str* str) {
	return Assign(*str);
}

String::~String() {
}



ObList::ObList(const ObList& in) : Obj(in) {
	list_type = in.list_type;
	list = in.list;
	base_class = in.base_class;
}

ObList::ObList(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("ObList"));
}

ObList& ObList::Assign(Str _list_type, bool _base_class) {
	list_type = _list_type;
	base_class = _base_class;
	return *this;
}

List<Obj>& ObList::GetList() {
	return list;
}

bool ObList::AddObj(Obj* obj) {
	if (!CanModify()) {
		return false;
	}

	if (base_class) {
		if (obj->type.IsPrnt(list_type)) {
			list.PushBack(obj);
		}
	}
	else if (obj->type.idname == list_type) {
		list.PushBack(obj);
	}
	else {
		return false;
	}

	Modified(ModType::SET);
	return true;
}

const Str& ObList::ListType() {
	return list_type;
}

ObList::~ObList() {
}



Link::Link(const Link& in) : Obj(in) {
	link_type = in.link_type;
	link = in.link;
	base_class = in.base_class;
	current_type = in.current_type;
}

Link::Link(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("Link"));
}

void Link::Init(STRR _link_type, bool _base_class) {
	link_type = _link_type;
	base_class = _base_class;
}

bool Link::SetLink(Obj* obj) {
	if (!CanModify()) {
		return false;
	}

	if (base_class) {
		if (obj->type.IsPrnt(link_type)) {
			link = obj;
			current_type = obj->type.idname;
		}
	}
	else if (obj->type.idname == link_type) {
		link = obj;
		current_type = obj->type.idname;
	}
	else {
		return false;
	}
	Modified(ModType::SET);
	return true;
}

bool Link::Equal(const Obj& obj) {
	assert(type == obj.type);
	return link == ((Link&)obj).GetLink();
}

Obj* Link::GetLink() {
	return link;
}

Link::~Link() {
}



Int::Int(const Int& in) : Obj(in) {
	val = in.val;
	max = in.max;
	min = in.min;
}

aligned max = ALIGNED_MAX;
aligned min = ALIGNED_MIN;

Int::Int(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("Int"));
}


bool Int::Assign(aligned _val, aligned _min, aligned _max) {
	if (!CanModify()) {
		return false;
	}
	val = _val;
	min = _min;
	max = _max;
	Modified(ModType::SET);
	return true;
}

aligned Int::GetVal() {
	return val;
}

bool Int::Set(aligned _val) {
	if (!CanModify()) {
		return false;
	}
	val = _val;
	CLAMP(val, min, max);
	Modified(ModType::SET);
	return true;
}

bool Int::Equal(const Obj& obj) {
	assert(type == obj.type);
	return val == ((Int&)obj).GetVal();
}

void Int::as_string(Str* str) {
	*str = Str((int)val);
}

bool Int::from_string(Str* str) {
	int input_val = 0;
	if (str->to_val(input_val) && Set(input_val)) {
		return true;
	}
	return false;
}

Int::~Int() {
}


Float::Float(const Float& in) : Obj(in) {
	val = in.val;
	max = in.max;
	min = in.min;
}

Float::Float(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("Float"));
}

bool Float::Assign(float _val, float _min, float _max) {
	if (!CanModify()) {
		return false;
	}
	val = _val;
	min = _min;
	max = _max;
	Modified(ModType::SET);
	return true;
}

float Float::GetVal() {
	return val;
}

bool Float::Set(float _val) {
	if (!CanModify()) {
		return false;
	}
	val = _val;
	CLAMP(val, min, max);
	Modified(ModType::SET);
	return true;
}

bool Float::Equal(const Obj& obj) {
	assert(type == obj.type);
	return val == ((Float&)obj).GetVal();
}

void Float::as_string(Str* str) {
	*str = Str(val);
}

bool Float::from_string(Str* str) {
	float input_val = 0;
	if (str->to_val(input_val) && Set(input_val)) {
		return true;
	}
	return false;
}

Float::~Float() {
}


Bool::Bool(const Bool& in) : Obj(in) {
	val = in.val;
}

Bool::Bool(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("Bool"));
}


Bool& Bool::Assign(bool _val) {
	val = _val;
	return *this;
}

bool Bool::GetVal() {
	return val;
}

bool Bool::Set(bool _val) {
	if (!CanModify()) {
		return false;
	}
	val = _val;
	Modified(ModType::SET);
	return true;
}

bool Bool::Equal(const Obj& obj) {
	assert(type == obj.type);
	return val == ((Bool&)obj).GetVal();
}

void Bool::as_string(Str* str) {
	if (val) {
		*str = "True";
	}
	else {
		*str = "False";
	}
}

bool Bool::from_string(Str* str) {
	bool input_val = 0;
	if (str->to_val(input_val) && Set(input_val)) {
		return true;
	}
	return false;
}

Bool::~Bool() {
}



ObDict::ObDict(const ObDict& in) : Obj(in) {
	dict_type = in.dict_type;
	dict = in.dict;
	base_class = in.base_class;
}

ObDict::ObDict(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("ObDict"));
}

ObDict& ObDict::Assign(Str _dict_type, bool _base_class) {
	dict_type = _dict_type;
	base_class = _base_class;
	return *this;
}

DictObj& ObDict::GetDict() {
	return dict;
}

bool ObDict::AddObj(Obj* obj, STRR name) {
	if (!CanModify()) {
		return false;
	}

	if (base_class) {
		if (obj->type.IsPrnt(dict_type)) {
			dict.Put(name, obj);
		}
	}
	else if (obj->type.idname == dict_type) {
		dict.Put(name, obj);
	}
	else {
		return false;
	}

	Modified(ModType::SET);
	return true;
}

Obj& ObDict::GetObj(STRR name) {
	return *dict.Get(name);
}

ObDict::~ObDict() {
}



ObjTuple::ObjTuple(const ObjTuple& in) : Obj(in) {}

ObjTuple::ObjTuple(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("Tuple"));
	ADDOBJ(Link, Head, *this, (this));
	ADDOBJ(Link, Tail, *this, (this));
}

Link& ObjTuple::GetHead() {
	return GETOBJ(Link, this, Head);
}

Link& ObjTuple::GetTail() {
	return GETOBJ(Link, this, Tail);
}

ObjTuple::~ObjTuple() {
}




CompareExpr::CompareExpr(const CompareExpr& in) : Obj(in) {}

CompareExpr::CompareExpr(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("Bool Compare Expretion"));

	ADDOBJ(ObList, Conditions, *this, (this)).Assign("Tuple", false);
	ADDOBJ(Bool, Or, *this, (this)).Assign(false);
}

bool CompareExpr::Evaluate() {
	bool any = GETOBJ(Bool, this, Or).GetVal();

	List<Obj>& conditions = GETOBJ(ObList, this, Conditions).GetList();

	for (auto tpl_obj : conditions) {
		ObjTuple* tpl = (ObjTuple*)tpl_obj.Data();

		if (!tpl->GetHead().GetLink() || !tpl->GetTail().GetLink()) {
			continue;
		}

		if (tpl->GetHead().GetLink()->Equal(*tpl->GetTail().GetLink())) {
			if (any) {
				return true;
			}
		}
		else if (!any) {
			return false;
		}
	}

	return true;
}

bool CompareExpr::Equal(const Obj& obj) {
	assert(type == obj.type);
	return Evaluate() == ((CompareExpr&)obj).Evaluate();
}


ColorObj::ColorObj(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("Color"));

	ADDOBJ(Float, R, *this, (this)).Assign(0, 0, 1);
	ADDOBJ(Float, G, *this, (this)).Assign(0, 0, 1);
	ADDOBJ(Float, B, *this, (this)).Assign(0, 0, 1);
	ADDOBJ(Float, A, *this, (this)).Assign(1, 0, 1);
}

void ColorObj::Get(Color* col) {
	col->r = GETOBJ(Float, this, R).GetVal();
	col->g = GETOBJ(Float, this, G).GetVal();
	col->b = GETOBJ(Float, this, B).GetVal();
	col->a = GETOBJ(Float, this, A).GetVal();
}

bool ColorObj::Set(const Color& col) {

	if (!CanModify()) {
		return false;
	}

	GETOBJ(Float, this, R).Set(col.r);
	GETOBJ(Float, this, G).Set(col.g);
	GETOBJ(Float, this, B).Set(col.b);
	GETOBJ(Float, this, A).Set(col.a);

	return true;
}

bool ColorObj::Equal(const Obj& obj) {
	assert((Str("Color") == obj.type.idname));

	return (
		GETOBJ(Float, this, R).GetVal() == GETOBJ(Float, this, R).GetVal() &&
		GETOBJ(Float, this, G).GetVal() == GETOBJ(Float, this, G).GetVal() &&
		GETOBJ(Float, this, B).GetVal() == GETOBJ(Float, this, B).GetVal() &&
		GETOBJ(Float, this, A).GetVal() == GETOBJ(Float, this, A).GetVal());
}