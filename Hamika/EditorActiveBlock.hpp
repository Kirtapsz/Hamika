#ifndef __EDITOR_ACTIVE_BLOCK_HPP__
#define __EDITOR_ACTIVE_BLOCK_HPP__

#include "EditorActiveBlock.h"

namespace Editor
{
	template <typename OBJECT_T>
	ActiveBlock<OBJECT_T>::~ActiveBlock()
	{
		if (object) delete object;
		if (remain) delete remain;
	}
}

#endif