#ifndef OBJECT_HITACTION_HPP
#define OBJECT_HITACTION_HPP

#include "ObjectHitAction.h"

namespace Object
{
	namespace Module
	{
		namespace HitAction
		{
			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				hitCoord = coord;
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				json["hitCoord.x"] = hitCoord.x;
				json["hitCoord.y"] = hitCoord.y;

				return json;
			}

			template <typename DATA>
			void Func<DATA>::SetHitCoord(Type::Coord coord_)
			{
				hitCoord = coord_;
			}
			template <typename DATA>
			Type::Coord Func<DATA>::GetHitCoord() const
			{
				return hitCoord;
			}
		}
	}
}

#endif