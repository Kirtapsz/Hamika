#ifndef OBJECT_ACTIONS_HPP
#define OBJECT_ACTIONS_HPP

#include "ObjectActions.h"

namespace Object
{
	namespace Module
	{
		namespace Actions
		{
			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				action = STEADY;
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				json["action"] = action;

				return json;
			}

			template <typename DATA>
			bool Func<DATA>::isAction(typename DATA::ACTION_T _action) const
			{
				return action == _action;
			}
			template <typename DATA>
			bool Func<DATA>::isAction() const
			{
				return action != STEADY;
			}
			template <typename DATA>
			bool Func<DATA>::isActionMove() const
			{
				return action & MOVE;
			}
			template <typename DATA>
			bool Func<DATA>::isActionRotate() const
			{
				return action & ROTATE;
			}
		}
	}
}

#endif