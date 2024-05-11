#ifndef OBJECT_COORD_HPP
#define OBJECT_COORD_HPP

#include "ObjectCoord.h"

namespace Object
{
	namespace Module
	{
		namespace Coord
		{
			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				data_.coord = coord;
			}
			template<typename MODULES_T>
			Json Func<MODULES_T>::print()
			{
				Json json;

				json["coord.x"] = data_.coord.x();
				json["coord.y"] = data_.coord.y();

				return json;
			}

			template<typename MODULES_T>
			inline Type::Coord Func<MODULES_T>::GetCoord() const
			{
				return data_.coord;
			}
			template<typename MODULES_T>
			inline Type::Coord Func<MODULES_T>::GetCoord(Type::Direction _direction) const
			{
				switch (_direction)
				{
					case Type::Directions::up: return GetCoordUp();
					case Type::Directions::right: return GetCoordRight();
					case Type::Directions::down: return GetCoordDown();
					case Type::Directions::left: return GetCoordLeft();
					default: return data_.coord;
				}
			}
			template<typename MODULES_T>
			inline Type::Coord Func<MODULES_T>::GetCoordLeft() const
			{
				return{data_.coord.x() - 1, data_.coord.y()};
			}
			template<typename MODULES_T>
			inline Type::Coord Func<MODULES_T>::GetCoordRight() const
			{
				return{data_.coord.x() + 1, data_.coord.y()};
			}
			template<typename MODULES_T>
			inline Type::Coord Func<MODULES_T>::GetCoordUp() const
			{
				return{data_.coord.x(), data_.coord.y() - 1};
			}
			template<typename MODULES_T>
			inline Type::Coord Func<MODULES_T>::GetCoordDown() const
			{
				return{data_.coord.x(), data_.coord.y() + 1};
			}
		}
	}
}

#endif