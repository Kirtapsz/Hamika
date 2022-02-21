#ifndef __ACTIVE_BLOCK_HPP__
#define __ACTIVE_BLOCK_HPP__

#include "ActiveBlock.h"

template <typename OBJECT_T>
ActiveBlock<OBJECT_T>::~ActiveBlock()
{
	if (object) delete object;
	if (remain) delete remain;
}

#endif