#pragma once 

#include "Containers/List.h"
#include "Trig.h"

class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();

	List<Trig> Trigs;
};