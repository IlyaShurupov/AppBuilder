#pragma once

#include "Thread/Thread.h"
#include "IO/File.h"

extern Obj* objs_entry;

class QuitProgram : public Operator {

	QuitProgram& operator = (const QuitProgram& in);
	QuitProgram(const QuitProgram& in) : Operator(in.prnt) {}

public:

	QuitProgram(Obj* prnt) : Operator(prnt) {
		RegisterType("Quit Program");
		ObDict& args = GETOBJ(ObDict, this, Interface);
		args.AddObj(new Bool(&args), "Save");
	}

	virtual QuitProgram& Instance() {
		return *new QuitProgram(*this);
	}

	bool Poll() { return true; }

	void Invoke() {
		DictObj* args = &((ObDict*)GETOBJ(Link, this, Args).GetLink())->GetDict();
		bool save = ((Bool*)args->Get("Save"))->GetVal();
		exit(0);
	}
};

class CopyObject : public Operator {

	CopyObject& operator = (const CopyObject& in);
	CopyObject(const CopyObject& in) : Operator(in) {
		int g = 0;
	}


public:

	CopyObject(Obj* prnt) : Operator(prnt) {
		RegisterType("Copy Object");
		
		ObDict& args = GETOBJ(ObDict, this, Interface);
		
		Link* dest = new Link(&args);
		args.AddObj(dest, "Destination");
		dest->Init("Obj", true);

		Link* target = new Link(&args);
		args.AddObj(target, "Target");
		dest->Init("Obj", true);
	}

	virtual CopyObject& Instance() {
		return *new CopyObject(*this);
	}

	bool Poll() { 
		DictObj& args = ((ObDict*)GETOBJ(Link, this, Args).GetLink())->GetDict();
		return ((Link*)args.Get("Destination"))->GetLink() && ((Link*)args.Get("Target"))->GetLink();
	}

	void Invoke() {
		DictObj& args = ((ObDict*)GETOBJ(Link, this, Args).GetLink())->GetDict();

		Obj* dest = ((Link*)args.Get("Destination"))->GetLink();
		Obj* target = ((Link*)args.Get("Target"))->GetLink();

		Obj* coppy = &target->Instance();
		dest->AddChld(coppy, "Clipboard Object");
		coppy->prnt = dest;


		state = OpState::FINISHED;
	}
};


class SaveProgramm : public Operator {

	SaveProgramm& operator = (const SaveProgramm& in);
	SaveProgramm(const SaveProgramm& in) : Operator(in) {
		int g = 0;
	}


public:

	struct NdHeader {
		char name[8] = { ' ', 'N',  'o', 'd', 'e', 's', ' ', '\0'};
		alnf version = 1.3f;
		alni first_blok = 520;
	} ndhead;

	SaveProgramm(Obj* prnt) : Operator(prnt) {
		RegisterType("Save Programm");

		ObDict& args = GETOBJ(ObDict, this, Interface);

		String* dest = new String(&args);
		args.AddObj(dest, "File Path");
	}

	virtual SaveProgramm& Instance() {
		return *new SaveProgramm(*this);
	}

	bool Poll() {
		// check paths
		return true;
	}

	void Invoke() {
		// objects cannot share non object pointers
		// each allocated object has alloc header with tmp 8b data
		
		// file layout :
		// ------------------------
		// |			header 512b 		|
		// ------------------------ 
		// |      obj size        |   
		// |	    obj base        |		<-----|
		// |		obj static dt     |					|
		// |  obj dyn alloc data  |					|
		// ------------------------					|
		// |				... 					|					|
		//																	|
		// mem layout :											|
		// ------------------------					|																		
		// |	alloc mem header    | reserved - new pointer in file adress space 
		// ------------------------ 
		// |	    obj base        |	
		// |		obj static dt     |
		// ------------------------ 
		// |				... 					|
		// -------------------------
		// |  obj dyn alloc data  |
		//--------------------------
		// |				... 					|


		// getting args
		DictObj& args = ((ObDict*)GETOBJ(Link, this, Args).GetLink())->GetDict();
		String* dest = ((String*)args.Get("File Path"));

		// opening file
		File file;
		if (file.create(dest->GetStr())) {

			// reserve for header
			file.fill(uint4(0), 513);
			file.write<NdHeader>(&ndhead, 0);

			file.adress = 512;

			clear_flags();

			// save all objects
			save_obj(&file, objs_entry);

		}

		state = OpState::FINISHED;
	}

	// returns file space adress of saved object
	static int save_obj(File* file, Obj* obj) {
		
		MemHead* allhead = MEMH_FROM_PTR(obj);

		// check if already
		alni obj_file_adress = allhead->reserved;
		if (!obj_file_adress) {

			// reserve for obj size
			file->adress += 8;

			obj_file_adress = file->adress;

			// save new pointer in file adress space
			allhead->reserved = obj_file_adress;

			uint8 saved_size = obj->save_to_file(file, obj_file_adress);

			// save size
			file->write<uint8>(&saved_size, obj_file_adress - 8);

			file->adress = obj_file_adress + saved_size;

			obj->save_oblinks(save_obj, file, obj_file_adress);
		}

		return obj_file_adress;
	}

	void clear_flags() {
		Obj* obj = objs_entry;
		while (obj) {
			MemHead* allhead = MEMH_FROM_PTR(obj);
			allhead->reserved = 0;
			obj = obj->prev;
		}
	}
};