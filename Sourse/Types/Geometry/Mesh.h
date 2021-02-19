#pragma once 

#include "Containers/LinkedList.h"
#include "Trig.h"

class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();

	List<Trig> Trigs;
};