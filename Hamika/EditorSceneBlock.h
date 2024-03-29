#ifndef __EDITOR_SCENE_BLOCK_H__
#define __EDITOR_SCENE_BLOCK_H__

#include "Types.h"
#include "Objects.h"

namespace Editor::UI
{
	template <typename OBJECT_T>
	struct SceneBlock
	{
		OBJECT_T *object = nullptr;//j�n be, vagy m�r bent van
		OBJECT_T *remain = nullptr;//�pp elt�nik, kimegy
		bool selected = false;
		bool deselectTmp1 = false;
		bool selectTmp1 = false;
		bool selectTmp2 = false;
		Type::Coord ComeFrom = Type::Coord::Invalid;
		Type::Coord GoTo = Type::Coord::Invalid;
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

		~SceneBlock();
	};
}

#endif

#include "EditorSceneBlock.hpp"