
#include "Primitives.h"

#include <cassert>


String::String(const String& in) : Obj(in) {
	string = in.string;
}

String::String(Obj* prnt) : Obj(prnt) {
	RegisterType("String");
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

Str String::as_string() {
	return string;
}

bool String::from_string(Str* str) {
	return Assign(*str);
}

String::~String() {
}


ObListIter::ObListIter(const Str* itertype, class ObList* oblist, int start_idx) {
	this->oblist = oblist;

	if (itertype) {
		this->itertype = itertype;
		type = true;
	}
	else {
		type = false;
	}

	idx = start_idx;
	
	iter = oblist->list.Find(start_idx);
	while (iter && type && !iter->data->type->InheritsFrom(itertype)) {
		iter = iter->next;
	}
}

void ObListIter::operator ++() {
	iter = iter->next;
	while (iter && type && !iter->data->type->InheritsFrom(itertype)) {
		iter = iter->next;
	}
	idx++;
}

int ObListIter::Len(const Str& type) {
	int len = 0;
	for (auto item : oblist->list) {
		if (item->type->InheritsFrom(type)) {
			len++;
		}
	}
	return len;
}

int ObListIter::Len() {
	return oblist->list.Len();
}

ObList::ObList(const ObList& in) : Obj(in) {
	list_type = in.list_type;
	
	list.Clear();

	const Node<Obj>* in_iter = in.list.First();
	while (in_iter) { 
		Obj& copy = in_iter->data->Instance();
		copy.prnt = this;
		list.PushBack(&copy);
		in_iter = in_iter->next;
	}

	base_class = in.base_class;
}

ObList::ObList(Obj* prnt) : Obj(prnt) {
	RegisterType("ObList");
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
		if (obj->type->InheritsFrom(list_type)) {
			list.PushBack(obj);
		}
	}
	else if (obj->type->idname == list_type) {
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
	RegisterType("Link");
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
		if (!obj || obj->type->InheritsFrom(link_type)) {
			link = obj;
		}
	}
	else if (obj->type->idname == link_type) {
		link = obj;
		current_type = obj->type->idname;
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

alni max = ALIGNED_MAX;
alni min = ALIGNED_MIN;

Int::Int(Obj* prnt) : Obj(prnt) {
	RegisterType("Int");
}


bool Int::Assign(alni _val, alni _min, alni _max) {
	if (!CanModify()) {
		return false;
	}
	val = _val;
	min = _min;
	max = _max;
	Modified(ModType::SET);
	return true;
}

alni Int::GetVal() {
	return val;
}

bool Int::Set(alni _val) {
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

Str Int::as_string() {
	return (int)val;
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
	RegisterType("Float");
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

Str Float::as_string() {
	return val;
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
	RegisterType("Bool");
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

Str Bool::as_string() {
	if (val) {
		return "True";
	}
	else {
		return "False";
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


ObDictIter::ObDictIter(const Str* itertype, class ObDict* oblist, int start_idx) {
	this->obdict = oblist;

	if (itertype) {
		this->itertype = itertype;
		type = true;
	}
	else {
		type = false;
	}

	idx = start_idx;

	for (slot_idx = start_idx; slot_idx < obdict->dict->nslots; slot_idx++) {
		if (!obdict->dict->table[slot_idx]) {
			continue;
		}

		if (type && !obdict->dict->table[slot_idx]->val->type->InheritsFrom(itertype)) {
			continue;
		}

		break;
	}

}

void ObDictIter::operator ++() {

	do {
		slot_idx++;
		if (slot_idx == obdict->dict->nslots) {
			break;
		}

	} while (!obdict->dict->table[slot_idx] || (type && !obdict->dict->table[slot_idx]->val->type->InheritsFrom(itertype)));

	idx++;
}

int ObDictIter::Len(const Str& type) {
	int len = 0;
	for (auto item : *obdict->dict) {
		if (item->val->type->InheritsFrom(type)) {
			len++;
		}
	}
	return len;
}

int ObDictIter::Len() {
	return obdict->dict->nentries;
}

Obj* ObDictIter::data(){
	return obdict->dict->table[slot_idx]->val;
}

Str ObDictIter::name() {
	return obdict->dict->table[slot_idx]->key;
}

ObDict::ObDict(const ObDict& in) : Obj(in) {
	dict_type = in.dict_type;
	dict = new DictObj;
	*dict = *in.dict;
	base_class = in.base_class;
}

ObDict::ObDict(Obj* prnt) : Obj(prnt) {
	RegisterType("ObDict");
	dict = new DictObj;
}

ObDict& ObDict::Assign(Str _dict_type, bool _base_class) {
	dict_type = _dict_type;
	base_class = _base_class;
	return *this;
}

DictObj& ObDict::GetDict() {
	return *dict;
}

bool ObDict::AddObj(Obj* obj, STRR name) {
	if (!CanModify()) {
		return false;
	}

	if (base_class) {
		if (obj->type->InheritsFrom(dict_type)) {
			dict->Put(name, obj);
		}
	}
	else if (obj->type->idname == dict_type) {
		dict->Put(name, obj);
	}
	else {
		return false;
	}

	Modified(ModType::SET);
	return true;
}

Obj& ObDict::GetObj(STRR name) {
	return *dict->Get(name);
}

ObDict::~ObDict() {
}



ObjTuple::ObjTuple(const ObjTuple& in) : Obj(in) {}

ObjTuple::ObjTuple(Obj* prnt) : Obj(prnt) {
	RegisterType("Tuple");
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
	RegisterType("Bool Compare Expretion");

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
	RegisterType("Color");

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
	assert((Str("Color") == obj.type->idname));

	return (
		GETOBJ(Float, this, R).GetVal() == GETOBJ(Float, this, R).GetVal() &&
		GETOBJ(Float, this, G).GetVal() == GETOBJ(Float, this, G).GetVal() &&
		GETOBJ(Float, this, B).GetVal() == GETOBJ(Float, this, B).GetVal() &&
		GETOBJ(Float, this, A).GetVal() == GETOBJ(Float, this, A).GetVal());
}

/*
void Method::Call() {
	func->execute();
}

bool Method::Compile() {
	func->compile();
	return true;
}

Method& Method::operator=(const Method& in) {
	func = in.func;
	return *this;
}

Method::Method(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("Method"));
}
*/
