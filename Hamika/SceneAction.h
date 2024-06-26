﻿#pragma once

#include "Types.h"
#include "World.h"
#include "Objects.h"
#include "OriginalEntities.h"

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

namespace UI::Scene::Module::Action
{
	class Data
	{
		protected: std::vector<Object::Brick *> objects;
		protected: std::vector<Object::Brick *> remains;

		protected: virtual void Redrawn(Type::Coord coord) = 0;
	};

	template <typename DATA>
	class Func: public virtual Object::SceneInterface, public virtual DATA
	{
		protected: void initialize()
		{
			objects.resize(((Type::Size)*map).width() * ((Type::Size)*map).height());
			remains.resize(objects.size());
		}
		protected: void buildObjectsHolder()
		{
			Object::Brick **o = objects.data();
			Object::Brick **r = remains.data();

			map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
			{
				*o++ = block.object;
				*r++ = block.remain;
			});
		}


				 // gives back the remaining progress, 0 means it is just started, 1 means it is just finished
		protected: float getMoveProgress(Object::Brick &_brick) const
		{
			if (_brick.isExists)
			{
				Type::Coord coord = _brick.GetCoord();
				Type::Coord from_coord = reach(map)[coord].ComeFrom;
				if (coord != from_coord)
				{
					Type::Coord::base move_distance = std::abs((coord.x() - from_coord.x()) + (coord.y() - from_coord.y()));
					Type::Move::base move = _brick.GetAbsMove();
					return 1.f - (move / (float)move_distance);
				}
			}
			return 1.f;
		}
		protected: constexpr float getMoveProgress(Object::Brick *_brick) const
		{
			return getMoveProgress(*_brick);
		}
		protected: void murphyBlowup(Object::Brick *_brick)
		{
			if (murphy == _brick)
			{
				murphyDead(murphy);
			}
		}
		protected: void blowupBlock(Type::Coord coord, SceneBlock<Object::Brick> &block, Type::ID IDto)
		{
			if (block.object->flags & Object::Brick::Flags::CanBeExploded
				&&
				(
					!block.remain->isExists
					||
					block.remain->flags & Object::Brick::Flags::CanBeExploded
					)
				&&
				(
					block.GoTo == coord
					||
					(
						reach(map)[block.GoTo].object->flags & Object::Brick::Flags::CanBeExploded
						||
						getMoveProgress(reach(map)[block.GoTo].object) > 0.5f
						)
					)
				)
			{
				if (block.remain->isExists && block.remain->id == ObjectID::Explosion)
				{
					block.remain->SetObjectIDremain(std::max(IDto, block.remain->GetObjectIDremain()));
					Object::Entity::Explosion_033::ReCreate(*block.remain);
				}
				else
				{
					DeleteRemain(coord);
					ObjectCreate(block.remain, ObjectID::Explosion, coord);
					block.remain->SetObjectIDremain(IDto);
					murphyBlowup(block.object);
					block.object->events.clear();
				}

				if (block.ComeFrom != coord)
				{
					if (getMoveProgress(block.object) <= 0.7f)
					{
						Object::Brick *remain = reach(map)[block.ComeFrom].remain;
						if (!remain->isExists
							||
							(
								remain->id != ObjectID::ExplosionEffect
								&&
								remain->id != ObjectID::Explosion
								)
							)
						{
							DeleteRemain(block.ComeFrom);
							ObjectCreate(remain, ObjectID::ExplosionEffect, block.ComeFrom);
						}
					}
				}
				if (block.GoTo != coord)
				{
					const SceneBlock<Object::Brick> &goto_block = reach(map)[block.GoTo];

					if (getMoveProgress(goto_block.object) <= 0.5f)
					{
						Object::Brick *remain = goto_block.remain;

						if (remain->isExists && remain->id != ObjectID::Explosion)
						{
							Type::ID id_to = goto_block.object->GetTranslationTo();
							DeleteRemain(block.GoTo);
							ObjectCreate(remain, ObjectID::Explosion, block.GoTo);
							remain->SetObjectIDremain(std::max(id_to, remain->GetObjectIDremain()));
							murphyBlowup(goto_block.object);
							goto_block.object->events.clear();
						}
					}
				}
				Redrawn(coord);
			}
		}
		protected: virtual void blowup(Object::Brick &_brick, Type::Coord center)
		{
			Type::Flags flags = _brick.GetFlags();

			if (flags & Object::Brick::Flags::ExplosionType)
			{
				Type::ID id_to = _brick.GetTranslationTo();
				Type::Coord coord = _brick.GetCoord();
				Type::Coord coord_come_from = reach(map)[coord].ComeFrom;
				if (center == Type::Coord::Invalid)
				{
					center = coord;
					if (_brick.IsMove())
					{
						if (getMoveProgress(_brick) <= 0.5f)
						{
							center = coord_come_from;
						}
					}
				}

				_brick.RemoveFlags(Object::Brick::Flags::ExplosionType);
				_brick.SetTranslationID(ObjectID::Space);
				Type::Coord::base d = 0; // Object::Brick::Flags::ExplosionType1
				if (flags & Object::Brick::Flags::ExplosionType3)
				{
					d = 1;
				}
				else if (flags & Object::Brick::Flags::ExplosionType5)
				{
					d = 2;
				}
				Type::Coord begin_of_center = center - d;
				Type::Coord end_of_center = center + 1 + d;
				begin_of_center.limiter({0,0}, map->size());
				end_of_center.limiter({0,0}, map->size());

				map->forrange(begin_of_center, end_of_center, [&](Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					blowupBlock(coord, block, id_to);
				});
				if (!coord.isInside(begin_of_center, end_of_center))
				{
					blowupBlock(coord, reach(map)[coord], ObjectID::Space);
				}
			}
		}
		protected: void actionRun()
		{
			buildObjectsHolder();

			if (murphy)
			{
				murphy->RunTimer();
			}

			constexpr auto &fnc = [](Object::Brick *o)
			{
				if (o->isExists && o->events.timer && o->requests.timer)
				{
					o->RunTimer();
				}
			};
			std::for_each(objects.begin(), objects.end(), fnc);
			std::for_each(remains.begin(), remains.end(), fnc);

			UpdateRun();

			FinalizeRun();

			map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
			{
				if (block.remain->isExists && block.remain->requests.remove)
					DeleteRemain(coord);
				if (block.object->isExists && block.object->requests.remove)
					DeleteObject(coord);
			});
		}

		protected: void CopyObject(Type::Coord coordDst, Type::Coord coordSrc)
		{
			if (map->Test(coordDst) && map->Test(coordSrc))
			{
				Object::Brick *objTmp = reach(map)[coordDst].object;
				reach(map)[coordDst].object = reach(map)[coordSrc].object;
				reach(map)[coordSrc].object = objTmp;

				reach(map)[coordDst].object->coord = coordDst;
				reach(map)[coordSrc].object->coord = coordSrc;
			}
		}
		protected: void CopyRemain(Type::Coord coordDst, Type::Coord coordSrc)
		{
			if (map->Test(coordDst) && map->Test(coordSrc))
			{
				Object::Brick *objTmp = reach(map)[coordDst].remain;
				reach(map)[coordDst].remain = reach(map)[coordSrc].remain;
				reach(map)[coordSrc].remain = objTmp;

				reach(map)[coordDst].remain->coord = coordDst;
				reach(map)[coordSrc].remain->coord = coordSrc;
			}
		}
		protected: void CopyObjectToRemain(Type::Coord coordDst, Type::Coord coordSrc)
		{
			if (map->Test(coordDst) && map->Test(coordSrc))
			{
				Object::Brick *objTmp = reach(map)[coordDst].remain;
				reach(map)[coordDst].remain = reach(map)[coordSrc].object;
				reach(map)[coordSrc].object = objTmp;

				reach(map)[coordDst].remain->coord = coordDst;
				reach(map)[coordSrc].object->coord = coordSrc;
			}
		}
		protected: void CopyRemainToObject(Type::Coord coordDst, Type::Coord coordSrc)
		{
			if (map->Test(coordDst) && map->Test(coordSrc))
			{
				Object::Brick *objTmp = reach(map)[coordDst].object;
				reach(map)[coordDst].object = reach(map)[coordSrc].remain;
				reach(map)[coordSrc].remain = objTmp;

				reach(map)[coordDst].object->coord = coordDst;
				reach(map)[coordSrc].remain->coord = coordSrc;
			}
		}

		protected: void StepDisappear(Type::Coord coord)
		{
			if (TestObject(coord))
			{
				if (reach(map)[coord].object->id == 0)
				{
					if (reach(map)[coord].ComeFrom != coord)
					{
						reach(map)[reach(map)[coord].ComeFrom].GoTo = reach(map)[coord].ComeFrom;
						reach(map)[coord].ComeFrom = coord;
					}
				}
				else
				{
					DeleteRemain(coord);
					CopyObjectToRemain(coord, coord);
					//(*reach(map)[coord].remain) = reach(map)[coord].object;
				}
				reach(map)[coord].object->isExists = false;
			}
		}
				 //protected: void SetObject(Type::Coord coord, Object::Brick *object)
				 //{
				 //	//reach(map)[coord].object->isExists=false;
				 //	CopyObject
				 //	//(*reach(map)[coord].object) = object;
				 //	Redrawn(coord);
				 //}
		protected:  void DeleteObject(Type::Coord coord)
		{
			if (TestObject(coord))
			{
				ObjectCreate(reach(map)[coord].object, reach(map)[coord].object->GetObjectIDremain(), coord);
				if (reach(map)[coord].ComeFrom != coord)
				{
					reach(map)[reach(map)[coord].ComeFrom].GoTo = reach(map)[coord].ComeFrom;
					reach(map)[coord].ComeFrom = coord;
				}
				Redrawn(coord);
				UpdateSquare33(coord);
			}
		}
		protected:  void DeleteRemain(Type::Coord coord)
		{
			if (TestRemain(coord))
			{
				reach(map)[coord].remain->isExists = false;
				//Delete(reach(map)[coord].remain);
				Redrawn(coord);
				UpdateSquare33(coord);
			}
		}



		protected: void UpdateRun()
		{
			map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
			{
				if (block.object->isExists && block.object->events.update && block.object->requests.update)
				{
					block.object->RunUpdate(UpdateType::UPDATE_ASC);
				}
			});

			map->reverse_foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
			{
				if (block.object->isExists && block.object->events.update && block.object->requests.update)
				{
					block.object->RunUpdate(UpdateType::UPDATE_DESC);
				}
				if (block.remain->isExists && block.remain->events.update && block.remain->requests.update)
				{
					block.remain->RunUpdate(UpdateType::UPDATE_DESC);
				}
			});

			if (murphy)
			{
				murphy->RunUpdate(UpdateType::UPDATE_MURPHY);
			}
		}

		protected: void FinalizeRun()
		{
			map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
			{
				if (block.object->isExists && block.object->events.update && block.object->requests.finalize)
				{
					block.object->RunFinalize();
				}
				if (block.remain->isExists && block.remain->events.update && block.remain->requests.finalize)
				{
					block.remain->RunFinalize();
				}
			});
		}

		protected: void UpdateBlock(Type::Coord coord)
		{
			if (map->Test(coord))
			{
				if (reach(map)[coord].object->isExists)
				{
					reach(map)[coord].object->requests.update = true;
				}
				if (reach(map)[coord].remain->isExists)
				{
					reach(map)[coord].remain->requests.update = true;
				}
			}
		}

		protected: void UpdateSquare33(Type::Coord coord)
		{
			UpdateBlock({coord.x(),coord.y()});
			UpdateBlock({coord.x() - 1,coord.y()});
			UpdateBlock({coord.x(),coord.y() - 1});
			UpdateBlock({coord.x() + 1,coord.y()});
			UpdateBlock({coord.x(),coord.y() + 1});

			UpdateBlock({coord.x() - 1,coord.y() - 1});
			UpdateBlock({coord.x() + 1,coord.y() - 1});
			UpdateBlock({coord.x() + 1,coord.y() + 1});
			UpdateBlock({coord.x() - 1,coord.y() + 1});
		}
		protected:  void UpdateVertical3(Type::Coord coord)
		{
			UpdateBlock({coord.x(),coord.y()});
			UpdateBlock({coord.x(),coord.y() - 1});
			UpdateBlock({coord.x(),coord.y() + 1});
		}
		protected: void UpdateHorizontal3(Type::Coord coord)
		{
			UpdateBlock({coord.x(),coord.y()});
			UpdateBlock({coord.x() - 1,coord.y()});
			UpdateBlock({coord.x() + 1,coord.y()});
		}
		protected: bool TestObject(Type::Coord coord) const
		{
			return map->Test(coord) && reach(map)[coord].object->isExists;
		}
		protected:  bool TestRemain(Type::Coord coord) const
		{
			return map->Test(coord) && reach(map)[coord].remain->isExists;
		}

				 // OBJECT INTERFACE

		public: virtual void ObjectMove(Type::Coord from, Type::Coord to, Type::ID remain)
		{
			if (from != to && TestObject(from) && map->Test(to))
			{
				StepDisappear(to);

				CopyObject(to, from);
				ObjectCreate(reach(map)[from].object, remain, from);

				reach(map)[to].ComeFrom = from;
				reach(map)[from].GoTo = to;

				reach(map)[to].object->SetCoord(to);

				UpdateSquare33(from);
				UpdateSquare33(to);
			}
		}
		public: virtual void ObjectPut(Type::Coord coord, Type::ID create)
		{
			if (TestObject(coord))
			{
				ObjectCreate(reach(map)[coord].object, create, coord);

				UpdateSquare33(coord);
				Redrawn(coord);
			}
		}
		public: virtual void RemainPut(Type::Coord coord, Type::ID create)
		{
			if (map->Test(coord))
			{
				DeleteRemain(coord);
				ObjectCreate(reach(map)[coord].remain, create, coord);

				UpdateSquare33(coord);
				Redrawn(coord);
			}
		}
		public: virtual void ObjectArrived(Type::Coord coord)
		{
			if (map->Test(coord) && reach(map)[coord].ComeFrom != coord)
			{
				Redrawn(coord);
				Type::Coord ComeFrom = reach(map)[coord].ComeFrom;

				reach(map)[reach(map)[coord].ComeFrom].GoTo = reach(map)[coord].ComeFrom;
				reach(map)[coord].ComeFrom = coord;

				DeleteRemain(coord);

				UpdateSquare33(ComeFrom);
			}
			UpdateSquare33(coord);//bel�lr�l ki lett szedve, ugyanis mas okokbol is ezkezhet meg, pl ollo mozgasa fordulasbol
		}
		protected: void ObjectVirtualArrived(Type::Coord coord)
		{
			if (map->Test(coord) && reach(map)[coord].ComeFrom != coord)
			{
				Redrawn(coord);

				reach(map)[reach(map)[coord].ComeFrom].GoTo = reach(map)[coord].ComeFrom;
				reach(map)[coord].ComeFrom = coord;

				UpdateSquare33(coord);
			}
		}
		public: virtual void ObjectDisappear(Type::Coord coord)
		{
			if (TestObject(coord) && !reach(map)[coord].object->isActionMove())
			{
				CopyObjectToRemain(coord, coord);
				reach(map)[coord].remain->requests.remove = true;
				ObjectCreate(reach(map)[coord].object, reach(map)[coord].remain->GetObjectIDremain(), coord);
				UpdateSquare33(coord);
			}
		}
		public: virtual bool rollTrigger(Object::Brick *obj_, unsigned __int16 typeID, float chancePerSec)
		{
			return rngController->rollTrigger(obj_->rootId, typeID, chancePerSec);
		}
	};
}

#pragma warning(default:4250)