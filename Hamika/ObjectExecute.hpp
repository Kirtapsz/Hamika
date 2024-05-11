#ifndef OBJECT_EXECUTE_HPP
#define OBJECT_EXECUTE_HPP

#include "ObjectExecute.h"

namespace Object
{
	namespace Module
	{
		namespace Execute
		{
			template<typename MODULES_T>
			unsigned long long Data<MODULES_T>::totalUpdateNumber{0};

			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}

			template <typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				data_.printFnc = nullptr;
				data_.updaterFnc = nullptr;
				data_.finalizeFnc = nullptr;
				data_.timerFnc = nullptr;
			}
			template <typename MODULES_T>
			Json Func<MODULES_T>::print()
			{
				Json json;

				return json;
			}

			template <typename MODULES_T>
			inline void Func<MODULES_T>::RunUpdate(typename Data<MODULES_T>::UpdateType updateType)
			{
				data_.requests.update = false;
				data_.updateNumber = data_.totalUpdateNumber++;

				if (data_.updaterFnc)
				{
					typename MODULES_T::BRICK_T *object = static_cast<typename MODULES_T::BRICK_T *>(this);
					data_.updaterFnc(*object, updateType);
				}
			}

			template <typename MODULES_T>
			inline void Func<MODULES_T>::RunFinalize()
			{
				data_.requests.finalize = false;
				if (data_.finalizeFnc)
				{
					typename MODULES_T::BRICK_T *object = static_cast<typename MODULES_T::BRICK_T *>(this);
					data_.finalizeFnc(*object);
				}
			}

			template <typename MODULES_T>
			inline void Func<MODULES_T>::RunTimer()
			{
				data_.requests.timer = false;
				if (data_.timerFnc)
				{
					typename MODULES_T::BRICK_T *object = static_cast<typename MODULES_T::BRICK_T *>(this);
					data_.timerFnc(*object);
				}
			}
		}
	}
}

#endif