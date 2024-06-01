#include "BaseFunctions.h"
#include "Object.h"
#include "Tools.h"
#include <KIR/KIR4_console.h>

namespace Object
{
	bool ACTION_TIMER(float &timer,
					  const float &duration,
					  Brick &_brick,
					  const std::function<bool()> &triggerEvent,
					  const std::function<bool()> &timerStarted,
					  const std::function<bool()> &timerRunning,
					  const std::function<bool()> &timerExpired)
	{
		if (timer > 0)
		{
			timer -= CA;
			if (timer <= 0)
			{
				timer = 0;
				return timerExpired();
			}
			else
			{
				_brick.requests.timer = true;
				return timerRunning();
			}
		}
		else
		{
			if (triggerEvent())
			{
				timer = duration;
				_brick.requests.timer = true;
				return timerStarted();
			}
		}
		return false;
	}

	void DRAW_NUMBER_INVALIDATE(DrawNumber &_draw_number)
	{
		_draw_number = std::numeric_limits<DrawNumber>::max();
	}
	void DRAW_NUMBER_ASC_INIT(DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp)
	{
		_draw_number = 0;
		_brick.requests.draw = true;
	}
	void DRAW_NUMBER_ASC(float timer, const float duration, DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp)
	{
		DrawNumber draw_number = static_cast<DrawNumber>(bmp.getDrawNumber(1 - (timer / duration)));
		if (_draw_number != draw_number)
		{
			_brick.requests.draw = true;
			_draw_number = draw_number;
		}
	}
	void DRAW_NUMBER_DESC_INIT(DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp)
	{
		_draw_number = bmp.getCount() - 1;
		_brick.requests.draw = true;
	}
	void DRAW_NUMBER_DESC(float timer, const float duration, DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp)
	{
		DrawNumber draw_number = static_cast<DrawNumber>(bmp.getDrawNumber(timer / duration));
		if (_draw_number != draw_number)
		{
			_brick.requests.draw = true;
			_draw_number = draw_number;
		}
	}

	namespace Animator
	{
		void EntityData::Initialize()
		{
			timer = 0.f;
			time = 0.f;
			numberOfFrames = 0;
			draw_number_ = 0;
		}
		void EntityData::SetAnimationTime(std::float_t _time)
		{
			std::uniform_real_distribution<std::float_t> distribution(0.f, _time);

			time = _time;
			timer = distribution(generator);
			UpdateDrawNumber();
		}
		void EntityData::SetNumberOfFrames(std::int8_t _numberOfFrames)
		{
			numberOfFrames = _numberOfFrames;
		}
		std::int8_t EntityData::GetDrawNumber()
		{
			return draw_number_;
		}
		bool EntityData::UpdateDrawNumber()
		{
			DrawNumber _draw_number = limiter<std::int8_t>(0, numberOfFrames - 1,
														   static_cast<std::int8_t>((timer / time) * numberOfFrames));
			if (draw_number_ != _draw_number)
			{
				draw_number_ = _draw_number;
				return true;
			}
			return false;
		}
		void EntityData::UpdateTimer()
		{
			timer += CA;
			if (timer > time)
			{
				timer -= time;
			}
		}

		void Create(Object::Brick &_brick, EntityData &_entity_data)
		{
			_entity_data.Initialize();

			_brick.events.timer = true;

			_brick.requests.timer = true;
		}
		OBJECT_PRINTER_RET Print(Object::Brick &_brick, EntityData &_entity_data)
		{
			Json json;

			json["timer"] = _entity_data.timer;
			json["time"] = _entity_data.time;
			json["numberOfFrames"] = _entity_data.numberOfFrames;
			json["draw_number"] = _entity_data.draw_number_;

			return json;
		}
		void Timer(Object::Brick &_brick, EntityData &_entity_data)
		{
			_entity_data.UpdateTimer();
			if (_entity_data.UpdateDrawNumber())
			{
				_brick.requests.draw = true;
			}

			_brick.requests.timer = true;
		}
		void Update(Object::Brick &_brick, EntityData &_entity_data, UpdateType _updateType)
		{
		}
	}


