#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

namespace Object
{
	namespace Module
	{
		namespace Draw
		{
			template<typename MODULES_T>
			struct Data
			{
				enum DrawType: std::uint8_t
				{
					Standard,
					Top, // Force to draw it to the top

				};
				DrawType _draw_type = DrawType::Standard;

				unsigned long long int DrawNumber = 0;
				static unsigned long long int SObjectDrawCounts;
				unsigned long long int DrawnedCount = 0;

				struct Interface
				{
					virtual Type::Pixels getBlockSize() const = 0;
				} *drawIef;

				Type::Coord DrawCoord = {0,0};

				typedef void(*SIMPLE_DRAWNER)(Type::Coord::base x, Type::Coord::base y, Type::Coord::base w, Type::Coord::base h);
				typedef void(*DRAWNER)(typename MODULES_T::BRICK_T &_brick, Type::Coord::base x, Type::Coord::base y, Type::Coord::base w, Type::Coord::base h);
				DRAWNER drawnerFnc = nullptr;
				unsigned long long int drawCounter = 0;//ez az objektum hányszor lett eddig újrarajzolva
				unsigned long long int drawAtOnceCounter = 0;//ez az objektum hányszor lett újrarajzolva egy ciklus alatt
				unsigned long long int drawCounterID = 0;//az objektum hányadikként lett rajzolva az összesbõl
				static unsigned long long int totalDrawCounter;//az összes objektum eddigi újrarajzolása
			};

			template<typename MODULES_T>
			struct Func
			{
				private: typename MODULES_T::DATA_T &data_;
				private: typename MODULES_T::FUNC_T &func_;
				public: Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func);

				public: void __init__(Type::ID id, Type::Coord coord);
				public: Json print() const;

				public: void configureDrawOptions();

				public: void Draw(const Type::Coord &_coord, const Type::Coord &_size);
				public: void setOddDrawCoord();
				public: void setRoundDrawCoord();
			};
		}
	}
}

#include "ObjectDraw.hpp"