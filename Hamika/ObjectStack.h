#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"


#define OBJECT_CREATER_PARAM Object::Brick &_brick
#define OBJECT_CREATER_CALL _brick

#define OBJECT_INITIALIZER_PARAM

#define OBJECT_UPDATE_PARAM Object::Brick &_brick, Object::Brick::UpdateType updateType
#define OBJECT_UPDATE_CALL _brick, updateType

#define OBJECT_FINALIZE_PARAM Object::Brick &_brick
#define OBJECT_FINALIZE_CALL _brick

#define OBJECT_TIMER_PARAM Object::Brick &_brick
#define OBJECT_TIMER_CALL _brick

namespace Object
{
	namespace Module
	{
		namespace Stack
		{
			template<typename MODULES_T>
			struct Data
			{
				//OBJECT SPECIFIC
				union EntityData;
				EntityData entity_data;

				unsigned char *specific{(unsigned char *)&entity_data};

#define OBJECT_PRINTER_PARAM Object::Brick &_brick
#define OBJECT_PRINTER_CALL _brick
#define OBJECT_PRINTER_RET Json
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

#include "ObjectStack.hpp"