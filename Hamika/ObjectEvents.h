#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

namespace Object
{
	namespace Module
	{
		namespace Events
		{
			template <typename OBJECT>
			struct Data
			{
				struct Events
				{
					bool timer;
					bool update;
					bool topDraw;

					void clear();

				} events = {0};
			};
			template <typename DATA>
			struct Func: virtual DATA
			{
				void __init__(Type::ID id, Type::Coord coord);
				Json print();
			};
		}
	}
}

#include "ObjectEvents.hpp"