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
		{StepOn,"StepOn"},{PlayerDies,"PlayerDies"},{CanPushUp,"CanPushUp"},{CanPushDown,"CanPushDown"},{CanPushRight,"CanPushRight"},{CanPushLeft,"CanPushLeft"},{CanPush,"CanPush"},{PlayerStepOn,"PlayerStepOn"},{RollOffTop,"RollOffTop"},{RollOffBottom,"RollOffBottom"},{RollOff,"RollOff"},{PassageFromRight,"PassageFromRight"},{PassageFromLeft,"PassageFromLeft"},{PassageFromTop,"PassageFromTop"},{PassageFromBottom,"PassageFromBottom"},{PassageVertical,"PassageVertical"},{PassageHorizontal,"PassageHorizontal"},{Passage,"Passage"},{FallOnExplosion,"FallOnExplosion"},{ExplosionType1,"ExplosionType1"},{ExplosionType3,"ExplosionType3"},{ExplosionType5,"ExplosionType5"},{CanBeExplosion,"CanBeExplosion"},{LimitSpeed,"LimitSpeed"},{PhysicsSpeed,"PhysicsSpeed"},{InstantSpeed,"InstantSpeed"},{PlayerCanSniff,"PlayerCanSniff"},{CanBeKilled,"CanBeKilled"},{GiveGravityDelay,"GiveGravityDelay"},{ButtonPush,"ButtonPush"},{Give1Aim,"Give1Aim"},{Give1Unity,"Give1Unity"},
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
	requests_.update = false;
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
		rotation = this->rotation + 0.01/*hibat�r�s*/;

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
	rotation += 0.01/*hibat�r�s*/;

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
	rotation += 0.01/*hibat�r�s*/;

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
	if (events_.timer) clog << "timer, ";
	if (events_.tick) clog << "tick, ";
	if (events_.update) clog << "update, ";
	if (events_.topDraw) clog << "topDraw, ";
	clog << "\n";
	clog << "		Actions: ";
	if (actions.move) clog << "move, ";
	if (actions.rotate) clog << "rotate, ";
	clog << "\n";
	clog << "		Requests: ";
	if (requests_.remove) clog << "timer, ";
	if (requests_.remove) clog << "tick, ";
	if (requests_.remove) clog << "update, ";
	if (requests_.remove) clog << "draw, ";
	if (requests_.remove) clog << "remove, ";
	if (requests_.remove) clog << "blowUp, ";
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
	KIR5::Bitmap lineDisappear("Hamika\\Texture\\Block\\Original\\LineDisappear.png");
	KIR5::Bitmap squareDisappear("Hamika\\Texture\\Block\\Original\\SquareDisappear.png");
	KIR5::Bitmap squareDisappearV4("Hamika\\Texture\\Block\\Original\\RectangleDisappear_v4.png");
	KIR5::Bitmap tmp;

	// Unknown
	bitmapPool.addBitmap(unknownBitmap(KIR5::Color(255, 0, 255)), "Unknown");

	// Player
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Player\\Base.png"), "Pleyer-Base");
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Player\\MovingVertical.png"), "Pleyer-MovingVertical");
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Player\\MovingHorizontal.png"), "Pleyer-MovingHorizontal");
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Player\\PassageVertical.png"), "Pleyer-PassageVertical");
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Player\\PassageHorizontal.png"), "Pleyer-PassageHorizontal");
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Player\\Sniff.png"), "Pleyer-Sniff");
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Player\\Push.png"), "Pleyer-Push");

	// 001
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\001-ZonkMoveHorizontal.png"), "001-ZonkMoveHorizontal");

	// 002
	tmp.load("Hamika\\Texture\\Block\\Original\\002-BaseX.png");
	bitmapPool.addBitmap(tmp, "002-BaseX");
	bitmapPool.addBitmap(CreateBitmapTransition(tmp, squareDisappearV4), "002-BaseX.square-disappear");

	// 003
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\003-HardwareBlueLed.png"), "003-HardwareBlueLed");

	// 004
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\004-HardwareBrick1.png"), "004-HardwareBrick1");

	// 005
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\005-HardwareBlueResistance.png"), "005-HardwareBlueResistance");

	// 006
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\006-HardwareGreenLed.png"), "006-HardwareGreenLed");

	// 007
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\007-HardwareRedLed.png"), "007-HardwareRedLed");

	// 008
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\008-HardwareResource.png"), "008-HardwareResource");

	// 009
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\009-HardwareResistance.png"), "009-HardwareResistance");

	// 010
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\010-HardwareStripLine.png"), "010-HardwareStripLine");

	// 011
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\011-HardwareYellowResistance.png"), "011-HardwareYellowResistance");

	// 012
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\012-HardwareRedCapacitor.png"), "012-HardwareRedCapacitor");

	// 013
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\013-Bug.png"), "013-Bug");

	// 014
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\014-InfotronDisappear.png"), "014-InfotronDisappear");
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\014-InfotronMoveHorizontal.png"), "014-InfotronMoveHorizontal");

	// 015
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\015-Exit.png"), "015-Exit");

	// 016
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\016-CrossPort.png"), "016-CrossPort");

	// 017
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\017-DualPortHorizontal.png"), "017-DualPortHorizontal");

	// 018
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\018-DualPortVertical.png"), "018-DualPortVertical");

	// 019
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\019-OneWayPortDown.png"), "019-OneWayPortDown");

	// 020
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\020-OneWayPortLeft.png"), "020-OneWayPortLeft");

	// 021
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\021-OneWayPortRight.png"), "021-OneWayPortRight");

	// 022
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\022-OneWayPortUp.png"), "022-OneWayPortUp");

	// 023
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\023-RAMChipsMini.png"), "023-RAMChipsMini");

	// 024
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\024-RAMChipsLeft.png"), "024-RAMChipsLeft");

	// 025
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\025-RAMChipsRight.png"), "025-RAMChipsRight");

	// 026
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\026-Electrons.png"), "026-Electrons");

	// 027
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\027-SnikSnakMoveVertical.png"), "027-SnikSnakMoveVertical");
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\027-SnikSnakMoveHorizontal.png"), "027-SnikSnakMoveHorizontal");
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\027-SnikSnakRotate.png"), "027-SnikSnakRotate");

	// 028
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\028-Terminal.png"), "028-Terminal");

	// 029
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\029-Utility1.png"), "029-Utility1");

	// 030
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\030-Utility2.png"), "030-Utility2");
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\030-Utility2Activated.png"), "030-Utility2Activated");

	// 031
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\031-Utility3.png"), "031-Utility3");

	// 032
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\032-Explosion.png"), "032-Explosion");

	// 033

	// 034

	// 035
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\035-RAMChipsTop.png"), "035-RAMChipsTop");

	// 036
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\036-RAMChipsBottom.png"), "036-RAMChipsBottom");

	// 037
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\037-HardwareSwitch.png"), "037-HardwareSwitch");

	// 038
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\038-OneWayPortDownGrav.png"), "038-OneWayPortDownGrav");

	// 039
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\039-OneWayPortLeftGrav.png"), "039-OneWayPortLeftGrav");

	// 040
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\040-OneWayPortRightGrav.png"), "040-OneWayPortRightGrav");

	// 041
	bitmapPool.addBitmap(KIR5::Bitmap("Hamika\\Texture\\Block\\Original\\041-OneWayPortUpGrav.png"), "041-OneWayPortUpGrav");

	bitmapPool.finalise(unknownBitmap(KIR5::Color(255, 0, 0)));

	unknownBmp = bitmapPool.get("Unknown");
}

void ObjectBase::IncreaseSpeed(Type::Move::Type max)
{
	if (GetFlags() & LimitSpeed)
		max = limitspeed;

	if (GetFlags() & PhysicsSpeed)
	{
		currentspeed += 0.1;
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


BitmapPool ObjectBase::bitmapPool;
KIR5::SubBitmap ObjectBase::unknownBmp;
unsigned long long int ObjectBase::SUpdateNumber = 0;

