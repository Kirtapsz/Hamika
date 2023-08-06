#ifndef OBJECT_MOVE_HPP
#define OBJECT_MOVE_HPP

#include "ObjectMove.h"

namespace Object
{
	namespace Module
	{
		namespace Move
		{
			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				move = {0,0};
				MoveSpeed = {1,1};

				accelaratePercent_ = 0.f;
				limitSpeed_ = 0.f;
				currentSpeed_ = 0.f;
				carryMove_ = 0.f;
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				json["move.x"] = move.x();
				json["move.y"] = move.y();

				json["MoveSpeed.x"] = MoveSpeed.x();
				json["MoveSpeed.y"] = MoveSpeed.y();

				json["accelaratePercent"] = accelaratePercent_;
				json["limitSpeed"] = limitSpeed_;
				json["currentSpeed"] = currentSpeed_;

				return json;
			}


			template <typename DATA>
			void Func<DATA>::doMove(typename DATA::ACTION_T _action, Type::ID _remain)
			{
				action = _action;
				switch (_action)
				{
					case MOVE_UP:
					{
						scene->ObjectMove(GetCoord(), GetCoordUp(), _remain);
						SetMove({0,1});
						carryStepUp();
						break;
					}
					case MOVE_RIGHT:
					{
						scene->ObjectMove(GetCoord(), GetCoordRight(), _remain);
						SetMove({-1,0});
						carryStepRight();
						break;
					}
					case MOVE_DOWN:
					{
						scene->ObjectMove(GetCoord(), GetCoordDown(), _remain);
						SetMove({0,-1});
						carryStepDown();
						break;
					}
					case MOVE_LEFT:
					{
						scene->ObjectMove(GetCoord(), GetCoordLeft(), _remain);
						SetMove({1,0});
						carryStepLeft();
						break;
					}
				}
			}
			template <typename DATA>
			void Func<DATA>::doMoveEx(typename DATA::ACTION_T _action, Type::ID _remain, Type::Coord _to, Type::Move _move)
			{
				action = _action;
				scene->ObjectMove(GetCoord(), _to, _remain);
				SetMove(_move);
				carryStep();
			}
			template <typename DATA>
			void Func<DATA>::finishMove()
			{
				carryMove_ = 0.f;
				accelaratePercent_ = 0.f;
				action = STEADY;
			}

			template <typename DATA>
			void Func<DATA>::enablePhysics()
			{
				AddFlags(PhysicsSpeed);
			}
			template <typename DATA>
			void Func<DATA>::disablePhysics()
			{
				RemoveFlags(PhysicsSpeed);
			}

			template <typename DATA>
			void Func<DATA>::enableFixSpeed(Type::Speed _fixSpeed)
			{
				AddFlags(LimitSpeed);
				limitSpeed_ = _fixSpeed;
			}
			template <typename DATA>
			void Func<DATA>::disableFixSpeed()
			{
				RemoveFlags(LimitSpeed);
			}
			template <typename DATA>
			void Func<DATA>::calculateSpeed(Type::Speed _baseSpeed)
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


