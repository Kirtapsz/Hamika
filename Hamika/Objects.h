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

void ObjectCreate(Object::Brick *object, Type::ID id, Type::Coord coord, Object::Brick::CREATER creater = nullptr);
inline void ObjectCreate(Object::Brick::Interface *scene, Object::Brick *object, Type::ID id, Type::Coord coord, Object::Brick::CREATER creater = nullptr)
{
	object->setScene(scene);
	ObjectCreate(object, id, coord, creater);
}

namespace Editor
{
	void ObjectCreate(Object::Brick *object, Type::ID id, Type::Coord coord, Object::Brick::CREATER creater = nullptr);
	inline void ObjectCreate(Object::Brick::Interface *scene, Object::Brick *object, Type::ID id, Type::Coord coord, Object::Brick::CREATER creater = nullptr)
	{
		object->setScene(scene);
		ObjectCreate(object, id, coord, creater);
	}
}