#ifndef __SCENE_BLOCK_H__
#define __SCENE_BLOCK_H__

#include "Types.h"
#include "Objects.h"


template <typename OBJECT_T_>
struct SceneBlock
{
	using OBJECT_T = OBJECT_T_;
	OBJECT_T_ *object = nullptr;//jön be, vagy már bent van
	OBJECT_T_ *remain = nullptr;//épp eltûnik, kimegy
	Type::Coord ComeFrom;
	Type::Coord GoTo;

	Type::Flags grid = 0;//GridFlags::Gravity,

	Type::Flags redrawn_type_ = DrawnedType::None;
	Type::Flags drawned_type_ = DrawnedType::None;

	unsigned long long int drawned_cycle_ = 0;

	~SceneBlock();

	::Json print() const
	{
		Json json;

		json["ComeFrom"] = ComeFrom.str();
		json["GoTo"] = GoTo.str();
		json["grid"] = grid;
		json["redrawn_type_"] = redrawn_type_;
		json["drawned_type_"] = drawned_type_;
		json["drawned_cycle_"] = drawned_cycle_;

		return json;
	}
};

#endif

#include "SceneBlock.hpp"