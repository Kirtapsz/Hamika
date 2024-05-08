#pragma once

#include "Object.h"
#include "KeyboardController.h"
#include "BaseFunctions.h"

namespace Object::Entity
{
	namespace Murphy
	{
		extern const char *name;

		struct EntityData
		{
			KeyboardController *controller;
			DRAW_NUMBER_T draw_number_;

			float _effect_timer;
			Type::Flags _effect_type;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);

		void InitializerCrawlTail(OBJECT_INITIALIZER_PARAM);
		void CreateCrawlTail(OBJECT_CREATER_PARAM);
		void TimerCrawlTail(OBJECT_TIMER_PARAM);
		void UpdateCrawlTail(OBJECT_UPDATE_PARAM);
		void DrawnerCrawlTail(OBJECT_DRAWNER_PARAM);
		void simpleDrawCrawlTail(OBJECT_SIMPLE_DRAWNER_PARAM);

		void SetController(Brick &o, KeyboardController *controller);
	}
}
