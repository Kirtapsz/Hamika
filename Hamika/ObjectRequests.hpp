#ifndef OBJECT_REQUESTS_HPP
#define OBJECT_REQUESTS_HPP

#include "ObjectRequests.h"

namespace Object
{
	namespace Module
	{
		namespace Requests
		{
			template <typename OBJECT>
			void Data<OBJECT>::Requests::clear()
			{
				timer = false;
				tick = false;
				update = false;
				draw = false;
				remove = false;
			}

			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				requests.clear();
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				json["requests.timer"] = requests.timer;
				json["requests.tick"] = requests.tick;
				json["requests.update"] = requests.update;
				json["requests.draw"] = requests.draw;
				json["requests.remove"] = requests.remove;

				return json;
			}
		}
	}
}

#endif