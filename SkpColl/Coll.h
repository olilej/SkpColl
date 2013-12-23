#pragma once

#include <string>
#include "EntityInfo.h"
#include "simple_tree.h"
#include "ruby.h"

typedef tree<EntityInfo*> TreeEntity; //tree<Wm5::Pointer0<EntityInfo>>

class Coll
{
public:
    Coll();
    virtual ~Coll(void);

	static void Init_SkpCol();
	static Coll* GetColl(VALUE rColl);

	void LoadModel(VALUE);
	void Reset();

private:
	TreeEntity EntityTree;
	void LoadEntities(VALUE entities, Matrix4_*, const TreeEntity::pre_order_iterator&, bool root = false);
};




#ifdef ENABLE_TRACE
#  ifdef _MSC_VER
#    include <windows.h>
#    include <sstream>
#    define TRACE(x)                           \
     do {  std::stringstream s;  s << (x);     \
           OutputDebugStringA(s.str().c_str()); \
        } while(0)
#  else
#    include <iostream>
#    define TRACE(x)  std::clog << (x)
#  endif        // or std::cerr << (x) << std::flush
#else
#  define TRACE(x)
#endif