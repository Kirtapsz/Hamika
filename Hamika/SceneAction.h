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

namespace UI::Scene::Module::Action
{
	class Data
	{
		protected: std::vector<ObjectBase *> objects;
		protected: std::vector<ObjectBase *> remains;
		protected: bool enableupdateskip;

		protected: virtual void Redrawn(Type::Coord coord) = 0;
	};

	template <typename DATA>
	class Func: public virtual ObjectBase::Interface, public virtual DATA
	{
		protected: void initialize()
		{
			objects.resize(((Type::Size)*map).width * ((Type::Size)*map).height);
			remains.resize(objects.size());
		}
		protected: void buildObjectsHolder()
		{
			ObjectBase **o = objects.data();
			ObjectBase **r = remains.data();

			map->foreach([&](const Type::Coord &coord, SceneBlock<ObjectBase> &block)
			{
				*o++ = block.object;
				*r++ = block.remain;
			});
		}
		protected: void actionRun()
		{
			buildObjectsHolder();
			constexpr auto &fnc = [](ObjectBase *o)
			{
				if (o->isExists && o->events.timer && o->requests.timer)
				{
					o->RunTimer();
				}
			};
			std::for_each(objects.begin(), objects.end(), fnc);
			std::for_each(remains.begin(), remains.end(), fnc);

			UpdateRun();

			map->foreach([&](const Type::Coord &coord, SceneBlock<ObjectBase> &block)
			{
				if (block.remain->isExists && block.remain->requests.remove)
					DeleteRemain(coord);
				if (block.object->isExists)
				{
					if (block.object->requests.remove)
						DeleteObject(coord);
					else if (block.object->requests.blowUp && block.object->GetFlags() & ObjectBase::Flags::ExplosionType1)
					{
						block.object->requests.blowUp = false;
						Blasting(coord);
					}
				}
			});
		}

		protected: void CopyObject(Type::Coord coordDst, Type::Coord coordSrc)
		{
			if (map->Test(coordDst) && map->Test(coordSrc))
			{
				ObjectBase *objTmp = reach(map)[coordDst].object;
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
				ObjectBase *objTmp = reach(map)[coordDst].remain;
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
				ObjectBase *objTmp = reach(map)[coordDst].remain;
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
				ObjectBase *objTmp = reach(map)[coordDst].object;
				reach(map)[coordDst].object = reach(map)[coordSrc].remain;
				reach(map)[coordSrc].remain = objTmp;

				reach(map)[coordDst].object->coord = coordDst;
				reach(map)[coordSrc].remain->coord = coordSrc;
			}
		}
		protected: void ExplosionPut(Type::Coord coord, Type::ID IDto)
		{
			if (map->Test(coord) && reach(map)[coord].object->requests.blowUp == false)
			{
				if (murphy == reach(map)[coord].object)
				{
					murphyDead(murphy);
				}

				DeleteRemain(coord);
				if (reach(map)[coord].object->GetFlags() & ObjectBase::Flags::CanBeExplosion)
				{
					ObjectCreate(reach(map)[coord].remain, ObjectID::ExplosionExpansive, coord);
					reach(map)[coord].remain->SetObjectIDremain(IDto);
				}
				else
				{
					Type::ID Expid = reach(map)[coord].remain->isExists ? Expid = reach(map)[coord].remain->id : ObjectID::Space;
					if (Expid == ObjectID::Explosion || Expid == ObjectID::ExplosionEffect || Expid == ObjectID::ExplosionExpansive)
					{
						ObjectCreate(reach(map)[coord].remain, ObjectID::ExplosionEffect, coord);
					}
				}
				Redrawn(coord);
			}
		}

