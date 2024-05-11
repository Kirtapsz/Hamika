#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

namespace Object
{
	namespace Module
	{
		namespace Execute
		{
			template<typename MODULES_T>
			struct Data
			{
				//LÉTRHOZÁS #####################################################################################

				typedef void(*CREATER)(typename MODULES_T::BRICK_T &_brick);

				//INICIALIZÁLÓ
				typedef void(*INITIALIZER)();

				//UPDATE #####################################################################################
				typedef void(*UPDATE)(typename MODULES_T::BRICK_T &_brick, UpdateType);
				UPDATE updaterFnc = nullptr;
				unsigned long long updateNumber = 0;
				static unsigned long long totalUpdateNumber;

				//FINALIZE #####################################################################################
				typedef void(*FINALIZE)(typename MODULES_T::BRICK_T &_brick);
				FINALIZE finalizeFnc = nullptr;

				//TIMER EVENT #####################################################################################
				typedef void(*TIMER)(typename MODULES_T::BRICK_T &_brick);
				TIMER timerFnc = nullptr;
			};

			template<typename MODULES_T>
			struct Func
			{
				private: typename MODULES_T::DATA_T &data_;
				private: typename MODULES_T::FUNC_T &func_;
				public: Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func);

				public: void __init__(Type::ID id, Type::Coord coord);
				public: Json print();

				public: void RunUpdate(UpdateType updateType);
				public: void RunFinalize();
				public: void RunTimer();
			};
		}
	}
}

#include "ObjectExecute.hpp"