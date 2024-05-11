#ifndef OBJECT_STACK_HPP
#define OBJECT_STACK_HPP

#include "ObjectStack.h"
#include "EntityData.hpp"

namespace Object
{
	namespace Module
	{
		namespace Stack
		{
			template <typename MODULES_T>
			Data<MODULES_T>::~Data()
			{

			}

			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}
			template <typename MODULES_T>
			inline Func<MODULES_T>::~Func()
			{

			}
			template <typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				memset(data_.specific, 0, sizeof(data_.specific));
			}
			template <typename MODULES_T>
			inline Json Func<MODULES_T>::print()
			{
				if (data_.printFnc)
				{
					typename MODULES_T::BRICK_T *object = static_cast<typename MODULES_T::BRICK_T *>(this);
					return data_.printFnc(*object);
				}
				else
				{
					return Json();
				}
			}
		}
	}
}

#endif