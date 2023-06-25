#pragma once

#include "Object.h"

namespace Editor::Object
{
	namespace Back
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Drawner(Brick::Stack *stack, Type::Coord::base x, Type::Coord::base y, Type::Coord::base w, Type::Coord::base h);
	}
	namespace Front
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Drawner(Brick::Stack *stack, Type::Coord::base x, Type::Coord::base y, Type::Coord::base w, Type::Coord::base h);
	}
}
