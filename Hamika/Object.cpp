#include "Object.h"

//void ObjectBase::operator=(const ObjectBase *o)
//{
//	memcpy(this->specific, o->specific, sizeof(this->specific));
//
//	this->DrawNumber = o->DrawNumber;
//	this->UpdateNumber = o->UpdateNumber;
//	this->DrawnedCount = o->DrawnedCount;
//
//	this->name = o->name;
//
//	this->printFnc = o->printFnc;
//	this->timerFnc = o->timerFnc;
//	this->tickerFnc = o->tickerFnc;
//	this->updaterFnc = o->updaterFnc;
//	this->drawnerFnc = o->drawnerFnc;
//
//	this->MoveSpeed = o->MoveSpeed;
//	this->RotationSpeed = o->RotationSpeed;
//	this->TranslationTo = o->TranslationTo;
//	this->ObjectIDremain = o->ObjectIDremain;
//	this->rotation = o->rotation;
//	this->coord = o->coord;
//	//this->hitCoord=o->hitCoord;
//	this->DrawCoord = o->DrawCoord;
//	this->move = o->move;
//	this->flags = o->flags;
//	this->events = o->events;
//	this->id = o->id;
//	this->currentspeed = o->currentspeed;
//	this->limitspeed = o->limitspeed;
//
//	this->isExists = o->isExists;
//}

void ObjectBase::PrintFlags(Type::Flags flags_)
{
	struct F
	{
		Type::Flags f;
		const char *n;
	} flags[] = {
		{StepOn,"StepOn"},{MurphyDies,"MurphyDies"},{CanPushUp,"CanPushUp"},{CanPushDown,"CanPushDown"},{CanPushRight,"CanPushRight"},{CanPushLeft,"CanPushLeft"},{CanPush,"CanPush"},{MurphyStepOn,"MurphyStepOn"},{RollOffTop,"RollOffTop"},{RollOffBottom,"RollOffBottom"},{RollOff,"RollOff"},{PassageFromRight,"PassageFromRight"},{PassageFromLeft,"PassageFromLeft"},{PassageFromTop,"PassageFromTop"},{PassageFromBottom,"PassageFromBottom"},{PassageVertical,"PassageVertical"},{PassageHorizontal,"PassageHorizontal"},{Passage,"Passage"},{FallOnExplosion,"FallOnExplosion"},{ExplosionType1,"ExplosionType1"},{ExplosionType3,"ExplosionType3"},{ExplosionType5,"ExplosionType5"},{CanBeExplosion,"CanBeExplosion"},{LimitSpeed,"LimitSpeed"},{PhysicsSpeed,"PhysicsSpeed"},{InstantSpeed,"InstantSpeed"},{MurphyCanSniff,"MurphyCanSniff"},{CanBeKilled,"CanBeKilled"},{GiveGravityDelay,"GiveGravityDelay"},{ButtonPush,"ButtonPush"},{Give1Aim,"Give1Aim"},{Give1Unity,"Give1Unity"},
	};
	for (int i = 0; i < sizeof(F) / sizeof(flags[0]); i++)
	{
		if (flags_ & flags[i].f)
		{
			clog << flags[i].n << ", ";
		}
	}
}

ObjectBase::ObjectBase(
	Interface &ief
):
	ObjectDrawModule(&ief),
	ief(ief)
{

}
bool ObjectBase::Roll(double PpM)
{
	return rand() % 1000 <= PpM / CPS * 10.;
}

void ObjectBase::Update()
{
	requests.update = false;
	UpdateNumber = SUpdateNumber++;
	RunUpdate();
}
ObjectBase::~ObjectBase()
{
}

