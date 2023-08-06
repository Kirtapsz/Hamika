#ifndef OBJECT_COORD_HPP
#define OBJECT_COORD_HPP

#include "ObjectCoord.h"

namespace Object
{
	namespace Module
	{
		namespace Coord
		{
			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				this->coord = coord;
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				json["coord.x"] = coord.x();
				json["coord.y"] = coord.y();

				return json;
			}

			template <typename DATA>
			Type::Coord Func<DATA>::GetCoord() const
			{
				return coord;
			}
			template <typename DATA>
			Type::Coord Func<DATA>::GetCoord(Type::Direction _direction) const
			{
				switch (_direction)
				{
					case Type::Directions::up: return GetCoordUp();
					case Type::Directions::right: return GetCoordRight();
					case Type::Directions::down: return GetCoordDown();
					case Type::Directions::left: return GetCoordLeft();
					default: return coord;
				}
			}
			template <typename DATA>
			Type::Coord Func<DATA>::GetCoordLeft() const
			{
				return{coord.x() - 1,coord.y()};
			}
			template <typename DATA>
			Type::Coord Func<DATA>::GetCoordRight() const
			{
				return{coord.x() + 1,coord.y()};
			}
			template <typename DATA>
			Type::Coord Func<DATA>::GetCoordUp() const
			{
				return{coord.x(),coord.y() - 1};
			}
			template <typename DATA>
			Type::Coord Func<DATA>::GetCoordDown() const
			{
				return{coord.x(),coord.y() + 1};
			}
		}
	}
}

#endif