#pragma once

#include "Types.h"
#include "World.h"
#include "Objects.h"

#include <KIR\AL\KIR5_panel.h>
#include <KIR\KIR4_time.h>
#include <KIR\AL\KIR5_display.h>
#include <KIR\AL\KIR5_event_engine.h>

#include <memory>
#include <limits>

#include "Account.h"
#include "StatusBar.h"
#include "SceneDrawer.h"
#include "Object.h"
#include "UItools.h"
#include "SceneBlock.h"
#include "KeyboardController.h"
#include "RngController.h"
#include "Validator.h"

#pragma warning(disable: 4250)

namespace UI::Scene::Module::Field
{
	class Data
	{
		protected: std::shared_ptr<Res::BluePrint> _bluePrint;
		protected: KIR5::Shared<Matrix<SceneBlock<Object::Brick>>> map;
		protected: Object::Brick bedrock;
		protected: Object::Brick space;
		protected: bool global_gravity_;
	};

	template <typename DATA>
	class Func: public virtual Object::SceneInterface, public virtual DATA
	{
		protected: void initialize(const std::shared_ptr<Res::BluePrint> &bluePrint_)
		{
			_bluePrint = bluePrint_;

			ObjectCreate(this, &bedrock, ObjectID::FullBedrock, Type::Coord::Invalid, Object::Entity::Bedrock::Create);
			ObjectCreate(this, &space, ObjectID::FullSpace, Type::Coord::Invalid, Object::Entity::Space::Create);

			global_gravity_ = _bluePrint->globalGravity;

			map.reset(new Matrix<SceneBlock<Object::Brick>>(_bluePrint->blocks));
			objects.resize(((Type::Size)*map).width() * ((Type::Size)*map).height());
			remains.resize(objects.size());
			Type::ID rootId = 1;
			map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
			{
				const Res::BluePrint::Block &blue_print_block = (*_bluePrint)[coord];

				block.object = new Object::Brick;
				block.remain = new Object::Brick;

				block.object->rootId = rootId++;
				block.remain->rootId = rootId++;
				block.remain->setScene(this);

				block.GoTo = coord;
				block.ComeFrom = coord;

				block.grid = blue_print_block.flags;

				ObjectCreate(this, block.object, blue_print_block.id, coord, [&](Object::Brick &object)
				{
					object.rotation = blue_print_block.rotation;
				});
			});
		}

				 // OBJECT INTERFACE

		public: virtual Type::Flags GetBlockFlags(Type::Coord coord)
		{
			if (map->Test(coord))
				return reach(map)[coord].grid;
			return 0;
		}
		public: virtual Object::Brick &GetObject(Type::Coord coord)
		{
			if (map->Test(coord))
			{
				if (reach(map)[coord].object->isExists)
					return *reach(map)[coord].object;
				else
					return space;
			}
			return bedrock;
		}
		public: virtual Object::Brick &GetObjectOut(Type::Coord coord)
		{
			if (map->Test(coord))
			{
				if (reach(map)[coord].GoTo != coord && reach(map)[reach(map)[coord].GoTo].object->isExists)
					return *reach(map)[reach(map)[coord].GoTo].object;
				return space;
			}
			return bedrock;
		}
		public: virtual Object::Brick *GetObjectOutU(Type::Coord coord)
		{
			if (map->Test(coord))
			{
				if (reach(map)[coord].GoTo != coord && reach(map)[reach(map)[coord].GoTo].object->isExists)
					return reach(map)[reach(map)[coord].GoTo].object;
			}
			return nullptr;
		}
		public: virtual Object::Brick &GetRemain(Type::Coord coord)
		{
			if (map->Test(coord))
			{
				if (reach(map)[coord].remain->isExists)
					return *reach(map)[coord].remain;
				else
					return space;
			}
			return bedrock;
		}
		public: virtual Type::Flags GetUnionFlags(Type::Coord coord)
		{
			return GetObject(coord).GetFlags() | GetObjectOut(coord).GetFlags() | GetRemain(coord).GetFlags();
		}
		public: virtual Type::Flags GetSectionFlags(Type::Coord coord)
		{
			if (map->Test(coord))
			{
				Type::Flags
					flags = reach(map)[coord].object->GetFlags();
				if (reach(map)[coord].remain->isExists)
					flags &= reach(map)[coord].remain->GetFlags();
				if (reach(map)[coord].GoTo != coord)
					flags &= reach(map)[reach(map)[coord].GoTo].object->GetFlags();
				return flags;
			}
			return bedrock.GetFlags();
		}
		public: virtual Type::Coord GetGoto(Type::Coord coord)
		{
			if (map->Test(coord))
				return reach(map)[coord].GoTo;
			return coord;
		}
		public: virtual Type::Coord GetComefrom(Type::Coord coord)
		{
			if (map->Test(coord))
				return reach(map)[coord].ComeFrom;
			return coord;
		}
		public: virtual bool IsObjectOut(Type::Coord coord)
		{
			return (map->Test(coord) && coord != reach(map)[coord].GoTo);
		}
		public: virtual bool IsGlobalGravity() const
		{
			return global_gravity_;
		}
		public: virtual void switchGravity()
		{
			global_gravity_ = !global_gravity_;
			setGlobalGravity(global_gravity_);
		}
		public: virtual bool IamRemain(const Object::Brick &_brick)
		{
			if (map->Test(_brick.GetCoord()))
			{
				if (reach(map)[_brick.GetCoord()].object == &_brick)
					return false;
				if (reach(map)[_brick.GetCoord()].remain == &_brick)
					return true;
			}
			return true;
		}
		public: virtual Type::Size MapSize()
		{
			return (Type::Size)(reach(map));
		}
		public: virtual Object::Brick *GetObjectU(Type::Coord coord)
		{
			return reach(map)[coord].object;
		}
	};
}

#pragma warning(default:4250)