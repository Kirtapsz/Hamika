#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

#include "ObjectActions.h"
#include "ObjectFlags.h"
#include "ObjectCoord.h"

namespace Object
{
	namespace Module
	{
		namespace Move
		{
			template <typename OBJECT>
			struct Data
			{
				Type::Move move = {0,0};
				Type::Move MoveSpeed = {1,1};
				float accelaratePercent_ = 0;
				Type::Speed limitSpeed_ = 0.f; // this speed cannot be stepped over, it is a fix speed
				Type::Speed currentSpeed_ = 0.f; // this amout will be stepped down
				Type::Speed carryMove_ = 0.f; // when a step does not cost currentSpeed_, it will be added to the next preiod

				virtual void setOddDrawCoord() = 0;
			};
			template <typename DATA>
			struct Func: virtual DATA,
				virtual Flags::Func<DATA>,
				virtual Coord::Func<DATA>
			{
				void __init__(Type::ID id, Type::Coord coord);
				Json print();

				// calling every time when starting/stopping a block move
				void doMove(typename DATA::ACTION_T _action, Type::ID _remain);
				void doMoveEx(typename DATA::ACTION_T _action, Type::ID _remain, Type::Coord _to, Type::Move _move);
				void finishMove();

				// physics is responsible to dynamic speed up
				void enablePhysics();
				void disablePhysics();

				// regardles of the original speed or physics setting, this values will be used
				void enableFixSpeed(Type::Speed _fixSpeed);
				void disableFixSpeed();
				void calculateSpeed(Type::Speed _baseSpeed);

				void StepUp();
				void StepDown();
				void StepLeft();
				void StepRight();
				void Step();

				void carryStepUp();
				void carryStepDown();
				void carryStepLeft();
				void carryStepRight();
				void carryStep();

				Type::Move GetMoveSpeed();
				Type::Move::base GetMoveSpeedVertical();
				Type::Move::base GetMoveSpeedHorizontal();

				//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 1 jelent egy teljes n�gyzetet, 2.5: k�t �s f�l n�gyzet m�sodpercenk�nt....
				void SetMoveSpeed(Type::Move::base speed);
				//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 1 jelent egy teljes n�gyzetet, 2.5: k�t �s f�l n�gyzet m�sodpercenk�nt....
				void SetMoveSpeed(Type::Move speed);

				void SetMoveUnsafe(Type::Move move_ = {0,0});
				void SetMove(Type::Move move_ = {0,0});
				Type::Move GetMove();
				bool IsMove();
				bool IsMoveHorizontal();
				bool IsMoveVertical();
				bool IsMoveLeft();
				bool IsMoveRight();
				bool IsMoveDown();
				bool IsMoveUp();
				Type::Direction getMoveDirection() const;
				/*
				be�ll�tja aut�matikusan a movet a rotation alapj�n

				a move param�ter adja meg a move x vagy y-t, pozit�v eg�sz sz�mnak kell lennie
				*/
				void SetMove(Type::Rotation rotation, Type::Move move = {1,1});
				void SetMoveUnsafe(Type::Rotation rotation, Type::Move move = {1,1});
			};
		}
	}
}

#include "ObjectMove.hpp"