#pragma once

#include "Object.h"

namespace Object
{
	//Editor object
	namespace EditorObject
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Drawner(EditorObjectBase::Stack *stack, Type::Coord::Type x, Type::Coord::Type y, Type::Coord::Type w, Type::Coord::Type h);
	}
	//Editor remain
	namespace EditorRemain
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Drawner(EditorObjectBase::Stack *stack, Type::Coord::Type x, Type::Coord::Type y, Type::Coord::Type w, Type::Coord::Type h);
	}
}