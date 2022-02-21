#include "ActiveMap.h"
#include "MainEvent.h"
#include "Global.h"

#include <random>

#define MAP (*map)

//StatisticsTimer drawStatistics;

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
			if (MAP[coord].object->isExists)
				MAP[coord].object->Print();
			if (MAP[coord].remain->isExists)
				MAP[coord].remain->Print();
			clog << "- - - - - - - - - - - - - - -" << KIR4::eol;
		}

		return false;
	};

	fncMoved = [&](FNC_MOVED_PARAMS)
	{
		drawnerPanel->move(0, 0, width(), height() - statusbar->Height());
		drawer.InitializeDrawOptions(drawnerPanel->width(), drawnerPanel->height() - statusbar->Height(), CA);
		statusbar->Align();
	};

	fncKeyDown = [&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
	{
		if (player)
		{
			if (key_ == ALLEGRO_KEY_UP)
				Object::Player::SetMoveUp(player, true);
			else if (key_ == ALLEGRO_KEY_DOWN)
				Object::Player::SetMoveDown(player, true);
			else if (key_ == ALLEGRO_KEY_LEFT)
				Object::Player::SetMoveLeft(player, true);
			else if (key_ == ALLEGRO_KEY_RIGHT)
				Object::Player::SetMoveRight(player, true);
			else if (key_ == ALLEGRO_KEY_SPACE)
				Object::Player::SetSpell(player, true);
			else if (key_ == ALLEGRO_KEY_ESCAPE)
			{
				if (player)
				{
					player->requests.blowUp = true;
					player->hitCoord = player->GetCoord();
					Blasting(player->GetCoord());
				}
			}
		}
		return false;
	};

	fncKeyUp = [&](FNC_KEY_UP_PARAMS)->FNC_KEY_UP_RET
	{
		if (player)
		{
			if (key_ == ALLEGRO_KEY_UP)
				Object::Player::SetMoveUp(player, false);
			else if (key_ == ALLEGRO_KEY_DOWN)
				Object::Player::SetMoveDown(player, false);
			else if (key_ == ALLEGRO_KEY_LEFT)
				Object::Player::SetMoveLeft(player, false);
			else if (key_ == ALLEGRO_KEY_RIGHT)
				Object::Player::SetMoveRight(player, false);
			else if (key_ == ALLEGRO_KEY_SPACE)
				Object::Player::SetSpell(player, false);
		}
		return false;
	};

	fncTimer = [&](FNC_TIMER_PARAMS)
	{
		if (startLoop == 0)
		{
			if (player)
			{
				ObjectBase::Stack stack;
				stack.o = player;
				stack.specific = player->specific;
				Object::Player::Controll(&stack);
			}

			buildObjectsHolder();

			for (auto &o : objects)
			{
				if (o->isExists && o->events.timer)
				{
					o->RunTimer();
				}
			}
			for (auto &o : remains)
			{
				if (o->isExists && o->events.timer)
				{
					o->RunTimer();
				}
			}

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

			if (!player)
			{
				if (stopLoop == 0)
				{
					mainEvent->mapFinished(victory, loopCounter);
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

			for (auto &o : objects)
			{
				if (o->isExists && o->events.tick)
				{
					o->RunTick();
				}
			}
			for (auto &o : remains)
			{
				if (o->isExists && o->events.tick)
				{
					o->RunTick();
				}
			}
		}
	};

	fncKeyChar = [&](FNC_KEY_CHAR_PARAMS)->FNC_KEY_CHAR_RET
	{
		return false;
	};

	drawnerPanel->fncDraw = [&](FNC_DRAW_PARAMS)
	{
		if (player)
		{
			drawer.MoveCameraTo({player->GetCoord().x + player->GetMove().x, player->GetCoord().y + player->GetMove().y});
		}
		drawer.DrawBlocks(x_, y_);
		if (startLoop > 0)
		{
			al_draw_filled_rectangle(x_, y_, x_ + w_, y_ + h_, KIR5::Color(0, 0, 0, 255 * (startLoop / (float)startLoopInit)).getAlphaColored());
		}
		else
		{
			if (!player)
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
void ActiveMap::startMap(const BluePrint &disp_map)
{
	victory = false;

	loopCounter = 0;
	startLoop = startLoopInit;
	stopLoop = stopLoopInit;

	std::vector<Type::Coord> spawns;
	map.reset(new Array2D<ActiveBlock<ObjectBase>>(disp_map));
	objects.resize(((Type::Size)*map).width * ((Type::Size)*map).height);
	remains.resize(objects.size());
	map->foreach([&](const Type::Coord &coord, ActiveBlock<ObjectBase> &block)
	{
		block.object = new ObjectBase(*this);
		ObjectCreate(block.object, disp_map[coord].id, coord);
		block.object->rotation = disp_map[coord].rotation;
		block.object->requests.update;
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
	ObjectCreate(MAP[spawn].object, ObjectID::Player, spawn);
	player = MAP[spawn].object;

	//SetPlayer(player);
	statusbar->SetMap(disp_map);

	drawer.SetMap(map);
	drawer.InitializeDrawOptions(width(), height() - statusbar->Height(), CA);
}

void ActiveMap::Redrawn(Type::Coord coord)
{
	drawer.Redrawn(coord);
}



void ActiveMap::CopyObject(Type::Coord coordDst, Type::Coord coordSrc)
{
	if (MAP.Test(coordDst) && MAP.Test(coordSrc))
	{
		ObjectBase *objTmp = MAP[coordDst].object;
		MAP[coordDst].object = MAP[coordSrc].object;
		MAP[coordSrc].object = objTmp;

		MAP[coordDst].object->coord = coordDst;
		MAP[coordSrc].object->coord = coordSrc;
	}
}
void ActiveMap::CopyRemain(Type::Coord coordDst, Type::Coord coordSrc)
{
	if (MAP.Test(coordDst) && MAP.Test(coordSrc))
	{
		ObjectBase *objTmp = MAP[coordDst].remain;
		MAP[coordDst].remain = MAP[coordSrc].remain;
		MAP[coordSrc].remain = objTmp;

		MAP[coordDst].remain->coord = coordDst;
		MAP[coordSrc].remain->coord = coordSrc;
	}
}
void ActiveMap::CopyObjectToRemain(Type::Coord coordDst, Type::Coord coordSrc)
{
	if (MAP.Test(coordDst) && MAP.Test(coordSrc))
	{
		ObjectBase *objTmp = MAP[coordDst].remain;
		MAP[coordDst].remain = MAP[coordSrc].object;
		MAP[coordSrc].object = objTmp;

		MAP[coordDst].remain->coord = coordDst;
		MAP[coordSrc].object->coord = coordSrc;
	}
}
void ActiveMap::CopyRemainToObject(Type::Coord coordDst, Type::Coord coordSrc)
{
	if (MAP.Test(coordDst) && MAP.Test(coordSrc))
	{
		ObjectBase *objTmp = MAP[coordDst].object;
		MAP[coordDst].object = MAP[coordSrc].remain;
		MAP[coordSrc].remain = objTmp;

		MAP[coordDst].object->coord = coordDst;
		MAP[coordSrc].remain->coord = coordSrc;
	}
}




void ActiveMap::ExplosionPut(Type::Coord coord, Type::ID IDto)
{
	if (MAP.Test(coord) && MAP[coord].object->requests.blowUp == false)
	{
		if (player == MAP[coord].object)
		{
			playerDead(player);
		}

		Type::ID Expid = ObjectID::Space;
		if (MAP[coord].remain->isExists)
		{
			Expid = MAP[coord].remain->id;
		}

		DeleteRemain(coord);
		if (MAP[coord].object->GetFlags() & ObjectBase::Flags::CanBeExplosion)
		{
			ObjectCreate(MAP[coord].remain, ObjectID::ExplosionExpansive, coord);
			if (/*!(MAP[coord].object->GetFlags()&Object::Flags::ExplosionType1) && */IDto != 0 && (MAP[coord].object->GetTranslationTo() == 0 || rand() % 2))
			{
				MAP[coord].object->SetObjectIDremain(IDto);
			}
		}
		else if (Expid == ObjectID::Explosion || Expid == ObjectID::ExplosionEffect || Expid == ObjectID::ExplosionExpansive)
		{
			ObjectCreate(MAP[coord].remain, ObjectID::ExplosionEffect, coord);
		}
		Redrawn(coord);
	}
}
void ActiveMap::Blasting(Type::Coord coord)
{
	if (MAP.Test(coord) && MAP[coord].object->isExists)
	{
		Type::Flags flag = MAP[coord].object->GetFlags();
		Type::ID IDto = MAP[coord].object->GetTranslationTo();
		Type::Coord center = MAP[coord].object->GetHitCoord();

		MAP[coord].object->RemoveFlags(ObjectBase::Flags::ExplosionType1);
		MAP[coord].object->SetTranslationID(ObjectID::Space);

		//bool
		//	exploseNow = MAP[coord].remain && (MAP[coord].remain->ID() == ObjectID::Explosion || MAP[coord].remain->ID() == ObjectID::ExplosionExpansive);

		ExplosionPut(center, IDto);

		//if (exploseNow)
		//{
		//	Type::ID
		//		remainID = MAP[coord].object->GetTranslationTo();
		//	ObjectPut(coord, ObjectSpace::Create);
		//	MAP[coord].object->SetTranslationID(remainID);
		//}

		if (flag & ObjectBase::ExplosionType3)
		{
			MAP[coord].object->RemoveFlags(ObjectBase::Flags::ExplosionType3);
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
			MAP[coord].object->RemoveFlags(ObjectBase::Flags::ExplosionType5);
		}
		UpdateSquare33({center.x + 1,center.y + 1});
		UpdateSquare33({center.x - 1,center.y + 1});
		UpdateSquare33({center.x + 1,center.y - 1});
		UpdateSquare33({center.x - 1,center.y - 1});
	}
}
void ActiveMap::ObjectVirtualArrived(Type::Coord coord)
{
	if (MAP.Test(coord) && MAP[coord].ComeFrom != coord)
	{
		Redrawn(coord);

		MAP[MAP[coord].ComeFrom].GoTo = MAP[coord].ComeFrom;
		MAP[coord].ComeFrom = coord;

		MAP[coord].object->actions.move = false;

		UpdateSquare33(coord);
	}
}
void ActiveMap::StepDisappear(Type::Coord coord)
{
	if (TestObject(coord))
	{
		if (MAP[coord].object->id == 0)
		{
			if (MAP[coord].ComeFrom != coord)
			{
				MAP[MAP[coord].ComeFrom].GoTo = MAP[coord].ComeFrom;
				MAP[coord].ComeFrom = coord;
			}
		}
		else
		{
			DeleteRemain(coord);
			CopyObjectToRemain(coord, coord);
			//(*MAP[coord].remain) = MAP[coord].object;
		}
		MAP[coord].object->isExists = false;
	}
}
//void ActiveMap::SetObject(Type::Coord coord, ObjectBase *object)
//{
//	//MAP[coord].object->isExists=false;
//	CopyObject
//	//(*MAP[coord].object) = object;
//	Redrawn(coord);
//}
void ActiveMap::DeleteObject(Type::Coord coord)
{
	if (TestObject(coord))
	{
		ObjectCreate(MAP[coord].object, MAP[coord].object->GetTranslationTo(), coord);
		if (MAP[coord].ComeFrom != coord)
		{
			MAP[MAP[coord].ComeFrom].GoTo = MAP[coord].ComeFrom;
			MAP[coord].ComeFrom = coord;
		}
		Redrawn(coord);
		UpdateSquare33(coord);
	}
}
void ActiveMap::DeleteRemain(Type::Coord coord)
{
	if (TestRemain(coord))
	{
		MAP[coord].remain->isExists = false;
		//Delete(MAP[coord].remain);
		Redrawn(coord);
		UpdateSquare33(coord);
	}
}



void ActiveMap::UpdateRun()
{
	enableupdateskip = true;
	map->foreach([&](const Type::Coord &coord, ActiveBlock<ObjectBase> &block)
	{
		if (block.object->isExists && GetObject(coord)->requests.update && !block.object->IsMoving())
		{
			GetObject(coord)->Update();
		}
	});

	enableupdateskip = false;
	map->reverse_foreach([&](const Type::Coord &coord, ActiveBlock<ObjectBase> &block)
	{
		if (block.object->isExists && block.object->requests.update && !block.object->IsMoving())
		{
			block.object->Update();
		}
		if (block.remain->isExists && block.remain->requests.update && !block.remain->IsMoving())
		{
			block.remain->Update();
		}
	});
}

void ActiveMap::UpdateBlock(Type::Coord coord)
{
	if (MAP.Test(coord))
	{
		if (MAP[coord].object->isExists)
		{
			MAP[coord].object->requests.update = true;
		}
		if (MAP[coord].remain->isExists)
		{
			MAP[coord].remain->requests.update = true;
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
	return MAP.Test(coord) && MAP[coord].object->isExists;
}
bool ActiveMap::TestRemain(Type::Coord coord) const
{
	return MAP.Test(coord) && MAP[coord].remain->isExists;
}









/*Object Interfaces*/

void ActiveMap::playerMoved(ObjectBase *object)
{
	player = object;
}
void ActiveMap::playerDead(ObjectBase *oplayer)
{
	if (player == oplayer)
	{
		player = nullptr;
	}
}
void ActiveMap::playerVictory()
{
	if (player)
	{
		player = nullptr;
		victory = true;
	}
}

Type::Flags ActiveMap::GetBlockFlags(Type::Coord coord)
{
	if (MAP.Test(coord))
		return MAP[coord].grid;
	return 0;
}

ObjectBase *ActiveMap::GetObject(Type::Coord coord)
{
	if (MAP.Test(coord))
	{
		if (MAP[coord].object->isExists)
			return MAP[coord].object;
		else
			return &space;
	}
	return &bedrock;
}

ObjectBase *ActiveMap::GetObjectOut(Type::Coord coord)
{
	if (MAP.Test(coord))
	{
		if (MAP[coord].GoTo != coord && MAP[MAP[coord].GoTo].object->isExists)
			return MAP[MAP[coord].GoTo].object;
		return &space;
	}
	return &bedrock;
}

ObjectBase *ActiveMap::GetRemain(Type::Coord coord)
{
	if (MAP.Test(coord))
	{
		if (MAP[coord].remain->isExists)
			return MAP[coord].remain;
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
	if (MAP.Test(coord))
	{
		Type::Flags
			flags = MAP[coord].object->GetFlags();
		if (MAP[coord].remain->isExists)
			flags &= MAP[coord].remain->GetFlags();
		if (MAP[coord].GoTo != coord)
			flags &= MAP[MAP[coord].GoTo].object->GetFlags();
		return flags;
	}
	return bedrock.GetFlags();
}

void ActiveMap::ObjectMove(ObjectBase::Stack *stack, Type::Coord from, Type::Coord to, Type::ID remain)
{
	if (from != to && TestObject(from) && MAP.Test(to))
	{
		StepDisappear(to);

		CopyObject(to, from);
		//(*MAP[to].object) = MAP[from].object;
		if (stack)
		{
			stack->o = MAP[to].object;
			stack->specific = stack->o->specific;
		}
		ObjectCreate(MAP[from].object, remain, from);

		MAP[to].ComeFrom = from;
		MAP[from].GoTo = to;

		MAP[to].object->SetCoord(to);
		MAP[to].object->actions.move = true;

		UpdateSquare33(from);
		UpdateSquare33(to);
	}
}

void ActiveMap::ObjectPut(Type::Coord coord, Type::ID create)
{
	if (TestObject(coord))
	{
		ObjectCreate(MAP[coord].object, create, coord);

		UpdateSquare33(coord);
		Redrawn(coord);
	}
}

void ActiveMap::RemainPut(Type::Coord coord, Type::ID create)
{
	if (MAP.Test(coord))
	{
		DeleteRemain(coord);
		ObjectCreate(MAP[coord].remain, create, coord);

		UpdateSquare33(coord);
		Redrawn(coord);
	}
}

void ActiveMap::ObjectArrived(Type::Coord coord)
{
	if (MAP.Test(coord) && MAP[coord].ComeFrom != coord)
	{
		Redrawn(coord);
		Type::Coord
			ComeFrom = MAP[coord].ComeFrom;

		MAP[MAP[coord].ComeFrom].GoTo = MAP[coord].ComeFrom;
		MAP[coord].ComeFrom = coord;

		DeleteRemain(coord);

		MAP[coord].object->actions.move = false;

		UpdateSquare33(ComeFrom);
	}
	UpdateSquare33(coord);//bel�lr�l ki lett szedve, ugyanis mas okokbol is ezkezhet meg, pl ollo mozgasa fordulasbol
}

void ActiveMap::ObjectDisappear(Type::Coord coord)
{
	if (TestObject(coord) && !MAP[coord].object->IsMoving())
	{
		CopyObjectToRemain(coord, coord);
		//(*MAP[coord].remain) = MAP[coord].object;
		MAP[coord].remain->requests.remove = true;

		ObjectCreate(MAP[coord].object, MAP[coord].object->GetTranslationTo(), coord);

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
	return (MAP.Test(coord) && coord != MAP[coord].GoTo);
}

Type::Coord ActiveMap::GetGoto(Type::Coord coord)
{
	if (MAP.Test(coord))
		return MAP[coord].GoTo;
	return coord;
}

Type::Coord ActiveMap::GetComefrom(Type::Coord coord)
{
	if (MAP.Test(coord))
		return MAP[coord].ComeFrom;
	return coord;
}

Type::Size ActiveMap::MapSize()
{
	return (Type::Size)(MAP);
}

ObjectBase *ActiveMap::GetObjectU(Type::Coord coord)
{
	return MAP[coord].object;
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
	if (MAP.Test(o->GetCoord()))
	{
		if (MAP[o->GetCoord()].object == o)
			return false;
		if (MAP[o->GetCoord()].remain == o)
			return true;
	}
	return true;
}

std::random_device rd;
std::default_random_engine generator(rd());

bool ActiveMap::rollTrigger(ObjectBase *obj_, float chancePerSec)
{
	std::uniform_int_distribution<unsigned> distribution(0, (int)(CPS * 100000));
	return distribution(generator) <= (unsigned)(chancePerSec * 100000);
}