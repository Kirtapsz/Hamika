#ifndef OBJECT_EXECUTE_HPP
#define OBJECT_EXECUTE_HPP

#include "ObjectExecute.h"

namespace Object
{
	namespace Module
	{
		namespace Execute
		{
			template<typename OBJECT>
			unsigned long long Data<OBJECT>::totalUpdateNumber{0};

			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				printFnc = nullptr;
				updaterFnc = nullptr;
				finalizeFnc = nullptr;
				timerFnc = nullptr;
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				return json;
			}

			template <typename DATA>
			void Func<DATA>::RunUpdate(typename DATA::UpdateType updateType)
			{
				requests.update = false;
				updateNumber = totalUpdateNumber++;

				if (updaterFnc)
				{
					typename DATA::OBJECT_T *object = dynamic_cast<typename DATA::OBJECT_T *>(this);
					updaterFnc(*object, updateType);
				}
			}

			template <typename DATA>
			void Func<DATA>::RunFinalize()
			{
				requests.finalize = false;
				if (finalizeFnc)
				{
					typename DATA::OBJECT_T *object = dynamic_cast<typename DATA::OBJECT_T *>(this);
					finalizeFnc(*object);
				}
			}

			template <typename DATA>
			void Func<DATA>::RunTimer()
			{
				requests.timer = false;
				if (timerFnc)
				{
					typename DATA::OBJECT_T *object = dynamic_cast<typename DATA::OBJECT_T *>(this);
					timerFnc(*object);
				}
			}
		}
	}
}

#endif