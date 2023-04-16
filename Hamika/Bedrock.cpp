#include "Bedrock.h"

namespace Object
{
	//Bedrock
	namespace Bedrock
	{
		const char *name = "Bedrock";

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
		}
		void Create(OBJECT_CREATER_PARAM)
		{
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