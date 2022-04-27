#ifndef __ACTIVE_BLOCK_H__
#define __ACTIVE_BLOCK_H__

#include "Types.h"
#include "Objects.h"


template <typename OBJECT_T>
struct ActiveBlock
{
	OBJECT_T *object = nullptr;//j�n be, vagy m�r bent van
	OBJECT_T *remain = nullptr;//�pp elt�nik, kimegy
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

#endif

#include "ActiveBlock.hpp"