	namespace Fall
	{
		void heavyAction(Brick &_brick, EntityData &_entity_data)
		{
			if (_brick.isAction(Brick::MOVE_DOWN))
			{
				if (_entity_data.heavy_object_)
				{
					Type::Coord coord_down = _brick.GetCoordDown();
					Brick &_below_brick = _brick.scene->GetObject(coord_down);
					_brick.scene->blowup(_below_brick);
					if (_brick.scene->IsObjectOut(coord_down))
					{
						Brick &_below_out_brick = _brick.scene->GetObjectOut(coord_down);
						if (_brick.scene->getMoveProgress(_below_out_brick) < 0.5f)
						{
							_brick.scene->blowup(_below_out_brick);
						}
					}
				}
			}
		}
		void EntityData::setHeavy(bool _heavy_object)
		{
			heavy_object_ = _heavy_object;
		}

		void Create(Object::Brick &_brick, EntityData &_entity_data)
		{
			_entity_data.heavy_object_ = false;
			_brick.enablePhysics();
			_brick.AddFlags(Brick::Flags::FallDownRule);

			_brick.events.timer = true;
			_brick.events.update = true;

			_brick.requests.timer = true;
			_brick.requests.update = true;
		}
		OBJECT_PRINTER_RET Print(Object::Brick &_brick, EntityData &_entity_data)
		{
			Json json;

			json["heavy_object"] = _entity_data.heavy_object_;

			return json;
		}
		void Timer(Object::Brick &_brick, EntityData &_entity_data)
		{
			if (_brick.isActionMove())
			{
				_brick.StepDown();
				_brick.requests.timer = true;
			}
		}
		void Update(Object::Brick &_brick, EntityData &_entity_data, UpdateType _updateType)
		{
			if (_updateType == UpdateType::UPDATE_ASC)
			{
				_brick.requests.update = true;
				return;
			}

			if (!_brick.IsMove())
			{
				if (_brick.CanMoveDown())
				{
					if (_brick.scene->GetObjectOut(_brick.GetCoordDown()).GetAbsMove() <= 0.5f)
					{
						_brick.doMove(Brick::MOVE_DOWN, ObjectID::Space);
						_brick.requests.timer = true;
						return;
					}
				}
				if (_brick.isActionMove())
				{
					_brick.finishMove();
					heavyAction(_brick, _entity_data);
				}
			}
		}
	}
	namespace FallAndRoll
	{
		bool CanRollAffect(Brick &_brick, Type::Coord coord1)
		{
			return
				!_brick.scene->IsObjectOut(_brick.GetCoordUp())
				||
				!_brick.scene->IsObjectOut({_brick.GetCoord().x(),_brick.GetCoord().y() - 2})
				||
				_brick.scene->GetGoto({_brick.GetCoord().x(), _brick.GetCoord().y() - 2}).x() != coord1.x()
				;
		}
		bool CanRoll(Brick &_brick, Type::Direction _direction)
		{
			Type::Coord coord = _brick.GetCoord();
			Type::Coord coord_bottom = _brick.GetCoordDown();
			Type::Coord coord_up = _brick.GetCoordUp();
			Type::Coord coord_next = _brick.GetCoord(_direction);
			Type::Coord coord_up_diagonal = _brick.scene->GetObject(coord_next).GetCoordUp();
			Type::Coord coord_bottom_diagonal = _brick.scene->GetObject(coord_next).GetCoordDown();

			return
				(
					_brick.scene->GetObject(coord_next).GetFlags() & Flags::StepOn // it can step to the next place (object)
					&&
					_brick.scene->GetRemain(coord_next).GetFlags() & Flags::StepOn // it can step to the next place (remain)
					&&
					(
						!(_brick.scene->GetObject(coord_up_diagonal).GetFlags() & Flags::FallDownRule) // The object there will not fall down
						||
						_brick.scene->GetObject(coord_up_diagonal).GetAbsMove() >= 0.5f // If the object can fall down, but it is far away yet
						)
					&&
					(
						!_brick.scene->IsObjectOut(coord_next)
						||
						(
							_brick.scene->GetObjectOut(coord_next).GetFlags() & Flags::StepOn // it can step to the next place (leaving)
							||
							_brick.scene->GetObjectOut(coord_next).GetCoord().x() != coord_next.x() // the leaving one moving away on the X
							)
						)
					&&
					(
						_brick.scene->GetObject(coord_bottom_diagonal).GetFlags() & Flags::StepOn // it can step to the diagonal place (object)
						&&
						_brick.scene->GetRemain(coord_bottom_diagonal).GetFlags() & Flags::StepOn // it can step to the diagonal place (remain)
						&&
						(
							!_brick.scene->IsObjectOut(coord_bottom_diagonal) // there is no leaving object in the diagonal
							||
							(
								_brick.scene->GetObjectOut(coord_bottom_diagonal).GetFlags() & Flags::StepOn // it can step to the next place (leaving)
								||
								(
									_brick.scene->GetObjectOut(coord_bottom_diagonal).isActionMove() // the leaving one moving away
									&&
									_brick.scene->GetObjectOut(coord_bottom_diagonal).GetCoord().x() != coord.x() // the leaving one is not moving under it
									)
								)
							)
						)
					&&
					(
						!_brick.scene->IsObjectOut(coord_up) // there is no leaving object on the top
						||
						(
							_brick.scene->GetObjectOut(coord_up).GetCoord().x() != coord_next.x() // the leaving object on the top moving different direction
							||
							_brick.scene->GetObjectOut(coord_up).GetAbsMove() >= 0.9f // the leaving object on the top at the beginning of it's move
							)
						)
					)
				;
		}
		bool CanRollOff(Brick &_brick)
		{
			return
				_brick.scene->GetSectionFlags(_brick.GetCoordDown()) & Flags::RollOffTop;
		}
		void finishAction(Brick &_brick, EntityData &_entity_data)
		{
			if (_brick.isAction())
			{
				Fall::heavyAction(_brick, _entity_data);
				_entity_data.roll_preference_ = 0;
				_brick.finishMove();
			}
		}
		void changeAction(Brick &_brick, EntityData &_entity_data, Brick::ACTION_T _new_action)
		{
			if (_brick.action != _new_action)
			{
				Fall::heavyAction(_brick, _entity_data);
			}
		}

