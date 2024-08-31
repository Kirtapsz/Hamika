#ifndef __EDITOR_SCENE_BLOCK_H__
#define __EDITOR_SCENE_BLOCK_H__

#include "Types.h"
#include "Objects.h"
#include "SceneBlock.h"

namespace Editor::UI
{
	template <typename OBJECT_T_>
	struct SceneBlock: ::SceneBlock<OBJECT_T_>
	{
		using OBJECT_T = OBJECT_T_;

		bool selected = false;
		bool deselectTmp1 = false;
		bool selectTmp1 = false;
		bool selectTmp2 = false;
	};
}

#endif

#include "EditorSceneBlock.hpp"