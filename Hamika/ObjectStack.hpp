#ifndef OBJECT_STACK_HPP
#define OBJECT_STACK_HPP

#include "ObjectStack.h"

namespace Object
{
	namespace Module
	{
		namespace Stack
		{
			template <typename OBJECT>
			Data<OBJECT>::~Data()
			{

			}

			template <typename DATA>
			Func<DATA>::~Func()
			{

			}
			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				memset(specific, 0, sizeof(specific));
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				if (printFnc)
				{
					Stack stack(dynamic_cast<typename DATA::OBJECT_T *>(this));
					return printFnc(&stack);
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