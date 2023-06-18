#ifndef OBJECT_BASE_HPP
#define OBJECT_BASE_HPP

#include "ObjectBase.h"

namespace Object
{
	namespace Module
	{
		namespace Base
		{
			template <typename OBJECT>
			inline Data<OBJECT>::~Data()
			{

			}

			template <typename DATA>
			Func<DATA>::~Func()
			{
			}
			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				this->id = id;
				name = nullptr;
				isExists = false;
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				json["rootId"] = rootId;
				json["id"] = id;
				json["name"] = name == nullptr ? "nullptr" : name;
				json["isExists"] = isExists;

				return json;
			}
		}
	}
}

#endif