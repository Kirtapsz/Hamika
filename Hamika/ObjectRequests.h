#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

#include "ObjectHitAction.h"

namespace Object
{
	namespace Module
	{
		namespace Requests
		{
			template <typename OBJECT>
			struct Data
			{
				struct Requests
				{
					bool timer;
					bool tick;
					bool update;
					bool draw;
					bool remove;

					void clear();

				} requests = {0};
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

#include "ObjectRequests.hpp"