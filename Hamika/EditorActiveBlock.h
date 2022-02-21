#ifndef __EDITOR_ACTIVE_BLOCK_H__
#define __EDITOR_ACTIVE_BLOCK_H__

#include "Types.h"
#include "Objects.h"

namespace Editor
{
	template <typename OBJECT_T>
	struct ActiveBlock
	{
		OBJECT_T *object = NULL;//jön be, vagy már bent van
		OBJECT_T *remain = NULL;//épp eltûnik, kimegy
		bool selected = false;
		bool deselectTmp1 = false;
		bool selectTmp1 = false;
		bool selectTmp2 = false;
		Type::Coord ComeFrom;
		Type::Coord GoTo;
		bool Redrawn = false;
		Type::Flags grid = 0;//GridFlags::Gravity,
		Type::Flags DrawType = 0;
		unsigned long long int DrawNumber = 0;
		enum DrawType:Type::Flags
		{
			Cleared = 1,
			IsNotMovingObjectDrawned = 2,
			IsNotMovingRemainDrawned = 4,
			IsMovingObjectDrawned = 8,
			LastDrawned = 16,
		};

		~ActiveBlock();
	};
}

#endif

#include "EditorActiveBlock.hpp"