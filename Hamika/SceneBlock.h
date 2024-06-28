#ifndef __SCENE_BLOCK_H__
#define __SCENE_BLOCK_H__

#include "Types.h"
#include "Objects.h"


template <typename OBJECT_T>
struct SceneBlock
{
	OBJECT_T *object = nullptr;//jön be, vagy már bent van
	OBJECT_T *remain = nullptr;//épp eltûnik, kimegy
	Type::Coord ComeFrom;
	Type::Coord GoTo;
	bool Redrawn = false;
	Type::Flags grid = 0;//GridFlags::Gravity,
	unsigned long long int DrawNumber = 0;

	~SceneBlock();
};

#endif

#include "SceneBlock.hpp"