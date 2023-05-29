#include "Object.h"

void ObjectBase::PrintFlags(Type::Flags flags_)
{
	struct F
	{
		Type::Flags f;
		const char *n;
	} flags[] = {
		{StepOn,"StepOn"},{MurphyDies,"MurphyDies"},{CanPushUp,"CanPushUp"},{CanPushDown,"CanPushDown"},{CanPushRight,"CanPushRight"},
		{CanPushLeft,"CanPushLeft"},{CanPush,"CanPush"},{MurphyStepOn,"MurphyStepOn"},{RollOffTop,"RollOffTop"},
		{RollOffBottom,"RollOffBottom"},{RollOff,"RollOff"},{PassageFromRight,"PassageFromRight"},{PassageFromLeft,"PassageFromLeft"},
		{PassageFromTop,"PassageFromTop"},{PassageFromBottom,"PassageFromBottom"},{PassageVertical,"PassageVertical"},
		{PassageHorizontal,"PassageHorizontal"},{Passage,"Passage"},{FallOnExplosion,"FallOnExplosion"},
		{ExplosionType1,"ExplosionType1"},{ExplosionType3,"ExplosionType3"},{ExplosionType5,"ExplosionType5"},
		{CanBeExplosion,"CanBeExplosion"},{LimitSpeed,"LimitSpeed"},{PhysicsSpeed,"PhysicsSpeed"},
		{MurphyCanSniff,"MurphyCanSniff"},{CanBeKilled,"CanBeKilled"},{GiveGravityDelay,"GiveGravityDelay"},{ButtonPush,"ButtonPush"},
		{Give1Score,"Give1Score"},{Give1Unity,"Give1Unity"},
	};
	for (int i = 0; i < sizeof(F) / sizeof(flags[0]); i++)
	{
		if (flags_ & flags[i].f)
		{
			clog << flags[i].n << ", ";
		}
	}
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
void ObjectBase::Initialize()
{
}

void ObjectBase::calculateSpeed(Type::Move::Type _baseSpeed)
{
	if (GetFlags() & LimitSpeed)
	{
		currentSpeed_ = limitSpeed_;
	}
	else if (GetFlags() & PhysicsSpeed)
	{
		accelaratePercent_ = std::min(1.f, accelaratePercent_ + 0.15f);
		currentSpeed_ = _baseSpeed * accelaratePercent_;
	}
	else
	{
		currentSpeed_ = _baseSpeed;
	}
}
bool ObjectBase::CanMovePosByRotationH(Type::Coord to, Type::Rotation rotation)
{
	ObjectBase *out = scene->GetObjectOut(to);
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
		scene->GetObject(to)->GetFlags() & StepOn
		&&
		scene->GetRemain(to)->GetFlags() & StepOn
		&&
		(
			scene->GetObjectOut(to)->GetFlags() & StepOn
			||
			CanMovePosByRotationH(to, rotation)
			);
}
bool ObjectBase::CanMoveDown()
{
	return
		scene->GetObject(GetCoordDown())->GetFlags() & StepOn
		&&
		scene->GetRemain(GetCoordDown())->GetFlags() & StepOn
		&&
		(
			scene->GetObjectOut(GetCoordDown())->GetFlags() & StepOn
			||
			scene->GetObjectOut(GetCoordDown())->IsMoveDown()
			);
}
bool ObjectBase::CanMoveUp()
{
	return
		scene->GetObject(GetCoordUp())->GetFlags() & StepOn
		&&
		scene->GetRemain(GetCoordUp())->GetFlags() & StepOn
		&&
		(
			scene->GetObjectOut(GetCoordUp())->GetFlags() & StepOn
			||
			scene->GetObjectOut(GetCoordUp())->IsMoveUp()
			);
}
bool ObjectBase::CanMoveLeft()
{
	return
		scene->GetObject(GetCoordLeft())->GetFlags() & StepOn
		&&
		scene->GetRemain(GetCoordLeft())->GetFlags() & StepOn
		&&
		(
			scene->GetObjectOut(GetCoordLeft())->GetFlags() & StepOn
			||
			scene->GetObjectOut(GetCoordLeft())->IsMoveLeft()
			);
}
bool ObjectBase::CanMoveRight()
{
	return
		scene->GetObject(GetCoordRight())->GetFlags() & StepOn
		&&
		scene->GetRemain(GetCoordRight())->GetFlags() & StepOn
		&&
		(
			scene->GetObjectOut(GetCoordRight())->GetFlags() & StepOn
			||
			scene->GetObjectOut(GetCoordRight())->IsMoveRight()
			);
}



//akkor kell megh�vni ha nicns tov�bbi l�p�s, azaz meg�ll az object
void ObjectBase::StopStep()
{
	hitactive = false;
	carrySpeed_ = 0.f;
	AutoStepHit();
}
void ObjectBase::DisablePhysics()
{
	RemoveFlags(PhysicsSpeed);
}
void ObjectBase::EnablePhysics()
{
	AddFlags(PhysicsSpeed);
}
void ObjectBase::EnebleLimitSpeed(Type::Move::Type max)
{
	AddFlags(LimitSpeed);
	limitSpeed_ = max;
}
void ObjectBase::DisableLimitSpeed()
{
	RemoveFlags(LimitSpeed);
}

void ObjectBase::StepUp()
{
	calculateSpeed(GetMoveSpeed().y);
	SetMove({GetMove().x,GetMove().y - Type::Move::Type(currentSpeed_ / CPS)});
	if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().y > GetMove().y)
	{
		if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().y)
			currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().y;
		SetMove({GetMove().x,scene->GetObjectOut(GetCoord())->GetMove().y});
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
		carrySpeed_ = std::abs(GetMove().y);
		SetMove({GetMove().x,0});
		scene->ObjectArrived(GetCoord());
		ObjectArrived();
	}
}
void ObjectBase::StepDown()
{
	calculateSpeed(GetMoveSpeed().y);
	SetMove({GetMove().x,GetMove().y + Type::Move::Type(currentSpeed_ / CPS)});
	if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().y < GetMove().y)
	{
		if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().y)
			currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().y;
		SetMove({GetMove().x,scene->GetObjectOut(GetCoord())->GetMove().y});
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
		carrySpeed_ = std::abs(GetMove().y);
		SetMove({GetMove().x,0});
		scene->ObjectArrived(GetCoord());
		ObjectArrived();
	}
}
void ObjectBase::StepLeft()
{
	calculateSpeed(GetMoveSpeed().x);
	SetMove({GetMove().x - Type::Move::Type(currentSpeed_ / CPS),GetMove().y});
	if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().x > GetMove().x)
	{
		if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().x)
			currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().x;
		SetMove({scene->GetObjectOut(GetCoord())->GetMove().x,GetMove().y});
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
		carrySpeed_ = std::abs(GetMove().x);
		SetMove({0,GetMove().y});
		scene->ObjectArrived(GetCoord());
		ObjectArrived();
	}
}
void ObjectBase::StepRight()
{
	calculateSpeed(GetMoveSpeed().x);
	SetMove({GetMove().x + Type::Move::Type(currentSpeed_ / CPS),GetMove().y});
	if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().x < GetMove().x)
	{
		if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().x)
			currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().x;
		SetMove({scene->GetObjectOut(GetCoord())->GetMove().x,GetMove().y});
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
		carrySpeed_ = std::abs(GetMove().x);
		SetMove({0,GetMove().y});
		scene->ObjectArrived(GetCoord());
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

