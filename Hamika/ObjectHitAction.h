#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

namespace Object
{
	namespace Module
	{
		namespace HitAction
		{
			template <typename OBJECT>
			struct Data
			{
				Type::Coord hitCoord = {0,0};
			};
			template <typename DATA>
			struct Func: virtual DATA
			{
				void __init__(Type::ID id, Type::Coord coord);
				Json print();

				void SetHitCoord(Type::Coord coord_);
				Type::Coord GetHitCoord() const;
			};
		}
	}
}

#include "ObjectHitAction.hpp"