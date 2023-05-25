#pragma once

#include <KIR\AL\KIR5_bitmap.h>
#include <KIR\AL\KIR5_font.h>
#include <KIR\KIR4_string.h>

#include "Object.h"


#include "Bedrock.h"
#include "Space.h"
#include "Types.h"


namespace Objects
{
	void RunInitializer();
}

__int32 GetObjectNumber();

void DrawObject(int id, int x, int y, int w, int h);

const char *GetObjectName(int id);
void ObjectCreate(ObjectBase *object, Type::ID id, Type::Coord coord, ObjectBase::CREATER creater = nullptr);
inline void ObjectCreate(ObjectBase::Interface *scene, ObjectBase *object, Type::ID id, Type::Coord coord, ObjectBase::CREATER creater = nullptr)
{
	object->setIef(scene);
	ObjectCreate(object, id, coord, creater);
}
void ObjectCreate(EditorObjectBase *object, Type::ID id, Type::Coord coord, EditorObjectBase::CREATER creater = nullptr);
inline void ObjectCreate(EditorObjectBase::Interface *scene, EditorObjectBase *object, Type::ID id, Type::Coord coord, EditorObjectBase::CREATER creater = nullptr)
{
	object->setIef(scene);
	ObjectCreate(object, id, coord, creater);
}