		protected: void Blasting(Type::Coord coord)
		{
			if (map->Test(coord) && reach(map)[coord].object->isExists)
			{
				Type::Flags flag = reach(map)[coord].object->GetFlags();
				Type::ID IDto = reach(map)[coord].object->GetTranslationTo();
				Type::Coord center = reach(map)[coord].object->GetHitCoord();

				reach(map)[coord].object->RemoveFlags(ObjectBase::Flags::ExplosionType1);
				reach(map)[coord].object->SetTranslationID(ObjectID::Space);

				//bool
				//	exploseNow = reach(map)[coord].remain && (reach(map)[coord].remain->ID() == ObjectID::Explosion || reach(map)[coord].remain->ID() == ObjectID::ExplosionExpansive);

				ExplosionPut(center, IDto);

				//if (exploseNow)
				//{
				//	Type::ID
				// 
				//		remainID = reach(map)[coord].object->GetTranslationTo();
				//	ObjectPut(coord, ObjectSpace::Create);
				//	reach(map)[coord].object->SetTranslationID(remainID);
				//}

				if (flag & ObjectBase::ExplosionType3)
				{
					reach(map)[coord].object->RemoveFlags(ObjectBase::Flags::ExplosionType3);
					ExplosionPut({center.x + 1,center.y + 1}, IDto);
					ExplosionPut({center.x,center.y + 1}, IDto);
					ExplosionPut({center.x - 1,center.y + 1}, IDto);

					ExplosionPut({center.x + 1,center.y}, IDto);
					ExplosionPut({center.x - 1,center.y}, IDto);

					ExplosionPut({center.x + 1,center.y - 1}, IDto);
					ExplosionPut({center.x,center.y - 1}, IDto);
					ExplosionPut({center.x - 1,center.y - 1}, IDto);
				}
				if (flag & ObjectBase::ExplosionType5)
				{
					reach(map)[coord].object->RemoveFlags(ObjectBase::Flags::ExplosionType5);
				}

				if (reach(map)[coord].remain->id != ObjectID::Explosion &&
					reach(map)[coord].remain->id != ObjectID::ExplosionEffect &&
					reach(map)[coord].remain->id != ObjectID::ExplosionExpansive)
				{
					if (murphy == reach(map)[coord].object)
					{
						murphyDead(murphy);
					}

					DeleteRemain(coord);
					ObjectCreate(reach(map)[coord].remain, ObjectID::Explosion, coord);
					reach(map)[coord].object->SetObjectIDremain(ObjectID::Space);
					Redrawn(coord);
				}

				UpdateSquare33({center.x + 1,center.y + 1});
				UpdateSquare33({center.x - 1,center.y + 1});
				UpdateSquare33({center.x + 1,center.y - 1});
				UpdateSquare33({center.x - 1,center.y - 1});
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
				 //protected: void SetObject(Type::Coord coord, ObjectBase *object)
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
				ObjectCreate(reach(map)[coord].object, reach(map)[coord].object->GetTranslationTo(), coord);
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



		protected:  void UpdateRun()
		{
			std::function<void(const Type::Coord &, SceneBlock<ObjectBase> &)> fncPre = [&](const Type::Coord &coord, SceneBlock<ObjectBase> &block)
			{
				if (block.object->isExists && GetObject(coord)->events.update && GetObject(coord)->requests.update && !block.object->IsMoving())
				{
					GetObject(coord)->Update();
				}
			};
			std::function<void(const Type::Coord &, SceneBlock<ObjectBase> &)> fncPost = [&](const Type::Coord &coord, SceneBlock<ObjectBase> &block)
			{
				if (block.object->isExists && block.object->events.update && block.object->requests.update && !block.object->IsMoving())
				{
					block.object->Update();
				}
				if (block.remain->isExists && block.remain->events.update && block.remain->requests.update && !block.remain->IsMoving())
				{
					block.remain->Update();
				}
			};

			enableupdateskip = true;
			map->foreach(fncPre);

			enableupdateskip = false;
			map->reverse_foreach(fncPost);
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

		protected:  void UpdateSquare33(Type::Coord coord)
		{
			UpdateBlock({coord.x,coord.y});
			UpdateBlock({coord.x - 1,coord.y});
			UpdateBlock({coord.x,coord.y - 1});
			UpdateBlock({coord.x + 1,coord.y});
			UpdateBlock({coord.x,coord.y + 1});

			UpdateBlock({coord.x - 1,coord.y - 1});
			UpdateBlock({coord.x + 1,coord.y - 1});
			UpdateBlock({coord.x + 1,coord.y + 1});
			UpdateBlock({coord.x - 1,coord.y + 1});
		}
		protected:  void UpdateVertical3(Type::Coord coord)
		{
			UpdateBlock({coord.x,coord.y});
			UpdateBlock({coord.x,coord.y - 1});
			UpdateBlock({coord.x,coord.y + 1});
		}
		protected: void UpdateHorizontal3(Type::Coord coord)
		{
			UpdateBlock({coord.x,coord.y});
			UpdateBlock({coord.x - 1,coord.y});
			UpdateBlock({coord.x + 1,coord.y});
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

		public: virtual void BlowUpBlock(Type::Coord coord)
		{
			auto object = GetObject(coord);
			if (object->GetFlags() & ObjectBase::Flags::ExplosionType1)
			{
				if (object->GetAbsMove() > 0.5f)
				{
					object->blowUp(GetComefrom(coord));
				}
				else
				{
					object->blowUp(coord);
				}
			}

			object = GetObjectOut(coord);
			if (object->GetFlags() & ObjectBase::Flags::ExplosionType1)
			{
				if (object->GetAbsMove() < 0.5f)
				{
					object->blowUp(GetGoto(coord));
				}
				else
				{
					object->blowUp(coord);
				}
			}
		}
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
				reach(map)[to].object->actions.move = true;

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
				Type::Coord
					ComeFrom = reach(map)[coord].ComeFrom;

				reach(map)[reach(map)[coord].ComeFrom].GoTo = reach(map)[coord].ComeFrom;
				reach(map)[coord].ComeFrom = coord;

				DeleteRemain(coord);

				reach(map)[coord].object->actions.move = false;

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

				reach(map)[coord].object->actions.move = false;

				UpdateSquare33(coord);
			}
		}
		public: virtual void ObjectDisappear(Type::Coord coord)
		{
			if (TestObject(coord) && !reach(map)[coord].object->IsMoving())
			{
				CopyObjectToRemain(coord, coord);
				//(*reach(map)[coord].remain) = reach(map)[coord].object;
				reach(map)[coord].remain->requests.remove = true;

				ObjectCreate(reach(map)[coord].object, reach(map)[coord].object->GetTranslationTo(), coord);

				UpdateSquare33(coord);
			}
		}
		public: virtual bool EnableUpdateSkip()
		{
			return enableupdateskip;
		}
		public: virtual bool rollTrigger(ObjectBase *obj_, unsigned __int16 typeID, float chancePerSec)
		{
			return rngController->rollTrigger(obj_->rootId, typeID, chancePerSec);
		}
	};
}

#pragma warning(default:4250)