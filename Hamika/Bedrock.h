#pragma once

#include "Object.h"

namespace Object
{
	//Bedrock
	namespace Bedrock
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		void Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Tick(OBJECT_TICK_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
	}
}