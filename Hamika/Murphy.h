#pragma once

#include "Object.h"

namespace Object
{
	namespace Murphy
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		void CreatePassage(OBJECT_CREATER_PARAM);
		void Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Tick(OBJECT_TICK_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);


		void Controll(ObjectBase::Stack *stack);

		void SetSpell(ObjectBase *o, bool Spell);
		void SetMoveUp(ObjectBase *o, bool MoveUp);
		void SetMoveDown(ObjectBase *o, bool MoveDown);
		void SetMoveLeft(ObjectBase *o, bool MoveLeft);
		void SetMoveRight(ObjectBase *o, bool MoveRight);
	}
}