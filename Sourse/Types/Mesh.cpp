#include "public/Mesh.h"

StaticMesh::StaticMesh() {
}


StaticMesh::~StaticMesh() {
	Trigs.del();
}