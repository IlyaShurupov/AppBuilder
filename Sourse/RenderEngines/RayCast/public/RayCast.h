#pragma once
#include "FrameBuff.h"
#include "Object.h"
#include "LinkedList.h"

namespace RayCast {

	class RenderSettings
	{
	public:
		PropertyFloat Resolution;

	public:
		RenderSettings();
		~RenderSettings();
		void setObjList(List<Object>* list);
		void setCamera(Object* camera);
		Object* getCamera();
		List<Object>* getObjList();

	private:
		List<Object>* ObjList;
		Object* Camera;
	};

	void RenderToBuff(RenderSettings* settings, FBuff<RGBA_32>* Buff);
}
