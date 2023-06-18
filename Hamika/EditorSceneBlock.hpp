#ifndef __EDITOR_SCENE_BLOCK_HPP__
#define __EDITOR_SCENE_BLOCK_HPP__

#include "EditorSceneBlock.h"

namespace Editor::UI
{
	template <typename OBJECT_T>
	SceneBlock<OBJECT_T>::~SceneBlock()
	{
		if (object) delete object;
		if (remain) delete remain;
	}
}

#endif