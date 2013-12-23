#include "StdAfx.h"
#include "Coll.h"
#include "RubyUtils.h"
#include <sstream>
#include <ctime>
#include "Wm5Matrix4.h"


static VALUE rCollClass;
static VALUE _wrap_load_model(VALUE, VALUE);
static VALUE allocate(VALUE self);
static void deallocate(void* ptr);

Coll::Coll()
{
	EntityTree.set_head(NULL);
}

Coll::~Coll(void)
{
	//Reset();
}

void Coll::Reset(void)
{
	TreeEntity::post_order_iterator it2 = EntityTree.begin_post();
	TreeEntity::post_order_iterator end2 = EntityTree.end_post();
	int rootdepth = EntityTree.depth(it2);
	while(it2 != end2)
	{
		for(int i = 0; i <= EntityTree.depth(it2)-rootdepth; ++i) 
			if (it2.node->data)
				delete it2.node->data;
		++it2;
	}
}
;
Coll* Coll::GetColl(VALUE rColl)
{
    Coll* pExp = 0;
    if( TYPE(rColl) != T_DATA || !rb_obj_is_kind_of(rColl, rCollClass) )
    {
        rb_raise(rb_eArgError, "wrong type - expected Coll");
        return NULL;
    }
    return (Coll*)DATA_PTR(rColl);
}

void Coll::LoadModel(VALUE model)
{
	VALUE entities = rb_funcall(model, rb_intern("entities"), 0);
	LoadEntities(entities, NULL, EntityTree.begin(), true);
}

void Coll::LoadEntities(VALUE entities, Matrix4_* tr, const TreeEntity::pre_order_iterator& itparent, bool root)
{
	if (entities == Qnil)
		return;
	VALUE entities_a = rb_funcall(entities, rb_intern("to_a"), 0);
	UINT arr_size = RARRAY_LEN(entities_a);
	VALUE entity;
	VALUE entityID;
	EntityInfo* entityInfo;
	TreeEntity::pre_order_iterator child;
	VALUE trans;
	VALUE trans_a;
	Matrix4_ m;
	VALUE hidden;
	double test;
	VALUE compo_definition;
	VALUE compo_entities;
	VALUE group_entities;
	VALUE vertices;
	UINT num_vertices;
	VALUE vertex;
	VALUE pt;
	Wm5::Vector4f* result1;

	for (UINT i = 0; i < arr_size; i++)
	{
		entity = rb_ary_entry(entities_a, i);
		if (CRubyUtils::IsEdge(entity))
			continue;
		
		bool IsCompo = CRubyUtils::IsComponentInstance(entity);
		bool IsGroup = CRubyUtils::IsGroup(entity);
		bool IsFace = CRubyUtils::IsFace(entity);
		if (!(IsCompo || IsGroup || IsFace))
			continue;

		entityInfo = new EntityInfo();
		entityID = rb_funcall(entity, rb_intern("entityID"), 0);
		entityInfo->entityID = NUM2INT(entityID);
		if (!root)
			child = EntityTree.append_child(itparent, entityInfo);
		else
			child = EntityTree.insert(itparent, entityInfo);

		if (IsCompo || IsGroup)
		{
			trans = rb_funcall(entity, rb_intern("transformation"), 0);
			trans_a = rb_funcall(trans, rb_intern("to_a"), 0);
			arr_size = RARRAY_LEN(trans_a);

			for (UINT j = 0; j < arr_size; j++)
			{
				VALUE t = rb_ary_entry(trans_a, j);
				m(j % 4, (int)(j/4)) = (float)NUM2DBL(t);
			}	

			if (tr)
				entityInfo->m = new Matrix4_(*tr * m);
			else
				entityInfo->m = new Matrix4_(m);

			if (IsCompo)
			{
				hidden = CRubyUtils::GetDAttribute(entity, rb_str_new2("hidden"));
				if (hidden != Qnil)
				{
					test  = RFLOAT(hidden)->value;
					if (test == 1.0)
						continue;			
				}
				entityInfo->entityType = EntityInfo::COMPONENT;
				compo_definition = rb_funcall(entity, rb_intern("definition"), 0);
				compo_entities = rb_funcall(compo_definition, rb_intern("entities"), 0);
				LoadEntities(compo_entities, entityInfo->m, child, false);
			}
			else if (IsGroup)
			{
				entityInfo->entityType = EntityInfo::GROUP;
				group_entities = rb_funcall(entity, rb_intern("entities"), 0);
				LoadEntities(group_entities, entityInfo->m, child, false);
			}
		}
		else if (IsFace)
		{
			entityInfo->entityType = EntityInfo::FACE;

			vertices = rb_funcall(entity, rb_intern("vertices"), 0);
			num_vertices = RARRAY_LEN(vertices);

			result1 = new Wm5::Vector4f[num_vertices];
			entityInfo->points0.reserve(num_vertices);
			for (UINT k = 0; k < num_vertices; k++)
			{
				vertex = rb_ary_entry(vertices, k);
				pt = rb_funcall(vertex, rb_intern("position"), 0);
				result1[k][0] = (float)NUM2DBL( rb_funcall(pt, rb_intern("x"), 0) );
				result1[k][1] = (float)NUM2DBL( rb_funcall(pt, rb_intern("y"), 0) );
				result1[k][2] = (float)NUM2DBL( rb_funcall(pt, rb_intern("z"), 0) );
				result1[k][3] = 1.0;
				entityInfo->points0.push_back(&result1[k]);
			}
		}
	}
}

void Coll::Init_SkpCol()
{
    VALUE rModule = rb_define_module("CColl");
	rCollClass = rb_define_class_under(rModule, "Coll", rb_cObject);
	rb_define_alloc_func(rCollClass, allocate);
	rb_define_method(rCollClass, "load_model", VALUEFUNC(_wrap_load_model), 1);
}

static VALUE allocate(VALUE klass)
{
  Coll* pAnc = new Coll();
  return Data_Wrap_Struct(klass, NULL, deallocate, pAnc);
}

static void deallocate(void* ptr)
{
    Coll* pAnc = (Coll*)ptr;
	delete pAnc;
}

static VALUE _wrap_load_model(VALUE self, VALUE model)
{
	std::clock_t t = std::clock();
	Coll* pColl = Coll::GetColl(self);
	pColl->Reset();
	pColl->LoadModel(model);
	double delta = ((std::clock() - t) / (double) CLOCKS_PER_SEC) * 1000.0;
	return LONG2NUM(delta);
}
