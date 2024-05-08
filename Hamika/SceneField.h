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
		protected: bool globalGravity;
	};

	template <typename DATA>
	class Func: public virtual Object::Brick::Interface, public virtual DATA
	{
		protected: void initialize(const std::shared_ptr<Res::BluePrint> &bluePrint_)
		{
			_bluePrint = bluePrint_;

			ObjectCreate(this, &bedrock, ObjectID::FullBedrock, Type::Coord::Invalid, Object::Entity::Bedrock::Create);
			ObjectCreate(this, &space, ObjectID::FullSpace, Type::Coord::Invalid, Object::Entity::Space::Create);

			globalGravity = _bluePrint->globalGravity;

			map.reset(new Matrix<SceneBlock<Object::Brick>>(_bluePrint->blocks));
			objects.resize(((Type::Size)*map).width() * ((Type::Size)*map).height());
			remains.resize(objects.size());
			map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
			{
				block.object = new Object::Brick;
				ObjectCreate(this, block.object, (*_bluePrint)[coord].id, coord);
				block.object->rotation = (*_bluePrint)[coord].rotation;
				block.remain = new Object::Brick;
				block.remain->isExists = false;
				block.remain->setScene(this);

				block.GoTo = coord;
				block.ComeFrom = coord;

				block.grid = (*_bluePrint)[coord].flags;

			});

			Type::ID rootId = 1;
			map->foreach([&](const Type::Coord &, SceneBlock<Object::Brick> &block)
			{
				block.object->rootId = rootId++;
				block.remain->rootId = rootId++;
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
			return globalGravity;
		}
		public: virtual void switchGravity()
		{
			globalGravity = !globalGravity;
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