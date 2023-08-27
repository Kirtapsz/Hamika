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
				//LÉTRHOZÁS #####################################################################################
#define OBJECT_CREATER_PARAM Object::Brick::Stack *stack
#define OBJECT_CREATER_CALL stack
				typedef void(*CREATER)(Stack *);

				//INICIALIZÁLÓ
#define OBJECT_INITIALIZER_PARAM
				typedef void(*INITIALIZER)(OBJECT_INITIALIZER_PARAM);

				//UPDATE #####################################################################################
				enum UpdateType: std::uint8_t
				{
					UPDATE_ASC = 1 << 0,
					UPDATE_DESC = 1 << 1,
					UPDATE_MURPHY = 1 << 2,
				};
#define OBJECT_UPDATE_PARAM Object::Brick::Stack *stack, Object::Brick::UpdateType updateType
#define OBJECT_UPDATE_CALL stack, updateType
				typedef void(*UPDATE)(Stack *, UpdateType);
				UPDATE updaterFnc = nullptr;
				unsigned long long updateNumber = 0;
				static unsigned long long totalUpdateNumber;

				//FINALIZE #####################################################################################
#define OBJECT_FINALIZE_PARAM Object::Brick::Stack *stack
#define OBJECT_FINALIZE_CALL stack
				typedef void(*FINALIZE)(Stack *);
				FINALIZE finalizeFnc = nullptr;


				//TIMER EVENT #####################################################################################
#define OBJECT_TIMER_PARAM Object::Brick::Stack *stack
#define OBJECT_TIMER_CALL stack
				typedef void(*TIMER)(Stack *);
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