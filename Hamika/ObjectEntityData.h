#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

namespace Object
{
	namespace Module
	{
		namespace EntityData
		{
			template<typename MODULES_T>
			struct Data
			{
				union EntityData;
				EntityData entity_data;

				typedef Json(*PRINT)(typename MODULES_T::BRICK_T &_brick);
				PRINT printFnc = nullptr;

				virtual ~Data();
			};

			template<typename MODULES_T>
			struct Func
			{
				private: typename MODULES_T::DATA_T &data_;
				private: typename MODULES_T::FUNC_T &func_;
				public: Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func);

				public: virtual ~Func();
				public: void __init__(Type::ID id, Type::Coord coord);
				public: Json print();
			};
		}
	}
}

#include "ObjectEntityData.hpp"