			template <typename DATA>
			void Func<DATA>::StepUp()
			{
				calculateSpeed(GetMoveSpeed().y());
				SetMove({GetMove().x(),GetMove().y() - Type::Move::base(currentSpeed_ / CPS)});
				if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().y() > GetMove().y())
				{
					if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().y())
						currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().y();
					SetMove({GetMove().x(),scene->GetObjectOut(GetCoord())->GetMove().y()});
				}
				if (GetMove().y() <= 0)
				{
					carryMove_ = std::abs(GetMove().y());
					SetMove({0,0});
					scene->ObjectArrived(GetCoord());
				}
			}
			template <typename DATA>
			void Func<DATA>::StepDown()
			{
				calculateSpeed(GetMoveSpeed().y());
				SetMove({GetMove().x(),GetMove().y() + Type::Move::base(currentSpeed_ / CPS)});
				if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().y() < GetMove().y())
				{
					if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().y())
						currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().y();
					SetMove({GetMove().x(),scene->GetObjectOut(GetCoord())->GetMove().y()});
				}
				if (GetMove().y() >= 0)
				{
					carryMove_ = std::abs(GetMove().y());
					SetMove({0,0});
					scene->ObjectArrived(GetCoord());
				}
			}
			template <typename DATA>
			void Func<DATA>::StepLeft()
			{
				calculateSpeed(GetMoveSpeed().x());
				SetMove({GetMove().x() - Type::Move::base(currentSpeed_ / CPS),GetMove().y()});
				if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().x() > GetMove().x())
				{
					if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().x())
						currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().x();
					SetMove({scene->GetObjectOut(GetCoord())->GetMove().x(),GetMove().y()});
				}
				if (GetMove().x() <= 0)
				{
					carryMove_ = std::abs(GetMove().x());
					SetMove({0,0});
					scene->ObjectArrived(GetCoord());
				}
			}
			template <typename DATA>
			void Func<DATA>::StepRight()
			{
				calculateSpeed(GetMoveSpeed().x());
				SetMove({GetMove().x() + Type::Move::base(currentSpeed_ / CPS),GetMove().y()});
				if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().x() < GetMove().x())
				{
					if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().x())
						currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().x();
					SetMove({scene->GetObjectOut(GetCoord())->GetMove().x(),GetMove().y()});
				}
				if (GetMove().x() >= 0)
				{
					carryMove_ = std::abs(GetMove().x());
					SetMove({0,0});
					scene->ObjectArrived(GetCoord());
				}
			}
			template <typename DATA>
			void Func<DATA>::Step()
			{
				if (GetMove().x() > 0)
					StepLeft();
				else if (GetMove().x() < 0)
					StepRight();
				else if (GetMove().y() > 0)
					StepUp();
				else if (GetMove().y() < 0)
					StepDown();
			}

			template <typename DATA>
			void Func<DATA>::carryStepUp()
			{
				if (carryMove_ > 0.f)
				{
					SetMove({GetMove().x(),GetMove().y() - carryMove_});
					if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().y() > GetMove().y())
					{
						SetMove({GetMove().x(),scene->GetObjectOut(GetCoord())->GetMove().y()});
					}
					carryMove_ = 0.f;
				}
			}
			template <typename DATA>
			void Func<DATA>::carryStepDown()
			{
				if (carryMove_ > 0.f)
				{
					SetMove({GetMove().x(),GetMove().y() + carryMove_});
					if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().y() < GetMove().y())
					{
						if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().y())
							currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().y();
						SetMove({GetMove().x(),scene->GetObjectOut(GetCoord())->GetMove().y()});
					}
					carryMove_ = 0.f;
				}
			}
			template <typename DATA>
			void Func<DATA>::carryStepLeft()
			{
				if (carryMove_ > 0.f)
				{
					SetMove({GetMove().x() - carryMove_,GetMove().y()});
					if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().x() > GetMove().x())
					{
						if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().x())
							currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().x();
						SetMove({scene->GetObjectOut(GetCoord())->GetMove().x(),GetMove().y()});
					}
					carryMove_ = 0.f;
				}
			}
			template <typename DATA>
			void Func<DATA>::carryStepRight()
			{
				if (carryMove_ > 0.f)
				{
					SetMove({GetMove().x() + carryMove_,GetMove().y()});
					if (scene->IsObjectOut(GetCoord()) && scene->GetObjectOut(GetCoord())->GetMove().x() < GetMove().x())
					{
						if (currentSpeed_ > scene->GetObjectOut(GetCoord())->GetMoveSpeed().x())
							currentSpeed_ = scene->GetObjectOut(GetCoord())->GetMoveSpeed().x();
						SetMove({scene->GetObjectOut(GetCoord())->GetMove().x(),GetMove().y()});
					}
					carryMove_ = 0.f;
				}
			}
			template <typename DATA>
			void Func<DATA>::carryStep()
			{
				if (GetMove().x() > 0)
					carryStepLeft();
				else if (GetMove().x() < 0)
					carryStepRight();
				else if (GetMove().y() > 0)
					carryStepUp();
				else if (GetMove().y() < 0)
					carryStepDown();
			}


			template <typename DATA>
			Type::Move Func<DATA>::GetMoveSpeed()
			{
				return MoveSpeed;
			}
			template <typename DATA>
			Type::Move::base Func<DATA>::GetMoveSpeedVertical()
			{
				return MoveSpeed.y();
			}
			template <typename DATA>
			Type::Move::base Func<DATA>::GetMoveSpeedHorizontal()
			{
				return MoveSpeed.x();
			}

			//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 1 jelent egy teljes n�gyzetet, 2.5: k�t �s f�l n�gyzet m�sodpercenk�nt....
			template <typename DATA>
			void Func<DATA>::SetMoveSpeed(Type::Move::base speed)
			{
				MoveSpeed = {speed,speed};
			}
			//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 1 jelent egy teljes n�gyzetet, 2.5: k�t �s f�l n�gyzet m�sodpercenk�nt....
			template <typename DATA>
			void Func<DATA>::SetMoveSpeed(Type::Move speed)
			{
				MoveSpeed = speed;
			}


			template <typename DATA>
			void Func<DATA>::SetMoveUnsafe(Type::Move move_)
			{
				move = move_;
			}
			template <typename DATA>
			void Func<DATA>::SetMove(Type::Move move_)
			{
				move = move_;
				setOddDrawCoord();
			}
			template <typename DATA>
			Type::Move Func<DATA>::GetMove()
			{
				return move;
			}
			template <typename DATA>
			bool Func<DATA>::IsMove()
			{
				return move.x() != 0 || move.y() != 0;
			}
			template <typename DATA>
			bool Func<DATA>::IsMoveHorizontal()
			{
				return move.x() != 0;
			}
			template <typename DATA>
			bool Func<DATA>::IsMoveVertical()
			{
				return move.y() != 0;
			}
			template <typename DATA>
			bool Func<DATA>::IsMoveLeft()
			{
				return move.x() > 0;
			}
			template <typename DATA>
			bool Func<DATA>::IsMoveRight()
			{
				return move.x() < 0;
			}
			template <typename DATA>
			bool Func<DATA>::IsMoveDown()
			{
				return move.y() < 0;
			}
			template <typename DATA>
			bool Func<DATA>::IsMoveUp()
			{
				return move.y() > 0;
			}
			template <typename DATA>
			Type::Direction Func<DATA>::getMoveDirection() const
			{
				if (move.x() > 0)
					return Type::Directions::left;
				else if (move.x() < 0)
					return Type::Directions::right;
				else if (move.y() > 0)
					return Type::Directions::up;
				else if (move.y() < 0)
					return Type::Directions::down;
				else
					return Type::Directions::up;
			}


			template <typename DATA>
			void Func<DATA>::SetMove(Type::Rotation rotation, Type::Move move)
			{
				if (rotation == Type::Rotations::Up)
					SetMove({this->move.x(),move.y()});
				else if (rotation == Type::Rotations::Down)
					SetMove({this->move.x(),-move.y()});
				else if (rotation == Type::Rotations::Left)
					SetMove({move.x(),this->move.y()});
				else if (rotation == Type::Rotations::Right)
					SetMove({-move.x(),this->move.y()});
			}
			template <typename DATA>
			void Func<DATA>::SetMoveUnsafe(Type::Rotation rotation, Type::Move move)
			{
				if (rotation == Type::Rotations::Up)
					SetMoveUnsafe({this->move.x(),move.y()});
				else if (rotation == Type::Rotations::Down)
					SetMoveUnsafe({this->move.x(),-move.y()});
				else if (rotation == Type::Rotations::Left)
					SetMoveUnsafe({move.x(),this->move.y()});
				else if (rotation == Type::Rotations::Right)
					SetMoveUnsafe({-move.x(),this->move.y()});
			}
		}
	}
}

#endif