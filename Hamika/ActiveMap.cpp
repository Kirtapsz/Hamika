#include "ActiveMap.h"
#include "MainEvent.h"
#include "Tools.h"
#include "Murphy.h"

#include <fstream>

#include <KIR/sys/KIR5_files.h>
#include <KIR/sys/KIR5_system.h>

ActiveMap::ActiveMap()
{
	drawer.blockRefreshActive = false;
	drawer.layerActive = true;

	drawer.SetMap(map);

	ObjectCreate(&bedrock, ObjectID::FullBedrock, Type::Coord::Invalid, Object::Bedrock::Create);
	ObjectCreate(&space, ObjectID::FullSpace, Type::Coord::Invalid, Object::Space::Create);

	*this << statusbar;
	*this << drawnerPanel;
	drawnerPanel->show();

	fncMouseButtonDown = [&](FNC_MOUSE_BUTTON_DOWN_PARAMS) -> FNC_MOUSE_BUTTON_DOWN_RET
	{
		Type::Coord coord = drawer.GetFromCursor(x_, y_);
		if (map->Test(coord))
		{
			clog << "BLOCK POINT = " << coord << KIR4::eol;
			if (reach(map)[coord].object->isExists)
				reach(map)[coord].object->Print();
			if (reach(map)[coord].remain->isExists)
				reach(map)[coord].remain->Print();
			clog << "- - - - - - - - - - - - - - -" << KIR4::eol;
		}

		return false;
	};

	fncMoved = [&](FNC_MOVED_PARAMS)
	{
		drawnerPanel->move(0, 0, width(), height() - statusbar->Height());
		drawer.InitializeDrawOptions({drawnerPanel->width(), drawnerPanel->height() - statusbar->Height()}, cameraSize);
		statusbar->Align();
	};

	fncKeyDown = [&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
	{
		keyboardController->keyDown(key_);
		return false;
	};

	fncKeyUp = [&](FNC_KEY_UP_PARAMS)->FNC_KEY_UP_RET
	{
		keyboardController->keyUp(key_);
		return false;
	};

	fncTimer = [&](FNC_TIMER_PARAMS)
	{
		loopControllerInterface.loopRoll();
		keyboardController->loop();
		if (startLoop == 0)
		{
			buildObjectsHolder();
			static auto &fnc = [](ObjectBase *o)
			{
				if (o->isExists && o->events.timer && o->requests.timer)
				{
					o->RunTimer();
				}
			};
			std::for_each(objects.begin(), objects.end(), fnc);
			std::for_each(remains.begin(), remains.end(), fnc);

			UpdateRun();

			map->foreach([&](const Type::Coord &coord, ActiveBlock<ObjectBase> &block)
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

			if (!murphy)
			{
				if (stopLoop == 0)
				{
					stopMap();
				}
				else
				{
					--stopLoop;
				}
			}
			else
			{
				loopCounter++;
			}
		}
		else
		{
			--startLoop;
		}
	};

	fncTick = [&](FNC_TICK_PARAMS)
	{
		if (startLoop == 0)
		{
			buildObjectsHolder();
			static auto &fnc = [](ObjectBase *o)
			{
				if (o->isExists && o->events.tick && o->requests.tick)
				{
					o->RunTick();
				}
			};
			std::for_each(objects.begin(), objects.end(), fnc);
			std::for_each(remains.begin(), remains.end(), fnc);
		}
	};

	fncKeyChar = [&](FNC_KEY_CHAR_PARAMS)->FNC_KEY_CHAR_RET
	{
		return false;
	};

	drawnerPanel->fncDraw = [&](FNC_DRAW_PARAMS)
	{
		if (murphy)
		{
			drawer.MoveCameraTo({murphy->GetCoord().x + murphy->GetMove().x, murphy->GetCoord().y + murphy->GetMove().y});
		}
		drawer.DrawBlocks(x_, y_);
		if (startLoop > 0)
		{
			al_draw_filled_rectangle(x_, y_, x_ + w_, y_ + h_, KIR5::Color(0, 0, 0, 255 * (startLoop / (float)startLoopInit)).getAlphaColored());
		}
		else
		{
			if (!murphy)
			{
				al_draw_filled_rectangle(x_, y_, x_ + w_, y_ + h_, KIR5::Color(0, 0, 0, 255 - (255 * (stopLoop / (float)stopLoopInit))).getAlphaColored());
			}
		}
	};
}
ActiveMap::~ActiveMap()
{
	map.reset();
}

void ActiveMap::buildObjectsHolder()
{
	size_t i = 0;
	map->foreach([&](const Type::Coord &coord, ActiveBlock<ObjectBase> &block)
	{
		objects[i] = block.object;
		remains[i] = block.remain;
		++i;
	});
}
void ActiveMap::stopMap()
{
	logger->save(KIR5::pathCombine(KIR5::getModuleDirectory(), "Hamika", "replays", "test.log"));

	keyboardController = nullptr;
	rngController = nullptr;

	logger = nullptr;

	mainEvent->mapFinished(victory, loopCounter);
}
void ActiveMap::startMap(const BluePrint &disp_map, const std::shared_ptr<Account> &account)
{
	this->account = account;

	loopControllerInterface.loopStart();


	logger = std::shared_ptr<Logger>(new Logger(loopControllerInterface, disp_map.hash, account->hash));

	//logger->load(KIR5::pathCombine(KIR5::getModuleDirectory(), "Hamika", "replays", "test.log"));
	//keyboardController = std::shared_ptr<KeyboardController>(new KeyboardLoopReplay(loopControllerInterface, *logger));
	//rngController = std::shared_ptr<RngController>(new RngReplay(loopControllerInterface, *logger));

	keyboardController = std::shared_ptr<KeyboardController>(new StandardKeyboardRecord(loopControllerInterface, *logger));
	rngController = std::shared_ptr<RngController>(new RngRecord(loopControllerInterface, *logger));

	statusbar->SetMap(disp_map);

	victory = false;
	globalGravity = disp_map.globalGravity;
	cameraSize = disp_map.cameraSize;

	loopCounter = 0;
	startLoop = startLoopInit;
	stopLoop = stopLoopInit;

	std::vector<Type::Coord> spawns;
	map.reset(new Array2D<ActiveBlock<ObjectBase>>(disp_map.blocks));
	objects.resize(((Type::Size)*map).width * ((Type::Size)*map).height);
	remains.resize(objects.size());
	map->foreach([&](const Type::Coord &coord, ActiveBlock<ObjectBase> &block)
	{
		block.object = new ObjectBase(*this);
		ObjectCreate(block.object, disp_map[coord].id, coord);
		block.object->rotation = disp_map[coord].rotation;
		block.remain = new ObjectBase(*this);
		block.remain->isExists = false;

		block.GoTo = coord;
		block.ComeFrom = coord;

		block.grid = disp_map[coord].flags;

		if (block.grid & GridFlags::SpawnPoint)
		{
			spawns.push_back(coord);
		}
	});

	Type::Coord spawn = spawns[rand() % spawns.size()];
	ObjectCreate(reach(map)[spawn].object, ObjectID::Murphy, spawn);
	murphy = reach(map)[spawn].object;
	Object::Murphy::SetController(murphy, keyboardController.get());

	drawer.SetMap(map);
	drawer.InitializeDrawOptions({drawnerPanel->width(), drawnerPanel->height()}, cameraSize);
	drawer.setGlobalGravity(globalGravity);

	Type::ID rootId = 1;
	map->foreach([&](const Type::Coord &coord, ActiveBlock<ObjectBase> &block)
	{
		block.object->rootId = rootId++;
		block.remain->rootId = rootId++;
	});
}

void ActiveMap::Redrawn(Type::Coord coord)
{
	drawer.Redrawn(coord);
}

void ActiveMap::CopyObject(Type::Coord coordDst, Type::Coord coordSrc)
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
void ActiveMap::CopyRemain(Type::Coord coordDst, Type::Coord coordSrc)
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
void ActiveMap::CopyObjectToRemain(Type::Coord coordDst, Type::Coord coordSrc)
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
void ActiveMap::CopyRemainToObject(Type::Coord coordDst, Type::Coord coordSrc)
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




void ActiveMap::ExplosionPut(Type::Coord coord, Type::ID IDto)
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

void ActiveMap::BlowUpBlock(Type::Coord coord)
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

void ActiveMap::Blasting(Type::Coord coord)
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
void ActiveMap::ObjectVirtualArrived(Type::Coord coord)
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
void ActiveMap::StepDisappear(Type::Coord coord)
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
//void ActiveMap::SetObject(Type::Coord coord, ObjectBase *object)
//{
//	//reach(map)[coord].object->isExists=false;
//	CopyObject
//	//(*reach(map)[coord].object) = object;
//	Redrawn(coord);
//}
void ActiveMap::DeleteObject(Type::Coord coord)
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
void ActiveMap::DeleteRemain(Type::Coord coord)
{
	if (TestRemain(coord))
	{
		reach(map)[coord].remain->isExists = false;
		//Delete(reach(map)[coord].remain);
		Redrawn(coord);
		UpdateSquare33(coord);
	}
}



void ActiveMap::UpdateRun()
{
	std::function<void(const Type::Coord &, ActiveBlock<ObjectBase> &)> fncPre = [&](const Type::Coord &coord, ActiveBlock<ObjectBase> &block)
	{
		if (block.object->isExists && GetObject(coord)->events.update && GetObject(coord)->requests.update && !block.object->IsMoving())
		{
			GetObject(coord)->Update();
		}
	};
	std::function<void(const Type::Coord &, ActiveBlock<ObjectBase> &)> fncPost = [&](const Type::Coord &coord, ActiveBlock<ObjectBase> &block)
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

void ActiveMap::UpdateBlock(Type::Coord coord)
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

void ActiveMap::UpdateSquare33(Type::Coord coord)
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
void ActiveMap::UpdateVertical3(Type::Coord coord)
{
	UpdateBlock({coord.x,coord.y});
	UpdateBlock({coord.x,coord.y - 1});
	UpdateBlock({coord.x,coord.y + 1});
}
void ActiveMap::UpdateHorizontal3(Type::Coord coord)
{
	UpdateBlock({coord.x,coord.y});
	UpdateBlock({coord.x - 1,coord.y});
	UpdateBlock({coord.x + 1,coord.y});
}
bool ActiveMap::TestObject(Type::Coord coord) const
{
	return map->Test(coord) && reach(map)[coord].object->isExists;
}
bool ActiveMap::TestRemain(Type::Coord coord) const
{
	return map->Test(coord) && reach(map)[coord].remain->isExists;
}









/*Object Interfaces*/

void ActiveMap::murphyMoved(ObjectBase *object)
{
	murphy = object;
}
void ActiveMap::murphyDead(ObjectBase *omurphy)
{
	if (murphy == omurphy)
	{
		murphy = nullptr;
	}
}
void ActiveMap::murphyVictory()
{
	if (murphy)
	{
		murphy = nullptr;
		victory = true;
	}
}

Type::Flags ActiveMap::GetBlockFlags(Type::Coord coord)
{
	if (map->Test(coord))
		return reach(map)[coord].grid;
	return 0;
}

ObjectBase *ActiveMap::GetObject(Type::Coord coord)
{
	if (map->Test(coord))
	{
		if (reach(map)[coord].object->isExists)
			return reach(map)[coord].object;
		else
			return &space;
	}
	return &bedrock;
}

ObjectBase *ActiveMap::GetObjectOut(Type::Coord coord)
{
	if (map->Test(coord))
	{
		if (reach(map)[coord].GoTo != coord && reach(map)[reach(map)[coord].GoTo].object->isExists)
			return reach(map)[reach(map)[coord].GoTo].object;
		return &space;
	}
	return &bedrock;
}

ObjectBase *ActiveMap::GetRemain(Type::Coord coord)
{
	if (map->Test(coord))
	{
		if (reach(map)[coord].remain->isExists)
			return reach(map)[coord].remain;
		else
			return &space;
	}
	return &bedrock;
}

Type::Flags ActiveMap::GetUnionFlags(Type::Coord coord)
{
	return GetObject(coord)->GetFlags() | GetObjectOut(coord)->GetFlags() | GetRemain(coord)->GetFlags();
}

Type::Flags ActiveMap::GetSectionFlags(Type::Coord coord)
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

void ActiveMap::ObjectMove(Type::Coord from, Type::Coord to, Type::ID remain)
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

void ActiveMap::ObjectPut(Type::Coord coord, Type::ID create)
{
	if (TestObject(coord))
	{
		ObjectCreate(reach(map)[coord].object, create, coord);

		UpdateSquare33(coord);
		Redrawn(coord);
	}
}

void ActiveMap::RemainPut(Type::Coord coord, Type::ID create)
{
	if (map->Test(coord))
	{
		DeleteRemain(coord);
		ObjectCreate(reach(map)[coord].remain, create, coord);

		UpdateSquare33(coord);
		Redrawn(coord);
	}
}

void ActiveMap::ObjectArrived(Type::Coord coord)
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

void ActiveMap::ObjectDisappear(Type::Coord coord)
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

Type::Size ActiveMap::GetDrawSize() const
{
	return drawer.GetDrawSize();
}
bool ActiveMap::EnableUpdateSkip()
{
	return enableupdateskip;
}

bool ActiveMap::IsObjectOut(Type::Coord coord)
{
	return (map->Test(coord) && coord != reach(map)[coord].GoTo);
}

Type::Coord ActiveMap::GetGoto(Type::Coord coord)
{
	if (map->Test(coord))
		return reach(map)[coord].GoTo;
	return coord;
}

Type::Coord ActiveMap::GetComefrom(Type::Coord coord)
{
	if (map->Test(coord))
		return reach(map)[coord].ComeFrom;
	return coord;
}

Type::Size ActiveMap::MapSize()
{
	return (Type::Size)(reach(map));
}

ObjectBase *ActiveMap::GetObjectU(Type::Coord coord)
{
	return reach(map)[coord].object;
}

Type::Size ActiveMap::GetDrawOffSet() const
{
	return drawer.GetDrawOffSet();
}
void ActiveMap::AddUnity(int collect)
{
	statusbar->AddUnity(collect);
}

void ActiveMap::AddAim(int collect)
{
	statusbar->AddAim(collect);
}

int ActiveMap::GetUnityCount() const
{
	return statusbar->GetUnityCount();
}
int ActiveMap::GetAimRemaining() const
{
	return statusbar->GetAimRemaining();
}
bool ActiveMap::IamRemain(ObjectBase *o)
{
	if (map->Test(o->GetCoord()))
	{
		if (reach(map)[o->GetCoord()].object == o)
			return false;
		if (reach(map)[o->GetCoord()].remain == o)
			return true;
	}
	return true;
}
bool ActiveMap::IsGlobalGravity() const
{
	return globalGravity;
}
void ActiveMap::switchGravity()
{
	globalGravity = !globalGravity;
	drawer.setGlobalGravity(globalGravity);
}

bool ActiveMap::rollTrigger(ObjectBase *obj_, unsigned __int16 typeID, float chancePerSec)
{
	return rngController->rollTrigger(obj_->rootId, typeID, chancePerSec);
}
