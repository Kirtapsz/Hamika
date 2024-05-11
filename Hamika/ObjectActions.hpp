#ifndef OBJECT_ACTIONS_HPP
#define OBJECT_ACTIONS_HPP

#include "ObjectActions.h"

namespace Object
{
	namespace Module
	{
		namespace Actions
		{
			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				data_.action = MODULES_T::DATA_T::STEADY;
			}
			template<typename MODULES_T>
			Json Func<MODULES_T>::print()
			{
				Json json;

				json["action"] = data_.action;

				return json;
			}

			template<typename MODULES_T>
			inline bool Func<MODULES_T>::isAction(typename MODULES_T::DATA_T::ACTION_T _action) const
			{
				return data_.action == _action;
			}
			template<typename MODULES_T>
			inline bool Func<MODULES_T>::isAction() const
			{
				return data_.action != MODULES_T::DATA_T::STEADY;
			}
			template<typename MODULES_T>
			inline bool Func<MODULES_T>::isActionMove() const
			{
				return data_.action & MODULES_T::DATA_T::MOVE;
			}
			template<typename MODULES_T>
			inline bool Func<MODULES_T>::isActionRotate() const
			{
				return data_.action & MODULES_T::DATA_T::ROTATE;
			}
		}
	}
}

#endif