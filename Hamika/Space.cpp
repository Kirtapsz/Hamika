#include "Space.h"

namespace Object::Entity
{
	//Space
	namespace Space
	{
		const char *name = "EmptySpace";

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::StepOn | Brick::MurphyStepOn | Brick::CanBeExploded);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
		}
	}
}