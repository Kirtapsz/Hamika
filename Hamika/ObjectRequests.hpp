#ifndef OBJECT_REQUESTS_HPP
#define OBJECT_REQUESTS_HPP

#include "ObjectRequests.h"

namespace Object
{
	namespace Module
	{
		namespace Requests
		{
			inline void Data::Requests::clear()
			{
				timer = false;
				tick = false;
				update = false;
				draw = false;
				remove = false;
			}

			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				data_.requests.clear();
			}
			template<typename MODULES_T>
			Json Func<MODULES_T>::print()
			{
				Json json;

				json["requests.timer"] = data_.requests.timer;
				json["requests.tick"] = data_.requests.tick;
				json["requests.update"] = data_.requests.update;
				json["requests.draw"] = data_.requests.draw;
				json["requests.remove"] = data_.requests.remove;

				return json;
			}
		}
	}
}

#endif