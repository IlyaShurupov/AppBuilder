#pragma once

#ifdef _DEBUG
#define MEM_DEBUG 
#endif // _DEBUG


typedef long long byte_size;

#ifdef MEM_DEBUG

#include <crtdbg.h>


void* Alloc(byte_size size);
void Free(void* ptr);
void LogHeap();


#define FREE(ptr) Free((void*)ptr);

#define NEW_DBG(Type) new((Type*)Alloc(sizeof(Type)))
#define DELETE_DBG(Type, ptr) ptr->~Type(); Free((void*)ptr)

#define NEW_DBG_AR(Type, len) (Type*)Alloc(sizeof(Type) * (len))
#define DELETE_DBG_AR(ptr) Free((void*)ptr)


#define _CRTDBG_MAP_ALLOC
/*
#define ALLOC_DEF(Type, ptr) Type* ptr = (Type*)Alloc(sizeof(Type))
#define ALLOC_DEF_AR(Type, len, ptr) Type* ptr = (Type*)Alloc(sizeof(Type) * (len))
#define ALLOC_A(Type, ptr) ptr = (Type*)Alloc(sizeof(Type))
#define ALLOC(Type) (Type*)Alloc(sizeof(Type))
#define ALLOC_AR(Type, len, ptr) ptr = (Type*)Alloc(sizeof(Type) * (len))
*/

#else


#define NEW_DBG(Type) new
#define DELETE_DBG(Type, ptr) delete ptr

#define NEW_DBG_AR(Type, len) new Type[len]
#define DELETE_DBG_AR(ptr) delete[] ptr

#endif