void ObjectBase::StartRotation()
{
	actions.rotate = true;
}
bool ObjectBase::RotationLeft()
{
	if (rotation == Type::Rotations::_0)
		rotation = Type::Rotations::_360;
	if (rotation > Type::Rotations::_270)
	{
		rotation -= GetRotationSpeed() / CPS;
		if (rotation <= Type::Rotations::_270)
		{
			rotation = Type::Rotations::_270;
			actions.rotate = false;
			return true;
		}
	}
	else if (rotation > Type::Rotations::_180)
	{
		rotation -= GetRotationSpeed() / CPS;
		if (rotation <= Type::Rotations::_180)
		{
			rotation = Type::Rotations::_180;
			actions.rotate = false;
			return true;
		}
	}
	else if (rotation > Type::Rotations::_90)
	{
		rotation -= GetRotationSpeed() / CPS;
		if (rotation <= Type::Rotations::_90)
		{
			rotation = Type::Rotations::_90;
			actions.rotate = false;
			return true;
		}
	}
	else
	{
		rotation -= GetRotationSpeed() / CPS;
		if (rotation <= Type::Rotations::_0)
		{
			rotation = Type::Rotations::_0;
			actions.rotate = false;
			return true;
		}
	}
	return false;
}
bool ObjectBase::RotationRight()
{
	if (rotation < Type::Rotations::_90)
	{
		rotation += GetRotationSpeed() / CPS;
		if (rotation >= Type::Rotations::_90)
		{
			rotation = Type::Rotations::_90;
			actions.rotate = false;
			return true;
		}
	}
	else if (rotation < Type::Rotations::_180)
	{
		rotation += GetRotationSpeed() / CPS;
		if (rotation >= Type::Rotations::_180)
		{
			rotation = Type::Rotations::_180;
			actions.rotate = false;
			return true;
		}
	}
	else if (rotation < Type::Rotations::_270)
	{
		rotation += GetRotationSpeed() / CPS;
		if (rotation >= Type::Rotations::_270)
		{
			rotation = Type::Rotations::_270;
			actions.rotate = false;
			return true;
		}
	}
	else
	{
		rotation += GetRotationSpeed() / CPS;
		if (rotation >= Type::Rotations::_360)
		{
			rotation = Type::Rotations::_0;
			actions.rotate = false;
			return true;
		}
	}
	return false;
}

//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 1 jelent egy teljes n�gyzetet, 2.5: k�t �s f�l n�gyzet m�sodpercenk�nt....
void ObjectBase::SetMoveSpeed(Type::Move::Type speed)
{
	MoveSpeed = {speed,speed};
}
//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 1 jelent egy teljes n�gyzetet, 2.5: k�t �s f�l n�gyzet m�sodpercenk�nt....
void ObjectBase::SetMoveSpeed(Type::Move speed)
{
	MoveSpeed = speed;
}
//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 360 egy teljes k�rbefordul�s 1m�sodperc alatt...
void ObjectBase::SetRotationSpeed(Type::Speed speed)
{
	RotationSpeed = speed;
}
void ObjectBase::SetTranslationID(Type::ID id)
{
	TranslationTo = id;
}
void ObjectBase::SetObjectIDremain(Type::ID id)
{
	ObjectIDremain = id;
}

void ObjectBase::SetCoord(Type::Coord coord)
{
	this->coord = coord;
	SetDrawCoord();
}
void ObjectBase::SetRotation(Type::Rotation rotation)
{
	this->rotation = rotation;
}

//GET
//1 m�sik oldalon m�g teljesen  0 nyugalmi �llapot
float ObjectBase::GetAbsMove()
{
	float
		maxX = std::abs(move.x),
		maxY = std::abs(move.y);
	if (maxX > maxY)
		return maxX;
	else
		return maxY;
}

bool ObjectBase::IsRotating()
{
	return actions.rotate;
}

