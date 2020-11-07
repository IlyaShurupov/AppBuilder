#pragma once 

#include "LinkedList.h"
#include "Trig.h"

class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();

	List<Trig> Trigs;
};