#ifndef OBJECT_MOVE_HPP
#define OBJECT_MOVE_HPP

#include "ObjectMove.h"

namespace Object
{
	namespace Module
	{
		namespace Move
		{
			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				data_.move = {0,0};
				data_.MoveSpeed = {1,1};

				data_.accelaratePercent_ = 0.f;
				data_.limitSpeed_ = 0.f;
				data_.currentSpeed_ = 0.f;
				data_.carryMove_ = 0.f;
			}

			template<typename MODULES_T>
			Json Func<MODULES_T>::print()
			{
				Json json;

				json["move.x"] = data_.move.x();
				json["move.y"] = data_.move.y();

				json["MoveSpeed.x"] = data_.MoveSpeed.x();
				json["MoveSpeed.y"] = data_.MoveSpeed.y();

				json["accelaratePercent"] = data_.accelaratePercent_;
				json["limitSpeed"] = data_.limitSpeed_;
				json["currentSpeed"] = data_.currentSpeed_;

				return json;
			}

			template<typename MODULES_T>
			void Func<MODULES_T>::doMove(typename MODULES_T::ACTIONS_DATA_T::ACTION_T _action, Type::ID _remain)
			{
				data_.action = _action;
				switch (_action)
				{
					case MODULES_T::DATA_T::MOVE_UP:
					{
						data_.scene->ObjectMove(func_.GetCoord(), func_.GetCoordUp(), _remain);
						SetMove({0,1});
						carryStepUp();
						break;
					}
					case MODULES_T::DATA_T::MOVE_RIGHT:
					{
						data_.scene->ObjectMove(func_.GetCoord(), func_.GetCoordRight(), _remain);
						SetMove({-1,0});
						carryStepRight();
						break;
					}
					case MODULES_T::DATA_T::MOVE_DOWN:
					{
						data_.scene->ObjectMove(func_.GetCoord(), func_.GetCoordDown(), _remain);
						SetMove({0,-1});
						carryStepDown();
						break;
					}
					case MODULES_T::DATA_T::MOVE_LEFT:
					{
						data_.scene->ObjectMove(func_.GetCoord(), func_.GetCoordLeft(), _remain);
						SetMove({1,0});
						carryStepLeft();
						break;
					}
				}
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::doMoveEx(typename MODULES_T::ACTIONS_DATA_T::ACTION_T _action, Type::ID _remain, Type::Coord _to, Type::Move _move)
			{
				data_.action = _action;
				data_.scene->ObjectMove(func_.GetCoord(), _to, _remain);
				SetMove(_move);
				carryStep();
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::finishMove()
			{
				data_.carryMove_ = 0.f;
				data_.accelaratePercent_ = 0.f;
				data_.action = MODULES_T::DATA_T::STEADY;
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::enablePhysics()
			{
				func_.AddFlags(MODULES_T::BRICK_T::Flags::PhysicsSpeed);
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::disablePhysics()
			{
				func_.RemoveFlags(MODULES_T::BRICK_T::Flags::PhysicsSpeed);
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::enableFixSpeed(Type::Speed _fixSpeed)
			{
				func_.AddFlags(MODULES_T::BRICK_T::Flags::LimitSpeed);
				data_.limitSpeed_ = _fixSpeed;
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::disableFixSpeed()
			{
				func_.RemoveFlags(MODULES_T::BRICK_T::Flags::LimitSpeed);
			}

			template<typename MODULES_T>
			void Func<MODULES_T>::calculateSpeed(Type::Speed _baseSpeed)
			{
				if (func_.GetFlags() & MODULES_T::BRICK_T::Flags::LimitSpeed)
				{
					data_.currentSpeed_ = data_.limitSpeed_;
				}
				else if (func_.GetFlags() & MODULES_T::BRICK_T::Flags::PhysicsSpeed)
				{
					data_.accelaratePercent_ = std::min(1.f, data_.accelaratePercent_ + 0.15f);
					data_.currentSpeed_ = _baseSpeed * data_.accelaratePercent_;
				}
				else
				{
					data_.currentSpeed_ = _baseSpeed;
				}
			}

			template<typename MODULES_T>
			void Func<MODULES_T>::StepUp()
			{
				calculateSpeed(GetMoveSpeed().y());
				SetMove({GetMove().x(),GetMove().y() - Type::Move::base(data_.currentSpeed_ / CPS)});
				if (data_.scene->IsObjectOut(func_.GetCoord()) && data_.scene->GetObjectOut(func_.GetCoord()).GetMove().y() > GetMove().y())
				{
					if (data_.currentSpeed_ > data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().y())
						data_.currentSpeed_ = data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().y();
					SetMove({GetMove().x(),data_.scene->GetObjectOut(func_.GetCoord()).GetMove().y()});
				}
				if (GetMove().y() <= 0)
				{
					data_.carryMove_ = std::abs(GetMove().y());
					SetMove({0,0});
					data_.scene->ObjectArrived(func_.GetCoord());
				}
			}

			template<typename MODULES_T>
			void Func<MODULES_T>::StepDown()
			{
				calculateSpeed(GetMoveSpeed().y());
				SetMove({GetMove().x(),GetMove().y() + Type::Move::base(data_.currentSpeed_ / CPS)});
				if (data_.scene->IsObjectOut(func_.GetCoord()) && data_.scene->GetObjectOut(func_.GetCoord()).GetMove().y() < GetMove().y())
				{
					if (data_.currentSpeed_ > data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().y())
						data_.currentSpeed_ = data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().y();
					SetMove({GetMove().x(),data_.scene->GetObjectOut(func_.GetCoord()).GetMove().y()});
				}
				if (GetMove().y() >= 0)
				{
					data_.carryMove_ = std::abs(GetMove().y());
					SetMove({0,0});
					data_.scene->ObjectArrived(func_.GetCoord());
				}
			}

			template<typename MODULES_T>
			void Func<MODULES_T>::StepLeft()
			{
				calculateSpeed(GetMoveSpeed().x());
				SetMove({GetMove().x() - Type::Move::base(data_.currentSpeed_ / CPS),GetMove().y()});
				if (data_.scene->IsObjectOut(func_.GetCoord()) && data_.scene->GetObjectOut(func_.GetCoord()).GetMove().x() > GetMove().x())
				{
					if (data_.currentSpeed_ > data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().x())
						data_.currentSpeed_ = data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().x();
					SetMove({data_.scene->GetObjectOut(func_.GetCoord()).GetMove().x(),GetMove().y()});
				}
				if (GetMove().x() <= 0)
				{
					data_.carryMove_ = std::abs(GetMove().x());
					SetMove({0,0});
					data_.scene->ObjectArrived(func_.GetCoord());
				}
			}

			template<typename MODULES_T>
			void Func<MODULES_T>::StepRight()
			{
				calculateSpeed(GetMoveSpeed().x());
				SetMove({GetMove().x() + Type::Move::base(data_.currentSpeed_ / CPS),GetMove().y()});
				if (data_.scene->IsObjectOut(func_.GetCoord()) && data_.scene->GetObjectOut(func_.GetCoord()).GetMove().x() < GetMove().x())
				{
					if (data_.currentSpeed_ > data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().x())
						data_.currentSpeed_ = data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().x();
					SetMove({data_.scene->GetObjectOut(func_.GetCoord()).GetMove().x(),GetMove().y()});
				}
				if (GetMove().x() >= 0)
				{
					data_.carryMove_ = std::abs(GetMove().x());
					SetMove({0,0});
					data_.scene->ObjectArrived(func_.GetCoord());
				}
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::Step()
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

			template<typename MODULES_T>
			inline void Func<MODULES_T>::carryStepUp()
			{
				if (data_.carryMove_ > 0.f)
				{
					SetMove({GetMove().x(),GetMove().y() - data_.carryMove_});
					if (data_.scene->IsObjectOut(func_.GetCoord()) && data_.scene->GetObjectOut(func_.GetCoord()).GetMove().y() > GetMove().y())
					{
						SetMove({GetMove().x(),data_.scene->GetObjectOut(func_.GetCoord()).GetMove().y()});
					}
					data_.carryMove_ = 0.f;
				}
			}

			template<typename MODULES_T>
			void Func<MODULES_T>::carryStepDown()
			{
				if (data_.carryMove_ > 0.f)
				{
					SetMove({GetMove().x(),GetMove().y() + data_.carryMove_});
					if (data_.scene->IsObjectOut(func_.GetCoord()) && data_.scene->GetObjectOut(func_.GetCoord()).GetMove().y() < GetMove().y())
					{
						if (data_.currentSpeed_ > data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().y())
							data_.currentSpeed_ = data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().y();
						SetMove({GetMove().x(),data_.scene->GetObjectOut(func_.GetCoord()).GetMove().y()});
					}
					data_.carryMove_ = 0.f;
				}
			}

			template<typename MODULES_T>
			void Func<MODULES_T>::carryStepLeft()
			{
				if (data_.carryMove_ > 0.f)
				{
					SetMove({GetMove().x() - data_.carryMove_,GetMove().y()});
					if (data_.scene->IsObjectOut(func_.GetCoord()) && data_.scene->GetObjectOut(func_.GetCoord()).GetMove().x() > GetMove().x())
					{
						if (data_.currentSpeed_ > data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().x())
							data_.currentSpeed_ = data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().x();
						SetMove({data_.scene->GetObjectOut(func_.GetCoord()).GetMove().x(),GetMove().y()});
					}
					data_.carryMove_ = 0.f;
				}
			}

			template<typename MODULES_T>
			void Func<MODULES_T>::carryStepRight()
			{
				if (data_.carryMove_ > 0.f)
				{
					SetMove({GetMove().x() + data_.carryMove_,GetMove().y()});
					if (data_.scene->IsObjectOut(func_.GetCoord()) && data_.scene->GetObjectOut(func_.GetCoord()).GetMove().x() < GetMove().x())
					{
						if (data_.currentSpeed_ > data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().x())
							data_.currentSpeed_ = data_.scene->GetObjectOut(func_.GetCoord()).GetMoveSpeed().x();
						SetMove({data_.scene->GetObjectOut(func_.GetCoord()).GetMove().x(),GetMove().y()});
					}
					data_.carryMove_ = 0.f;
				}
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::carryStep()
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

			template<typename MODULES_T>
			inline Type::Move Func<MODULES_T>::GetMoveSpeed() const
			{
				return data_.MoveSpeed;
			}

			template<typename MODULES_T>
			inline Type::Move::base Func<MODULES_T>::GetMoveSpeedVertical()const
			{
				return data_.MoveSpeed.y();
			}

			template<typename MODULES_T>
			inline Type::Move::base Func<MODULES_T>::GetMoveSpeedHorizontal()const
			{
				return data_.MoveSpeed.x();
			}

			//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 1 jelent egy teljes n�gyzetet, 2.5: k�t �s f�l n�gyzet m�sodpercenk�nt....

			template<typename MODULES_T>
			inline void Func<MODULES_T>::SetMoveSpeed(Type::Move::base speed)
			{
				data_.MoveSpeed = {speed,speed};
			}
			//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 1 jelent egy teljes n�gyzetet, 2.5: k�t �s f�l n�gyzet m�sodpercenk�nt....

			template<typename MODULES_T>
			inline void Func<MODULES_T>::SetMoveSpeed(Type::Move speed)
			{
				data_.MoveSpeed = speed;
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::SetMoveUnsafe(Type::Move move_)
			{
				data_.move = move_;
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::SetMove(Type::Move move_)
			{
				data_.move = move_;
				func_.setOddDrawCoord();
			}

			template<typename MODULES_T>
			inline Type::Move &Func<MODULES_T>::GetMove() const
			{
				return data_.move;
			}

			// 1 means it is on the other side completely, 0 means it is in state of rest
			template<typename MODULES_T>
			Type::Move::base Func<MODULES_T>::GetAbsMove() const
			{
				return (std::max)(std::abs(data_.move.x()), std::abs(data_.move.y()));
			}

			template<typename MODULES_T>
			inline bool Func<MODULES_T>::IsMove() const
			{
				return data_.move.x() != 0 || data_.move.y() != 0;
			}

			template<typename MODULES_T>
			inline bool Func<MODULES_T>::IsMoveHorizontal() const
			{
				return data_.move.x() != 0;
			}

			template<typename MODULES_T>
			inline bool Func<MODULES_T>::IsMoveVertical() const
			{
				return data_.move.y() != 0;
			}

			template<typename MODULES_T>
			inline bool Func<MODULES_T>::IsMoveLeft() const
			{
				return data_.move.x() > 0;
			}

			template<typename MODULES_T>
			inline bool Func<MODULES_T>::IsMoveRight() const
			{
				return data_.move.x() < 0;
			}

			template<typename MODULES_T>
			inline bool Func<MODULES_T>::IsMoveDown() const
			{
				return data_.move.y() < 0;
			}

			template<typename MODULES_T>
			inline bool Func<MODULES_T>::IsMoveUp() const
			{
				return data_.move.y() > 0;
			}

			template<typename MODULES_T>
			inline Type::Direction Func<MODULES_T>::getMoveDirection() const
			{
				if (data_.move.x() > 0)
					return Type::Directions::left;
				else if (data_.move.x() < 0)
					return Type::Directions::right;
				else if (data_.move.y() > 0)
					return Type::Directions::up;
				else if (data_.move.y() < 0)
					return Type::Directions::down;
				else
					return Type::Directions::up;
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::SetMove(Type::Rotation rotation, Type::Move move)
			{
				if (rotation == Type::Rotations::Up)
					SetMove({data_.move.x(), move.y()});
				else if (rotation == Type::Rotations::Down)
					SetMove({data_.move.x(), -move.y()});
				else if (rotation == Type::Rotations::Left)
					SetMove({move.x(), data_.move.y()});
				else if (rotation == Type::Rotations::Right)
					SetMove({-move.x(), data_.move.y()});
			}
			
			template<typename MODULES_T>
			inline void Func<MODULES_T>::SetMoveUnsafe(Type::Rotation rotation, Type::Move move)
			{
				if (rotation == Type::Rotations::Up)
					SetMoveUnsafe({data_.move.x(), move.y()});
				else if (rotation == Type::Rotations::Down)
					SetMoveUnsafe({data_.move.x(), -move.y()});
				else if (rotation == Type::Rotations::Left)
					SetMoveUnsafe({move.x(), data_.move.y()});
				else if (rotation == Type::Rotations::Right)
					SetMoveUnsafe({-move.x(), data_.move.y()});
			}
		}
	}
}

#endif