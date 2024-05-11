#ifndef OBJECT_FLAGS_HPP
#define OBJECT_FLAGS_HPP

#include "ObjectFlags.h"

namespace Object
{
	namespace Module
	{
		namespace Flags
		{
			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				data_.flags = 0;
			}
			template<typename MODULES_T>
			Json Func<MODULES_T>::print()
			{
				Json json;

				json["flags"] = data_.flags;

				return json;
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::SetFlags(Type::Flags flags_)
			{
				data_.flags = flags_;
			}
			template<typename MODULES_T>
			inline Type::Flags Func<MODULES_T>::GetFlags()
			{
				return data_.flags;
			}
			template<typename MODULES_T>
			inline void Func<MODULES_T>::AddFlags(Type::Flags flags_)
			{
				data_.flags |= flags_;
			}
			template<typename MODULES_T>
			inline void Func<MODULES_T>::RemoveFlags(Type::Flags flags_)
			{
				data_.flags &= ~flags_;
			}
		}
	}
}

#endif