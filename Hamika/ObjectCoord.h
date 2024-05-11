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
			struct Data
			{
				Type::Coord coord = Type::Coord::Invalid;
			};

			template<typename MODULES_T>
			struct Func
			{
				private: typename MODULES_T::DATA_T &data_;
				private: typename MODULES_T::FUNC_T &func_;
				public: Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func);

				public: void __init__(Type::ID id, Type::Coord coord);
				public: Json print();

				public: Type::Coord GetCoord() const;
				public: Type::Coord GetCoord(Type::Direction _direction) const;
				public: Type::Coord GetCoordLeft() const;
				public: Type::Coord GetCoordRight() const;
				public: Type::Coord GetCoordUp() const;
				public: Type::Coord GetCoordDown() const;
			};
		}
	}
}

#include "ObjectCoord.hpp"