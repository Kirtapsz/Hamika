#include "Object.h"

namespace Object
{
	void Brick::PrintFlags(Type::Flags flags_)
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
			{CanBeExploded,"CanBeExploded"},{LimitSpeed,"LimitSpeed"},{PhysicsSpeed,"PhysicsSpeed"},
			{MurphyCanSuck,"MurphyCanSuck"},{CanBeKilled,"CanBeKilled"},{GiveGravityDelay,"GiveGravityDelay"},{ButtonPush,"ButtonPush"},
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

	bool Brick::Roll(double PpM)
	{
		return rand() % 1000 <= PpM / CPS * 10.;
	}


	void Brick::SetTranslationID(Type::ID id)
	{
		TranslationTo = id;
	}
	void Brick::SetObjectIDremain(Type::ID id)
	{
		ObjectIDremain = id;
	}

	void Brick::SetCoord(Type::Coord coord)
	{
		this->coord = coord;
		setOddDrawCoord();
	}

	//GET
	//1 m�sik oldalon m�g teljesen  0 nyugalmi �llapot
	float Brick::GetAbsMove()
	{
		float
			maxX = std::abs(move.x()),
			maxY = std::abs(move.y());
		if (maxX > maxY)
			return maxX;
		else
			return maxY;
	}


	Type::Coord Brick::GetForwardCoord()
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
	Type::Coord Brick::GetForwardCoord(Type::Rotation rotation)
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
	Type::ID Brick::GetTranslationTo()
	{
		return TranslationTo;
	}
	Type::ID Brick::GetObjectIDremain()
	{
		return ObjectIDremain;
	}
	void Brick::Initialize()
	{
	}

	bool Brick::CanMovePosByRotationH(Type::Coord to, Type::Rotation rotation) const
	{
		const Brick &out_brick = scene->GetObjectOut(to);
		return
			((rotation == Type::Rotations::Up && out_brick.GetCoord().y() < to.y())
			 ||
			 (rotation == Type::Rotations::Down && out_brick.GetCoord().y() > to.y())
			 ||
			 (rotation == Type::Rotations::Left && out_brick.GetCoord().x() < to.x())
			 ||
			 (rotation == Type::Rotations::Right && out_brick.GetCoord().x() > to.x())
			 );
	}

	bool Brick::CanMovePos(Type::Coord to, Type::Rotation rotation)
	{
		return
			scene->GetObject(to).GetFlags() & StepOn
			&&
			scene->GetRemain(to).GetFlags() & StepOn
			&&
			(
				scene->GetObjectOut(to).GetFlags() & StepOn
				||
				CanMovePosByRotationH(to, rotation)
				);
	}
	bool Brick::CanMoveDown()
	{
		return
			scene->GetObject(GetCoordDown()).GetFlags() & StepOn
			&&
			scene->GetRemain(GetCoordDown()).GetFlags() & StepOn
			&&
			(
				scene->GetObjectOut(GetCoordDown()).GetFlags() & StepOn
				||
				scene->GetObjectOut(GetCoordDown()).IsMoveDown()
				);
	}
	bool Brick::CanMoveUp()
	{
		return
			scene->GetObject(GetCoordUp()).GetFlags() & StepOn
			&&
			scene->GetRemain(GetCoordUp()).GetFlags() & StepOn
			&&
			(
				scene->GetObjectOut(GetCoordUp()).GetFlags() & StepOn
				||
				scene->GetObjectOut(GetCoordUp()).IsMoveUp()
				);
	}
	bool Brick::CanMoveLeft()
	{
		return
			scene->GetObject(GetCoordLeft()).GetFlags() & StepOn
			&&
			scene->GetRemain(GetCoordLeft()).GetFlags() & StepOn
			&&
			(
				scene->GetObjectOut(GetCoordLeft()).GetFlags() & StepOn
				||
				scene->GetObjectOut(GetCoordLeft()).IsMoveLeft()
				);
	}
	bool Brick::CanMoveRight()
	{
		return
			scene->GetObject(GetCoordRight()).GetFlags() & StepOn
			&&
			scene->GetRemain(GetCoordRight()).GetFlags() & StepOn
			&&
			(
				scene->GetObjectOut(GetCoordRight()).GetFlags() & StepOn
				||
				scene->GetObjectOut(GetCoordRight()).IsMoveRight()
				);
	}
}