#ifndef OBJECT_BASE_HPP
#define OBJECT_BASE_HPP

#include "ObjectBase.h"

namespace Object
{
	namespace Module
	{
		namespace Base
		{
			template<typename MODULES_T>
			inline void Data<MODULES_T>::setScene(typename MODULES_T::SCENE_INTERFACE_T *scene_)
			{
				scene = scene_;
			}


			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				data_.id = id;
				data_.name = nullptr;
				data_.isExists = false;
			}
			template<typename MODULES_T>
			Json Func<MODULES_T>::print()
			{
				Json json;

				json["rootId"] = data_.rootId;
				json["id"] = data_.id;
				json["name"] = data_.name == nullptr ? "nullptr" : data_.name;
				json["isExists"] = data_.isExists;

				return json;
			}
		}
	}
}

#endif