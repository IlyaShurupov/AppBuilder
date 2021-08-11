
#include "Primitives.h"

#include <cassert>

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
	while (iter && type && !iter->data->type.IsPrnt(itertype)) {
		iter = iter->next;
	}
}

void ObListIter::operator ++() {
	iter = iter->next;
	while (iter && type && !iter->data->type.IsPrnt(itertype)) {
		iter = iter->next;
	}
	idx++;
}

int ObListIter::Len(const Str& type) {
	int len = 0;
	for (auto item : oblist->list) {
		if (item->type.IsPrnt(type)) {
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
		if (!obj || obj->type.IsPrnt(link_type)) {
			link = obj;
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

		if (type && !obdict->dict->table[slot_idx]->val->type.IsPrnt(itertype)) {
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

	} while (!obdict->dict->table[slot_idx] || (type && !obdict->dict->table[slot_idx]->val->type.IsPrnt(itertype)));

	idx++;
}

int ObDictIter::Len(const Str& type) {
	int len = 0;
	for (auto item : *obdict->dict) {
		if (item->val->type.IsPrnt(type)) {
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
	RegisterType(ObjType("ObDict"));
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
		if (obj->type.IsPrnt(dict_type)) {
			dict->Put(name, obj);
		}
	}
	else if (obj->type.idname == dict_type) {
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

/*
void Method::Call() {
	if (method) {
		method(this);
	}
}


// all for method obj
#include <iostream>
#include <fstream>  //fopen
#include <stdio.h> //delete
#include <windows.h>


long GetFileSize(std::string filename) {
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

const wchar_t* GetWC(const char* c) {
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}

bool Method::Compile() {

	method = nullptr;

	Str& script = GETOBJ(String, this, Script).GetStr();

	Str exe_path = "D:\\Dev\\intern\\Nodes\\out\\msvc\\Debug\\TestApp\\";
	//Str exe_path = ".\\cppfile.cpp";

	system((Str("mkdir ") + exe_path + "tmp").str);

	// create file
	std::ofstream script_file((exe_path + "tmp\\\\cppfile.cpp").str);
	script_file << script.str << std::endl;
	script_file.close();

	// compile
	Str compiler = (exe_path + "compiler\\bin\\cl.exe /c /EHsc /Fo:") + (exe_path + "tmp\\cppfile.obj ");
	Str include1 = Str(" -I ") + exe_path + "include\\Object ";
	Str include2 = Str(" -I ") + exe_path + "include\\Types ";
	Str include3 = Str(" -I ") + exe_path + "compiler\\include ";
	Str include4 = Str(" -I ") + exe_path + "compiler\\include_win ";

	Str compile_command = compiler + include1 + include2 + (exe_path + "tmp\\cppfile.cpp") + include3 + include4;
	system(compile_command.str);

	
	// link
	Str linker = exe_path + "compiler\\bin\\link.exe /MACHINE:X64 /DLL /OUT:" + (exe_path + "tmp\\cppfile.dll ");
	Str lib = exe_path + "lib\\Application.lib ";
	Str libs = " kernel32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ";
	Str libpath = Str(" /LIBPATH:") + exe_path + "compiler\\lib ";

	Str link_command = linker + (exe_path + "tmp\\cppfile.obj ") + lib + libpath + libs;
	system(link_command.str);


	// getting the addresses
	const wchar_t* path = GetWC((exe_path + "tmp\\cppfile.dll ").str);
	HINSTANCE hGetProcIDDLL = LoadLibrary(path);

	if (!hGetProcIDDLL) {
		return false;
	}
	
	void(*func)(Obj*) = (void(*)(Obj*))GetProcAddress(hGetProcIDDLL, "entry");
	void(*func_start)() = (void(*)())GetProcAddress(hGetProcIDDLL, "__start");
	void(*func_end)() = (void(*)())GetProcAddress(hGetProcIDDLL, "__end");


	// creating bytecode
	
	//std::ifstream input("D:\\Dev\\intern\\Nodes\\out\\msvc\\Debug\\TestApp\\tmp\\cppfile.dll", std::ios::binary);
	
	LPVOID lpvBase;               // Base address of the test memory
	LPTSTR lpPtr;                 // Generic character pointer
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	long size = (char*)func_end - (char*)func_start + 200;

	lpvBase = VirtualAlloc(NULL, sysInfo.dwPageSize, MEM_RESERVE, PAGE_NOACCESS);
	lpPtr = (LPTSTR)lpvBase;
	LPVOID lpvResult = VirtualAlloc((LPVOID)lpPtr, sysInfo.dwPageSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);


	for (int i = 0; i < size; i++) {
		*((BYTE*)lpPtr + i) = *(((BYTE*)func) + i);
	}

	func(this);

	((void(*)(Obj*))lpPtr)(this);
	

	method = func;
	return true;
}

Method& Method::operator=(const Method& in) {
	return *this;
}

Method::Method(Obj* prnt) : Obj(prnt) {
	RegisterType(ObjType("Method"));

	ADDOBJ(String, Script, *this, (this));
}

*/