		void Create(Object::Brick &_brick, EntityData &_entity_data)
		{
			_entity_data.heavy_object_ = false;
			_brick.enablePhysics();
			_brick.AddFlags(
				Brick::Flags::FallDownRule |
				Brick::Flags::RollHorizontalRule);

			_brick.events.timer = true;
			_brick.events.update = true;

			_brick.requests.timer = true;
			_brick.requests.update = true;
		}
		OBJECT_PRINTER_RET Print(Object::Brick &_brick, EntityData &_entity_data)
		{
			Json json;

			json["heavy_object"] = _entity_data.heavy_object_;
			json["roll_preference"] = _entity_data.roll_preference_;

			return json;
		}
		void Timer(Object::Brick &_brick, EntityData &_entity_data)
		{
			if (_brick.isActionMove())
			{
				_brick.Step();
				_brick.requests.timer = true;
			}
		}
		void Update(Object::Brick &_brick, EntityData &_entity_data, UpdateType _updateType)
		{
			if (!_brick.IsMove())
			{
				if (_updateType == UpdateType::UPDATE_DESC && _brick.CanMoveDown())
				{
					if (_brick.scene->GetObjectOut(_brick.GetCoordDown()).GetAbsMove() <= 0.5f)
					{
						changeAction(_brick, _entity_data, Brick::MOVE_DOWN);
						_brick.doMove(Brick::MOVE_DOWN, ObjectID::Space);
						_brick.requests.timer = true;
						_entity_data.roll_preference_ = 0;
						return;
					}
				}

				if (CanRollOff(_brick))
				{
					bool can_roll_left = CanRoll(_brick, Type::Directions::left);
					if (_entity_data.roll_preference_ == -1 && can_roll_left && CanRollAffect(_brick, _brick.GetCoordLeft()))
					{
						changeAction(_brick, _entity_data, Brick::MOVE_LEFT);
						_entity_data.roll_preference_ = -1;
						_brick.doMove(Brick::MOVE_LEFT, ObjectID::Space);
						_brick.requests.timer = true;
						return;
					}
					else if ((_entity_data.roll_preference_ == 1 || !can_roll_left) &&
							 CanRoll(_brick, Type::Directions::right) &&
							 CanRollAffect(_brick, _brick.GetCoordRight()))
					{
						changeAction(_brick, _entity_data, Brick::MOVE_RIGHT);
						_entity_data.roll_preference_ = 1;
						_brick.doMove(Brick::MOVE_RIGHT, ObjectID::Space);
						_brick.requests.timer = true;
						return;
					}
					else if (can_roll_left)
					{
						changeAction(_brick, _entity_data, Brick::MOVE_LEFT);
						_entity_data.roll_preference_ = -1;
						_brick.doMove(Brick::MOVE_LEFT, ObjectID::Space);
						_brick.requests.timer = true;
						return;
					}
				}

				if (_updateType == UpdateType::UPDATE_ASC)
				{
					_brick.requests.update = true;
					return;
				}

				finishAction(_brick, _entity_data);
			}
		}
	}


