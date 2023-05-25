#ifndef __SCENE_BLOCK_HPP__
#define __SCENE_BLOCK_HPP__

#include "SceneBlock.h"

template <typename OBJECT_T>
SceneBlock<OBJECT_T>::~SceneBlock()
{
	if (object) delete object;
	if (remain) delete remain;
}

#endif