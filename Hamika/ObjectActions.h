#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

namespace Object
{
	namespace Module
	{
		namespace Actions
		{
			struct Data
			{
				typedef std::uint16_t ACTION_T;

				constexpr static ACTION_T STEADY = 0;
				enum: ACTION_T
				{
					MOVE = 1 << 0,
					MOVE_RIGHT = (1 << 1) | MOVE,
					MOVE_LEFT = (1 << 2) | MOVE,
					MOVE_UP = (1 << 3) | MOVE,
					MOVE_DOWN = (1 << 4) | MOVE,

					ROTATE = 1 << 5,
					ROTATE_LEFT = (1 << 6) | ROTATE,
					ROTATE_RIGHT = (1 << 7) | ROTATE,

					//action linked to an other object
					LINKED = 1 << 8,
				};

				ACTION_T action = STEADY;

				constexpr static ACTION_T ACTION_MOVE[4] = {MOVE_UP, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT};
			};

			template<typename MODULES_T>
			struct Func
			{
				private: typename MODULES_T::DATA_T &data_;
				private: typename MODULES_T::FUNC_T &func_;
				public: Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func);

				public: void __init__(Type::ID id, Type::Coord coord);
				public: Json print();

				public: bool isAction(typename MODULES_T::DATA_T::ACTION_T _action) const;
				public: bool isAction() const;

				public: bool isActionMove() const;
				public: bool isActionRotate() const;
			};
		}
	}
}

#include "ObjectActions.hpp"