Type::Coord ObjectBase::GetForwardCoord()
{
	Type::Rotation
		rotation = this->rotation + 0.01f/*hibat�r�s*/;

	if (rotation >= Type::Rotations::_315)
		return GetCoordUp();
	else if (rotation >= Type::Rotations::_225)
		return GetCoordLeft();
	else if (rotation >= Type::Rotations::_135)
		return GetCoordDown();
	else if (rotation >= Type::Rotations::_45)
		return GetCoordRight();
	else
		return GetCoordUp();
}
Type::Coord ObjectBase::GetForwardCoord(Type::Rotation rotation)
{
	rotation += 0.01f/*hibat�r�s*/;

	if (rotation >= Type::Rotations::_315)
		return GetCoordUp();
	else if (rotation >= Type::Rotations::_225)
		return GetCoordLeft();
	else if (rotation >= Type::Rotations::_135)
		return GetCoordDown();
	else if (rotation >= Type::Rotations::_45)
		return GetCoordRight();
	else
		return GetCoordUp();
}
Type::Rotation ObjectBase::GetRoundRotation(Type::Rotation rotation)
{
	rotation += 0.01f/*hibat�r�s*/;

	if (rotation >= Type::Rotations::_315)
		return Type::Rotations::Up;
	else if (rotation >= Type::Rotations::_225)
		return Type::Rotations::Left;
	else if (rotation >= Type::Rotations::_135)
		return Type::Rotations::Down;
	else if (rotation >= Type::Rotations::_45)
		return Type::Rotations::Right;
	else
		return Type::Rotations::Up;
}
Type::Move ObjectBase::GetMoveSpeed()
{
	return MoveSpeed;
}
Type::Move::Type ObjectBase::GetMoveSpeedVertical()
{
	return MoveSpeed.y;
}
Type::Move::Type ObjectBase::GetMoveSpeedHorizontal()
{
	return MoveSpeed.x;
}
Type::Speed ObjectBase::GetRotationSpeed()
{
	return RotationSpeed;
}
Type::ID ObjectBase::GetTranslationTo()
{
	return TranslationTo;
}
Type::ID ObjectBase::GetObjectIDremain()
{
	return ObjectIDremain;
}
Type::Rotation ObjectBase::GetAngel()
{
	return rotation / Type::Rotations::_360;
}
void ObjectBase::Print()
{
	clog.color(KIR4::LWHITE);
	clog << "	Name: " << name << "\n";
	clog << "	Address: " << this << "\n";
	clog << "		MoveX: " << move.x << "\n";
	clog << "		MoveY: " << move.y << "\n";
	clog << "		Rotation: " << rotation << "\n";
	clog << "		Flags: ";
	PrintFlags(flags);
	clog << "\n";
	clog << "		TranslationTo: " << GetTranslationTo() << "\n";
	clog << "		ObjectIDremain: " << GetObjectIDremain() << "\n";
	clog << "		Status: " << GetAbsMove() << "\n";
	clog << "		IsMoving: " << IsMoving() << "\n";
	clog << "			IsMoveLeft: " << IsMoveLeft() << "\n";
	clog << "			IsMoveRight: " << IsMoveRight() << "\n";
	clog << "			IsMoveDown: " << IsMoveDown() << "\n";
	clog << "			IsMoveUp: " << IsMoveUp() << "\n";
	clog << "		Events: ";
	if (events.timer) clog << "timer, ";
	if (events.tick) clog << "tick, ";
	if (events.update) clog << "update, ";
	if (events.topDraw) clog << "topDraw, ";
	clog << "\n";
	clog << "		Actions: ";
	if (actions.move) clog << "move, ";
	if (actions.rotate) clog << "rotate, ";
	clog << "\n";
	clog << "		Requests: ";
	if (requests.remove) clog << "timer, ";
	if (requests.remove) clog << "tick, ";
	if (requests.remove) clog << "update, ";
	if (requests.remove) clog << "draw, ";
	if (requests.remove) clog << "remove, ";
	if (requests.remove) clog << "blowUp, ";
	clog << "\n";
	clog << "		MoveSpeed: " << GetMoveSpeed() << "\n";
	clog << "		RotationSpeed: " << GetRotationSpeed() << "\n";
	clog << "	DrawNumber: " << DrawNumber << "\n";
	clog << "		DrawCoordX: " << DrawCoord.x << "\n";
	clog << "		DrawCoordY: " << DrawCoord.y << "\n";
	clog << "	UpdateNumber: " << UpdateNumber << "\n";
	clog.color(KIR4::LPURPLE);
	RunPrinter();
}
void ObjectBase::Initialize()
{
}

