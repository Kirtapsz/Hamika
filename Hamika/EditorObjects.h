#pragma once

#include "Object.h"

namespace Object::Editor
{
	namespace Object
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Drawner(EditorObjectBase::Stack *stack, Type::Coord::Type x, Type::Coord::Type y, Type::Coord::Type w, Type::Coord::Type h);
	}
	namespace Remain
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Drawner(EditorObjectBase::Stack *stack, Type::Coord::Type x, Type::Coord::Type y, Type::Coord::Type w, Type::Coord::Type h);
	}
}
