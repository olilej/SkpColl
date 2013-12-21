#pragma once

#include "Wm5Matrix4.h"
#include "Wm5SmartPointer.h"

typedef std::vector<Wm5::Vector4f*> vectorP_; //std::vector<Wm5::Pointer0<Wm5::Vector4f>>
typedef Wm5::Matrix4f Matrix4_; //Wm5::Pointer0<Wm5::Matrix4f>

class EntityInfo
{
public:
	EntityInfo(void);
	~EntityInfo(void);
	EntityInfo(const EntityInfo& val);
	EntityInfo& operator = (const EntityInfo& val);  

	int entityID;
	Matrix4_* m;
	vectorP_ points0;
	enum EntityType { OTHER, FACE, GROUP, COMPONENT } entityType;
};