void ObjectBase::IncreaseSpeed(Type::Move::Type max)
{
	if (GetFlags() & LimitSpeed)
		max = limitspeed;

	if (GetFlags() & PhysicsSpeed)
	{
		currentspeed += 0.1f;
		if (currentspeed >= max)
			currentspeed = max;
	}
	else if (GetFlags() & InstantSpeed)
		currentspeed = max;
}
bool ObjectBase::CanMovePosByRotationH(Type::Coord to, Type::Rotation rotation)
{
	ObjectBase *out = ief.GetObjectOut(to);
	return
		((rotation == Type::Rotations::Up && out->GetCoord().y < to.y)
		 ||
		 (rotation == Type::Rotations::Down && out->GetCoord().y > to.y)
		 ||
		 (rotation == Type::Rotations::Left && out->GetCoord().x < to.x)
		 ||
		 (rotation == Type::Rotations::Right && out->GetCoord().x > to.x)
		 );
}

bool ObjectBase::CanMovePos(Type::Coord to, Type::Rotation rotation)
{
	return
		ief.GetObject(to)->GetFlags() & StepOn
		&&
		ief.GetRemain(to)->GetFlags() & StepOn
		&&
		(
			ief.GetObjectOut(to)->GetFlags() & StepOn
			||
			CanMovePosByRotationH(to, rotation)
			);
}
bool ObjectBase::CanMoveDown()
{
	return
		ief.GetObject(GetCoordDown())->GetFlags() & StepOn
		&&
		ief.GetRemain(GetCoordDown())->GetFlags() & StepOn
		&&
		(
			ief.GetObjectOut(GetCoordDown())->GetFlags() & StepOn
			||
			ief.GetObjectOut(GetCoordDown())->IsMoveDown()
			);
}
bool ObjectBase::CanMoveUp()
{
	return
		ief.GetObject(GetCoordUp())->GetFlags() & StepOn
		&&
		ief.GetRemain(GetCoordUp())->GetFlags() & StepOn
		&&
		(
			ief.GetObjectOut(GetCoordUp())->GetFlags() & StepOn
			||
			ief.GetObjectOut(GetCoordUp())->IsMoveUp()
			);
}
bool ObjectBase::CanMoveLeft()
{
	return
		ief.GetObject(GetCoordLeft())->GetFlags() & StepOn
		&&
		ief.GetRemain(GetCoordLeft())->GetFlags() & StepOn
		&&
		(
			ief.GetObjectOut(GetCoordLeft())->GetFlags() & StepOn
			||
			ief.GetObjectOut(GetCoordLeft())->IsMoveLeft()
			);
}
bool ObjectBase::CanMoveRight()
{
	return
		ief.GetObject(GetCoordRight())->GetFlags() & StepOn
		&&
		ief.GetRemain(GetCoordRight())->GetFlags() & StepOn
		&&
		(
			ief.GetObjectOut(GetCoordRight())->GetFlags() & StepOn
			||
			ief.GetObjectOut(GetCoordRight())->IsMoveRight()
			);
}



//akkor kell megh�vni ha nicns tov�bbi l�p�s, azaz meg�ll az object
void ObjectBase::StopStep()
{
	currentspeed = 0;
	hitactive = false;
	AutoStepHit();
}
void ObjectBase::DisablePhysics()
{
	RemoveFlags(PhysicsSpeed);
	AddFlags(InstantSpeed);
}
void ObjectBase::EnablePhysics()
{
	AddFlags(PhysicsSpeed);
	RemoveFlags(InstantSpeed);
}
void ObjectBase::EnebleLimitSpeed(Type::Move::Type max)
{
	AddFlags(LimitSpeed);
	limitspeed = max;
}
void ObjectBase::DisableLimitSpeed()
{
	RemoveFlags(LimitSpeed);
}

