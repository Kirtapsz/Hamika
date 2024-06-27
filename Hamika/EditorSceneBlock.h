#ifndef __EDITOR_SCENE_BLOCK_H__
#define __EDITOR_SCENE_BLOCK_H__

#include "Types.h"
#include "Objects.h"

namespace Editor::UI
{
	template <typename OBJECT_T>
	struct SceneBlock
	{
		OBJECT_T *object = nullptr;//jön be, vagy már bent van
		OBJECT_T *remain = nullptr;//épp eltûnik, kimegy
		bool selected = false;
		bool deselectTmp1 = false;
		bool selectTmp1 = false;
		bool selectTmp2 = false;
		Type::Coord ComeFrom = Type::Coord::Invalid;
		Type::Coord GoTo = Type::Coord::Invalid;
		bool Redrawn = false;
		Type::Flags grid = 0;//GridFlags::Gravity,
		unsigned long long int DrawNumber = 0;

		~SceneBlock();
	};
}

#endif

#include "EditorSceneBlock.hpp"