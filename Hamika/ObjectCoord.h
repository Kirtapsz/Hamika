#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

namespace Object
{
	namespace Module
	{
		namespace Coord
		{
			template <typename OBJECT>
			struct Data
			{
				Type::Coord coord = Type::Coord::Invalid;
			};
			template <typename DATA>
			struct Func: virtual DATA
			{
				void __init__(Type::ID id, Type::Coord coord);
				Json print();


				Type::Coord GetCoord() const;
				Type::Coord GetCoord(Type::Direction _direction) const;
				Type::Coord GetCoordLeft() const;
				Type::Coord GetCoordRight() const;
				Type::Coord GetCoordUp() const;
				Type::Coord GetCoordDown() const;
			};
		}
	}
}

#include "ObjectCoord.hpp"