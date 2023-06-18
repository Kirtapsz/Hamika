#pragma once

#include "Object.h"

namespace Editor::Object
{
	namespace Back
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Drawner(Brick::Stack *stack, Type::Coord::Type x, Type::Coord::Type y, Type::Coord::Type w, Type::Coord::Type h);
	}
	namespace Front
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Drawner(Brick::Stack *stack, Type::Coord::Type x, Type::Coord::Type y, Type::Coord::Type w, Type::Coord::Type h);
	}
}