void ObjectBase::StepUp()
{
	IncreaseSpeed(GetMoveSpeed().y);
	SetMove({GetMove().x,GetMove().y - Type::Move::Type(currentspeed / CPS)});
	if (ief.IsObjectOut(GetCoord()) && ief.GetObjectOut(GetCoord())->GetMove().y > GetMove().y)
	{
		if (currentspeed > ief.GetObjectOut(GetCoord())->GetMoveSpeed().y)
			currentspeed = ief.GetObjectOut(GetCoord())->GetMoveSpeed().y;
		SetMove({GetMove().x,ief.GetObjectOut(GetCoord())->GetMove().y});
		if (!hitactive)
		{
			AutoStepHit();
			hitactive = true;
		}
	}
	else
		hitactive = false;
	if (GetMove().y <= 0)
	{
		//hitactive = false;
		SetMove({GetMove().x,0});
		ief.ObjectArrived(GetCoord());
		ObjectArrived();
	}
}
void ObjectBase::StepDown()
{
	IncreaseSpeed(GetMoveSpeed().y);
	SetMove({GetMove().x,GetMove().y + Type::Move::Type(currentspeed / CPS)});
	if (ief.IsObjectOut(GetCoord()) && ief.GetObjectOut(GetCoord())->GetMove().y < GetMove().y)
	{
		if (currentspeed > ief.GetObjectOut(GetCoord())->GetMoveSpeed().y)
			currentspeed = ief.GetObjectOut(GetCoord())->GetMoveSpeed().y;
		SetMove({GetMove().x,ief.GetObjectOut(GetCoord())->GetMove().y});
		if (!hitactive)
		{
			AutoStepHit();
			hitactive = true;
		}
	}
	else
		hitactive = false;
	if (GetMove().y >= 0)
	{
		//hitactive = false;
		SetMove({GetMove().x,0});
		ief.ObjectArrived(GetCoord());
		ObjectArrived();
	}
}
void ObjectBase::StepLeft()
{
	IncreaseSpeed(GetMoveSpeed().x);
	SetMove({GetMove().x - Type::Move::Type(currentspeed / CPS),GetMove().y});
	if (ief.IsObjectOut(GetCoord()) && ief.GetObjectOut(GetCoord())->GetMove().x > GetMove().x)
	{
		if (currentspeed > ief.GetObjectOut(GetCoord())->GetMoveSpeed().x)
			currentspeed = ief.GetObjectOut(GetCoord())->GetMoveSpeed().x;
		SetMove({ief.GetObjectOut(GetCoord())->GetMove().x,GetMove().y});
		if (!hitactive)
		{
			AutoStepHit();
			hitactive = true;
		}
	}
	else
		hitactive = false;
	if (GetMove().x <= 0)
	{
		//hitactive = false;
		SetMove({0,GetMove().y});
		ief.ObjectArrived(GetCoord());
		ObjectArrived();
	}
}
void ObjectBase::StepRight()
{
	IncreaseSpeed(GetMoveSpeed().x);
	SetMove({GetMove().x + Type::Move::Type(currentspeed / CPS),GetMove().y});
	if (ief.IsObjectOut(GetCoord()) && ief.GetObjectOut(GetCoord())->GetMove().x < GetMove().x)
	{
		if (currentspeed > ief.GetObjectOut(GetCoord())->GetMoveSpeed().x)
			currentspeed = ief.GetObjectOut(GetCoord())->GetMoveSpeed().x;
		SetMove({ief.GetObjectOut(GetCoord())->GetMove().x,GetMove().y});
		if (!hitactive)
		{
			AutoStepHit();
			hitactive = true;
		}
	}
	else
		hitactive = false;
	if (GetMove().x >= 0)
	{
		//hitactive = false;
		SetMove({0,GetMove().y});
		ief.ObjectArrived(GetCoord());
		ObjectArrived();
	}
}
void ObjectBase::Step()
{
	if (GetMove().x > 0)
		StepLeft();
	else if (GetMove().x < 0)
		StepRight();
	else if (GetMove().y > 0)
		StepUp();
	else if (GetMove().y < 0)
		StepDown();
}

bool isExists = false;

//nem �rkezett meg, hanme utol�rte az el�tte l�v�t
void ObjectBase::AutoStepHit()
{

}
void ObjectBase::ObjectArrived()
{

}


unsigned long long int ObjectBase::SUpdateNumber = 0;

