#include "Object.h"

namespace Object
{
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


	Type::Coord Brick::GetForwardCoord()
	{
		Type::Rotation
			rotation = this->rotation + 0.01f; // treashold

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
		rotation += 0.01f; // treashold

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
			scene->GetObject(to).GetFlags() & Flags::StepOn
			&&
			scene->GetRemain(to).GetFlags() & Flags::StepOn
			&&
			(
				scene->GetObjectOut(to).GetFlags() & Flags::StepOn
				||
				CanMovePosByRotationH(to, rotation)
				);
	}
	bool Brick::CanMoveDown()
	{
		return
			scene->GetObject(GetCoordDown()).GetFlags() & Flags::StepOn
			&&
			scene->GetRemain(GetCoordDown()).GetFlags() & Flags::StepOn
			&&
			(
				scene->GetObjectOut(GetCoordDown()).GetFlags() & Flags::StepOn
				||
				scene->GetObjectOut(GetCoordDown()).IsMoveDown()
				);
	}
	bool Brick::CanMoveUp()
	{
		return
			scene->GetObject(GetCoordUp()).GetFlags() & Flags::StepOn
			&&
			scene->GetRemain(GetCoordUp()).GetFlags() & Flags::StepOn
			&&
			(
				scene->GetObjectOut(GetCoordUp()).GetFlags() & Flags::StepOn
				||
				scene->GetObjectOut(GetCoordUp()).IsMoveUp()
				);
	}
	bool Brick::CanMoveLeft()
	{
		return
			scene->GetObject(GetCoordLeft()).GetFlags() & Flags::StepOn
			&&
			scene->GetRemain(GetCoordLeft()).GetFlags() & Flags::StepOn
			&&
			(
				scene->GetObjectOut(GetCoordLeft()).GetFlags() & Flags::StepOn
				||
				scene->GetObjectOut(GetCoordLeft()).IsMoveLeft()
				);
	}
	bool Brick::CanMoveRight()
	{
		return
			scene->GetObject(GetCoordRight()).GetFlags() & Flags::StepOn
			&&
			scene->GetRemain(GetCoordRight()).GetFlags() & Flags::StepOn
			&&
			(
				scene->GetObjectOut(GetCoordRight()).GetFlags() & Flags::StepOn
				||
				scene->GetObjectOut(GetCoordRight()).IsMoveRight()
				);
	}
}