void ObjectBase::carryStepUp()
{
	if (carrySpeed_ > 0.f)
	{
		SetMove({GetMove().x,GetMove().y - carrySpeed_});
		if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().y > GetMove().y)
		{
			SetMove({GetMove().x,scene->GetObjectOut(GetCoord())->GetMove().y});
			if (!hitactive)
			{
				AutoStepHit();
				hitactive = true;
			}
		}
		carrySpeed_ = 0.f;
	}
}
void ObjectBase::carryStepDown()
{
	if (carrySpeed_ > 0.f)
	{
		SetMove({GetMove().x,GetMove().y + carrySpeed_});
		if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().y < GetMove().y)
		{
			if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().y)
				currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().y;
			SetMove({GetMove().x,scene->GetObjectOut(GetCoord())->GetMove().y});
			if (!hitactive)
			{
				AutoStepHit();
				hitactive = true;
			}
		}
		carrySpeed_ = 0.f;
	}
}
void ObjectBase::carryStepLeft()
{
	if (carrySpeed_ > 0.f)
	{
		SetMove({GetMove().x - carrySpeed_,GetMove().y});
		if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().x > GetMove().x)
		{
			if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().x)
				currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().x;
			SetMove({scene->GetObjectOut(GetCoord())->GetMove().x,GetMove().y});
			if (!hitactive)
			{
				AutoStepHit();
				hitactive = true;
			}
		}
		carrySpeed_ = 0.f;
	}
}
void ObjectBase::carryStepRight()
{
	if (carrySpeed_ > 0.f)
	{
		SetMove({GetMove().x + carrySpeed_,GetMove().y});
		if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().x < GetMove().x)
		{
			if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().x)
				currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().x;
			SetMove({scene->GetObjectOut(GetCoord())->GetMove().x,GetMove().y});
			if (!hitactive)
			{
				AutoStepHit();
				hitactive = true;
			}
		}
		carrySpeed_ = 0.f;
	}
}
void ObjectBase::carryStep()
{
	if (GetMove().x > 0)
		carryStepLeft();
	else if (GetMove().x < 0)
		carryStepRight();
	else if (GetMove().y > 0)
		carryStepUp();
	else if (GetMove().y < 0)
		carryStepDown();
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

