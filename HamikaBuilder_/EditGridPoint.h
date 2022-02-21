#pragma once

#include "Types.h"
#include "Objects.h"

struct GridPoint
{
	ObjectBase object;
	bool Redrawn = true;
	Type::Flags grid = 0;
};