#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

namespace Object
{
	namespace Module
	{
		namespace Base
		{
			template <typename OBJECT>
			struct Data
			{
				typedef OBJECT OBJECT_T;
				Type::ID rootId = Type::INVALID_ID;
				Type::ID id = Type::INVALID_ID;
				const char *name = nullptr;
				bool isExists = false;

				virtual ~Data();
			};
			template <typename DATA>
			struct Func: virtual DATA
			{
				virtual ~Func();
				void __init__(Type::ID id, Type::Coord coord);
				Json print();
			};
		}
	}
}

#include "ObjectBase.hpp"