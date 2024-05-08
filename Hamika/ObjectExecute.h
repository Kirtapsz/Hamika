#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

#include "ObjectStack.h"
#include "ObjectRequests.h"

namespace Object
{
	namespace Module
	{
		namespace Execute
		{
			template <typename OBJECT>
			struct Data: virtual Stack::Data<OBJECT>
			{
				//L�TRHOZ�S #####################################################################################

				typedef void(*CREATER)(OBJECT &_object);

				//INICIALIZ�L�
				typedef void(*INITIALIZER)(OBJECT_INITIALIZER_PARAM);

				//UPDATE #####################################################################################
				enum UpdateType: std::uint8_t
				{
					UPDATE_ASC = 1 << 0,
					UPDATE_DESC = 1 << 1,
					UPDATE_MURPHY = 1 << 2,
				};
				typedef void(*UPDATE)(OBJECT &_object, UpdateType);
				UPDATE updaterFnc = nullptr;
				unsigned long long updateNumber = 0;
				static unsigned long long totalUpdateNumber;

				//FINALIZE #####################################################################################
				typedef void(*FINALIZE)(OBJECT &_object);
				FINALIZE finalizeFnc = nullptr;

				//TIMER EVENT #####################################################################################
				typedef void(*TIMER)(OBJECT &_object);
				TIMER timerFnc = nullptr;
			};

			template <typename DATA>
			struct Func: virtual DATA
			{
				void __init__(Type::ID id, Type::Coord coord);
				Json print();

				void RunUpdate(typename DATA::UpdateType updateType);
				void RunFinalize();
				void RunTimer();
			};
		}
	}
}

#include "ObjectExecute.hpp"