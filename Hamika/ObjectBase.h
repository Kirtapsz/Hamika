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
			template<typename MODULES_T>
			struct Data
			{
				Type::ID rootId = Type::INVALID_ID;
				Type::ID id = Type::INVALID_ID;
				const char *name = nullptr;
				bool isExists = false;

				typename MODULES_T::SCENE_INTERFACE_T *scene = nullptr;
				void setScene(typename MODULES_T::SCENE_INTERFACE_T *scene_);
			};

			template<typename MODULES_T>
			struct Func
			{
				private: typename MODULES_T::DATA_T &data_;
				private: typename MODULES_T::FUNC_T &func_;
				public: Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func);

				public: void __init__(Type::ID id, Type::Coord coord);
				public: Json print();
			};
		}
	}
}

#include "ObjectBase.hpp"