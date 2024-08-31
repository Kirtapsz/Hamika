#ifndef OBJECT_EVENTS_HPP
#define OBJECT_EVENTS_HPP

#include "ObjectEvents.h"

namespace Object
{
	namespace Module
	{
		namespace Events
		{
			inline void Data::Events::clear()
			{
				timer = false;
				update = false;
			}

			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}
			template<typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				data_.events.clear();
			}
			template<typename MODULES_T>
			Json Func<MODULES_T>::print()
			{
				Json json;

				json["events.timer"] = data_.events.timer;
				json["events.update"] = data_.events.update;

				return json;
			}

		};
	}
}

#endif