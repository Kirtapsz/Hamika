#ifndef OBJECT_EVENTS_HPP
#define OBJECT_EVENTS_HPP

#include "ObjectEvents.h"

namespace Object
{
	namespace Module
	{
		namespace Events
		{
			template <typename OBJECT>
			inline void Data<OBJECT>::Events::clear()
			{
				timer = false;
				update = false;
			}

			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				json["events.timer"] = events.timer;
				json["events.update"] = events.update;
				json["events.topDraw"] = events.topDraw;

				return json;
			}

		};
	}
}

#endif