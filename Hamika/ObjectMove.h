#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

#include "ObjectBase.h"
#include "ObjectActions.h"
#include "ObjectFlags.h"
#include "ObjectCoord.h"

namespace Object
{
	namespace Module
	{
		namespace Move
		{
			struct Data
			{
				Type::Move move = {0,0};
				Type::Move MoveSpeed = {1,1};
				float accelaratePercent_ = 0;
				Type::Speed limitSpeed_ = 0.f; // this speed cannot be stepped over, it is a fix speed
				Type::Speed currentSpeed_ = 0.f; // this amout will be stepped down
				Type::Speed carryMove_ = 0.f; // when a step does not cost currentSpeed_, it will be added to the next preiod
			};

			template<typename MODULES_T>
			struct Func
			{
				private: typename MODULES_T::DATA_T &data_;
				private: typename MODULES_T::FUNC_T &func_;
				public: Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func);

				public: void __init__(Type::ID id, Type::Coord coord);
				public: Json print();

					  // calling every time when starting/stopping a block move
				public: void doMove(typename MODULES_T::ACTIONS_DATA_T::ACTION_T _action, Type::ID _remain);
				public: void doMoveEx(typename MODULES_T::ACTIONS_DATA_T::ACTION_T _action, Type::ID _remain, Type::Coord _to, Type::Move _move);
				public: void finishMove();

					  // physics is responsible to dynamic speed up
				public: void enablePhysics();
				public: void disablePhysics();

					  // regardles of the original speed or physics setting, this values will be used
				public: void enableFixSpeed(Type::Speed _fixSpeed);
				public: void disableFixSpeed();
				public: void calculateSpeed(Type::Speed _baseSpeed);

				public: void StepUp();
				public: void StepDown();
				public: void StepLeft();
				public: void StepRight();
				public: void Step();

				public: void carryStepUp();
				public: void carryStepDown();
				public: void carryStepLeft();
				public: void carryStepRight();
				public: void carryStep();

				public: Type::Move GetMoveSpeed();
				public: Type::Move::base GetMoveSpeedVertical();
				public: Type::Move::base GetMoveSpeedHorizontal();

					  //a mértékegység hogy 1 másodperc alatt mennyit haladjon, 1 jelent egy teljes négyzetet, 2.5: két és fél négyzet másodpercenként....
				public: void SetMoveSpeed(Type::Move::base speed);
					  //a mértékegység hogy 1 másodperc alatt mennyit haladjon, 1 jelent egy teljes négyzetet, 2.5: két és fél négyzet másodpercenként....
				public: void SetMoveSpeed(Type::Move speed);

				public: void SetMoveUnsafe(Type::Move move_ = {0,0});
				public: void SetMove(Type::Move move_ = {0,0});
				public: Type::Move GetMove();
				public: bool IsMove();
				public: bool IsMoveHorizontal();
				public: bool IsMoveVertical();
				public: bool IsMoveLeft();
				public: bool IsMoveRight();
				public: bool IsMoveDown();
				public: bool IsMoveUp();
				public: Type::Direction getMoveDirection() const;
					  /*
					  beállítja autómatikusan a movet a rotation alapján

					  a move paraméter adja meg a move x vagy y-t, pozitív egész számnak kell lennie
					  */
				public: void SetMove(Type::Rotation rotation, Type::Move move = {1,1});
				public: void SetMoveUnsafe(Type::Rotation rotation, Type::Move move = {1,1});
			};
		}
	}
}

#include "ObjectMove.hpp"