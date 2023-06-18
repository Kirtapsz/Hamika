#ifndef OBJECT_FLAGS_HPP
#define OBJECT_FLAGS_HPP

#include "ObjectFlags.h"

namespace Object
{
	namespace Module
	{
		namespace Flags
		{
			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				flags = 0;
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				json["flags"] = flags;

				return json;
			}


			template <typename DATA>
			void Func<DATA>::SetFlags(Type::Flags flags_)
			{
				flags = flags_;
			}
			template <typename DATA>
			Type::Flags Func<DATA>::GetFlags()
			{
				return flags;
			}
			template <typename DATA>
			void Func<DATA>::AddFlags(Type::Flags flags_)
			{
				flags |= flags_;
			}
			template <typename DATA>
			void Func<DATA>::RemoveFlags(Type::Flags flags_)
			{
				flags &= ~flags_;
			}
		}
	}
}

#endif