	namespace MoveLeftWay
	{
		bool CanMoveForward(Type::Coord to, Brick &_brick)
		{
			return
				_brick.scene->GetObject(to).GetFlags() & Brick::Flags::StepOn
				&&
				_brick.scene->GetRemain(to).GetFlags() & Brick::Flags::StepOn
				&&
				_brick.scene->GetObjectOut(to).GetFlags() & Brick::Flags::StepOn
				;
		}
		bool CanExlosive(Type::Coord coord, Brick &_brick, bool _do_explosive)
		{
			Brick &_target_brick = _brick.scene->GetObject(coord);
			if (_target_brick.GetFlags() & Brick::Flags::CanBeKilled)
			{
				if (_do_explosive)
				{
					_brick.scene->blowup(_target_brick);
				}
				return true;
			}

			Brick &_target_out_brick = _brick.scene->GetObjectOut(coord);
			if (_target_out_brick.GetFlags() & Brick::Flags::CanBeKilled &&
				_brick.scene->getMoveProgress(_target_out_brick) < 0.5f)
			{
				if (_do_explosive)
				{
					_brick.scene->blowup(_target_out_brick);
				}
				return true;
			}
			return false;
		}
		bool CanTurnLeft(Brick &_brick)
		{
			Type::Coord
				CoordLeft = _brick.GetForwardCoord(_brick.GetRealRotation(_brick.GetRotation() - Type::Rotations::_90));
			return _brick.CanMovePos(CoordLeft, _brick.GetRealRotation(_brick.GetRotation() - Type::Rotations::_90)) || CanExlosive(CoordLeft, _brick, false);
		}
		void finishAction(Brick &_brick)
		{
			if (_brick.isActionMove())
			{
				_brick.finishMove();
			}
			else if (_brick.isActionRotate())
			{
				_brick.finishMove();
			}
		}
		void finishAction(Brick &_brick, Brick::ACTION_T _new_action)
		{
			if (_brick.action != _new_action)
			{
				finishAction(_brick);
			}
		}

		void Create(Object::Brick &_brick, EntityData &_entity_data)
		{
			_entity_data.PriorityStep = false;
			_brick.disablePhysics();

			_brick.events.timer = true;
			_brick.events.update = true;

			_brick.requests.timer = true;
			_brick.requests.update = true;
		}
		OBJECT_PRINTER_RET Print(Object::Brick &_brick, EntityData &_entity_data)
		{
			Json json;

			json["PriorityStep"] = _entity_data.PriorityStep;

			return json;
		}
		void Timer(Object::Brick &_brick, EntityData &_entity_data)
		{
			if (_brick.isActionMove())
			{
				_brick.Step();
			}
			else if (_brick.isActionRotate())
			{
				if (_brick.isAction(Brick::ROTATE_LEFT))
				{
					_brick.RotationLeft();
				}
				else
				{
					_brick.RotationRight();
				}
			}
			_brick.requests.timer = true;
		}
		void Update(Object::Brick &_brick, EntityData &_entity_data, UpdateType _updateType)
		{
			if (!_brick.IsMove() && !_brick.isRotate())
			{
				if (!_entity_data.PriorityStep)
				{
					if (CanTurnLeft(_brick))
					{
						_entity_data.PriorityStep = true;
						finishAction(_brick, Brick::ROTATE_LEFT);
						_brick.doRotate(Brick::ROTATE_LEFT);
						return;
					}
				}
				Type::Coord to = _brick.GetForwardCoord();
				if (CanExlosive(to, _brick, true))
				{
					_brick.scene->blowup(_brick);
					return;
				}
				else if (CanMoveForward(to, _brick))
				{
					_entity_data.PriorityStep = false;
					Brick::ACTION_T new_action = Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(_brick.GetRotation())];
					finishAction(_brick, new_action);
					_brick.doMove(new_action, ObjectID::Space);
					return;
				}
				else if (_updateType == UpdateType::UPDATE_ASC)
				{
					_brick.requests.update = true;
					return;
				}
				else
				{
					_entity_data.PriorityStep = true;
					finishAction(_brick, Brick::ROTATE_RIGHT);
					_brick.doRotate(Brick::ROTATE_RIGHT);
					return;
				}
			}
		}
	}
}