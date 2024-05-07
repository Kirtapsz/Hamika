#ifndef ENTITY_DATA_HPP
#define ENTITY_DATA_HPP

//#include "ObjectStack.h"
//
//#include "OriginalEntities.h"

namespace Object
{
	namespace Module
	{
		namespace Stack
		{
			template <typename OBJECT>
			union Data<OBJECT>::EntityData
			{
				unsigned char bytes[256];
				//Entity::Zonk_001::EntityData zonk_001;
			};
		}
	}
}

#endif