#include "stdafx.h"
#include "EntityInfo.h"

EntityInfo::EntityInfo(void)
{
	m = NULL;
	entityID = 0;
}

EntityInfo::~EntityInfo(void)
{
	if (m)
		delete m;

	if (points0.size() > 0)
	{
		for (size_t i = 0; i < points0.size(); i++)
			delete points0[i];
		points0.clear();
	}
}

EntityInfo::EntityInfo(const EntityInfo& val)
{
	*this = val;
}

EntityInfo& EntityInfo::operator=(const EntityInfo& val)
{
	if(&val != this)
	{
		entityID = val.entityID;

		if (m)
		{
			delete m;
			m = new Matrix4_(*val.m);
		}

		if (points0.size() > 0)
		{
			for (size_t i = 0; i < points0.size(); i++)
				delete points0[i];
			points0.clear();
		}
		points0.resize(val.points0.size());
		for (size_t i = 0; i < val.points0.size(); i++)
		{
			points0.push_back(new Wm5::Vector4f(*val.points0[i]));
		}
	}
	return *this;
}