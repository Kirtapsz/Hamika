#include "BaseFunctions.h"
#include "Tools.h"
#include <KIR/KIR4_console.h>

namespace Object
{
	namespace Animator
	{
		void Specific::Initialize()
		{
			timer = 0.f;
			time = 0.f;
			numberOfFrames = 0;
			draw_number_ = 0;
		}
		void Specific::SetAnimationTime(std::float_t _time)
		{
			std::uniform_real_distribution<std::float_t> distribution(0.f, _time);

			time = _time;
			timer = distribution(generator);
			UpdateDrawNumber();
		}
		void Specific::SetNumberOfFrames(std::int8_t _numberOfFrames)
		{
			numberOfFrames = _numberOfFrames;
		}
		std::int8_t Specific::GetDrawNumber()
		{
			return draw_number_;
		}
		bool Specific::UpdateDrawNumber()
		{
			DRAW_NUMBER_T _draw_number = limiter<std::int8_t>(0, numberOfFrames - 1,
															  static_cast<std::int8_t>((timer / time) * numberOfFrames));
			if (draw_number_ != _draw_number)
			{
				draw_number_ = _draw_number;
				return true;
			}
			return false;
		}
		void Specific::UpdateTimer()
		{
			timer += CA;
			if (timer > time)
			{
				timer -= time;
			}
		}

		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->Initialize();

			stack->o->events.timer = true;

			stack->o->requests.timer = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["timer"] = spec->timer;
			json["time"] = spec->time;
			json["numberOfFrames"] = spec->numberOfFrames;
			json["draw_number"] = spec->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			spec->UpdateTimer();
			if (spec->UpdateDrawNumber())
			{
				stack->o->requests.draw = true;
			}

			stack->o->requests.timer = true;
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
		}
	}


	namespace Fall
	{
		template<typename SPECIFIC>
		void heavyAction(Brick *o, SPECIFIC *spec)
		{
			if (o->isAction(Brick::MOVE_DOWN))
			{
				if (spec->heavy_object_)
				{
					Type::Coord coord_down = o->GetCoordDown();
					Brick *object = o->scene->GetObject(coord_down);
					o->scene->blowup(object);
					if (o->scene->IsObjectOut(coord_down))
					{
						Brick *object = o->scene->GetObjectOut(coord_down);
						if (o->scene->getMoveProgress(object) < 0.5f)
						{
							o->scene->blowup(object);
						}
					}
				}
			}
		}
		void setHeavy(Specific *spec, bool _heavy_object)
		{
			spec->heavy_object_ = _heavy_object;
		}

		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->heavy_object_ = false;
			stack->o->enablePhysics();

			stack->o->events.timer = true;
			stack->o->events.update = true;

			stack->o->requests.timer = true;
			stack->o->requests.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["heavy_object"] = spec->heavy_object_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			if (stack->o->isActionMove())
			{
				stack->o->StepDown();
				stack->o->requests.timer = true;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			if (updateType == Brick::UPDATE_ASC)
			{
				stack->o->requests.update = true;
				return;
			}

			if (!stack->o->IsMove())
			{
				if (stack->o->CanMoveDown())
				{
					if (stack->o->scene->GetObjectOut(stack->o->GetCoordDown())->GetAbsMove() <= 0.5f)
					{
						stack->o->doMove(Brick::MOVE_DOWN, ObjectID::Space);
						stack->o->requests.timer = true;
						return;
					}
				}
				if (stack->o->isActionMove())
				{
					Object::Brick::Stack::Handler<Specific> sHandler(stack);
					Specific *spec = sHandler;

					stack->o->finishMove();
					heavyAction(stack->o, spec);
				}
			}
		}
	}
	namespace FallAndRoll
	{
		void setHeavy(Specific *spec, bool _heavy_object)
		{
			spec->heavy_object_ = _heavy_object;
		}

		bool CanRollAffect(Brick *o, Type::Coord coord1)
		{
			return
				!o->scene->IsObjectOut(o->GetCoordUp())
				||
				!o->scene->IsObjectOut({o->GetCoord().x(),o->GetCoord().y() - 2})
				||
				o->scene->GetGoto({o->GetCoord().x(), o->GetCoord().y() - 2}).x() != coord1.x()
				;
		}
		bool CanRoll(Brick *object, Type::Direction _direction)
		{
			Type::Coord coord = object->GetCoord();
			Type::Coord coord_bottom = object->GetCoordDown();
			Type::Coord coord_up = object->GetCoordUp();
			Type::Coord coord_next = object->GetCoord(_direction);
			Type::Coord coord_diagonal = object->scene->GetObject(coord_next)->GetCoordDown();

			return
				(
					object->scene->GetObject(coord_next)->GetFlags() & Brick::StepOn // it can step to the next place (object)
					&&
					object->scene->GetRemain(coord_next)->GetFlags() & Brick::StepOn // it can step to the next place (remain)
					&&
					(
						!object->scene->IsObjectOut(coord_next)
						||
						(
							object->scene->GetObjectOut(coord_next)->GetFlags() & Brick::StepOn // it can step to the next place (leaving)
							||
							object->scene->GetObjectOut(coord_next)->GetCoord().x() != coord_next.x() // the leaving one moving away on the X
							)
						)
					&&
					(
						object->scene->GetObject(coord_diagonal)->GetFlags() & Brick::StepOn // it can step to the diagonal place (object)
						&&
						object->scene->GetRemain(coord_diagonal)->GetFlags() & Brick::StepOn // it can step to the diagonal place (remain)
						&&
						(
							!object->scene->IsObjectOut(coord_diagonal) // there is no leaving object in the diagonal
							||
							(
								object->scene->GetObjectOut(coord_diagonal)->GetFlags() & Brick::StepOn // it can step to the next place (leaving)
								||
								(
									object->scene->GetObjectOut(coord_diagonal)->isActionMove() // the leaving one moving away
									&&
									object->scene->GetObjectOut(coord_diagonal)->GetCoord().x() != coord.x() // the leaving one is not moving under it
									)
								)
							)
						)
					&&
					(
						!object->scene->IsObjectOut(coord_up) // there is no leaving object on the top
						||
						(
							object->scene->GetObjectOut(coord_up)->GetCoord().x() != coord_next.x() // the leaving object on the top moving different direction
							||
							object->scene->GetObjectOut(coord_up)->GetAbsMove() >= 0.9f // the leaving object on the top at the beginning of it's move
							)
						)
					)
				;
		}
		bool CanRollOff(Brick *o)
		{
			return
				o->scene->GetSectionFlags(o->GetCoordDown()) & Brick::RollOffTop;
		}
		void finishAction(Brick *o, Specific *spec)
		{
			if (o->isAction())
			{
				Fall::heavyAction(o, spec);
				spec->roll_preference_ = 0;
				o->finishMove();
			}
		}
		void changeAction(Brick *o, Specific *spec, Brick::ACTION_T _new_action)
		{
			if (o->action != _new_action)
			{
				Fall::heavyAction(o, spec);
			}
		}

		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->heavy_object_ = false;
			stack->o->enablePhysics();

			stack->o->events.timer = true;
			stack->o->events.update = true;

			stack->o->requests.timer = true;
			stack->o->requests.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["heavy_object"] = spec->heavy_object_;
			json["roll_preference"] = spec->roll_preference_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			if (stack->o->isActionMove())
			{
				stack->o->Step();
				stack->o->requests.timer = true;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			if (!stack->o->IsMove())
			{
				Object::Brick::Stack::Handler<Specific> sHandler(stack);
				Specific *spec = sHandler;

				if (updateType == Brick::UPDATE_DESC && stack->o->CanMoveDown())
				{
					if (stack->o->scene->GetObjectOut(stack->o->GetCoordDown())->GetAbsMove() <= 0.5f)
					{
						changeAction(stack->o, spec, Brick::MOVE_DOWN);
						stack->o->doMove(Brick::MOVE_DOWN, ObjectID::Space);
						stack->o->requests.timer = true;
						spec->roll_preference_ = 0;
						return;
					}
				}

				if (CanRollOff(stack->o))
				{
					bool can_roll_left = CanRoll(stack->o, Type::Directions::left);
					if (spec->roll_preference_ == -1 && can_roll_left && CanRollAffect(stack->o, stack->o->GetCoordLeft()))
					{
						changeAction(stack->o, spec, Brick::MOVE_LEFT);
						spec->roll_preference_ = -1;
						stack->o->doMove(Brick::MOVE_LEFT, ObjectID::Space);
						stack->o->requests.timer = true;
						return;
					}
					else if ((spec->roll_preference_ == 1 || !can_roll_left) &&
							 CanRoll(stack->o, Type::Directions::right) &&
							 CanRollAffect(stack->o, stack->o->GetCoordRight()))
					{
						changeAction(stack->o, spec, Brick::MOVE_RIGHT);
						spec->roll_preference_ = 1;
						stack->o->doMove(Brick::MOVE_RIGHT, ObjectID::Space);
						stack->o->requests.timer = true;
						return;
					}
					else if (can_roll_left)
					{
						changeAction(stack->o, spec, Brick::MOVE_LEFT);
						spec->roll_preference_ = -1;
						stack->o->doMove(Brick::MOVE_LEFT, ObjectID::Space);
						stack->o->requests.timer = true;
						return;
					}
				}

				if (updateType == Brick::UPDATE_ASC)
				{
					stack->o->requests.update = true;
					return;
				}

				finishAction(stack->o, spec);
			}
		}
	}


	namespace MoveLeftWay
	{
		bool CanMoveForward(Type::Coord to, Brick *o)
		{
			return
				o->scene->GetObject(to)->GetFlags() & Brick::Flags::StepOn
				&&
				o->scene->GetRemain(to)->GetFlags() & Brick::Flags::StepOn
				&&
				o->scene->GetObjectOut(to)->GetFlags() & Brick::Flags::StepOn
				;
		}
		bool CanExlosive(Type::Coord coord, Brick *o, bool _do_explosive)
		{
			Brick *object;

			object = o->scene->GetObject(coord);
			if (object->GetFlags() & Brick::Flags::CanBeKilled)
			{
				if (_do_explosive)
				{
					o->scene->blowup(object);
				}
				return true;
			}

			object = o->scene->GetObjectOut(coord);
			if (object->GetFlags() & Brick::Flags::CanBeKilled &&
				o->scene->getMoveProgress(object) < 0.5f)
			{
				if (_do_explosive)
				{
					o->scene->blowup(object);
				}
				return true;
			}
			return false;
		}
		bool CanTurnLeft(Brick *o)
		{
			Type::Coord
				CoordLeft = o->GetForwardCoord(o->GetRealRotation(o->GetRotation() - Type::Rotations::_90));
			return o->CanMovePos(CoordLeft, o->GetRealRotation(o->GetRotation() - Type::Rotations::_90)) || CanExlosive(CoordLeft, o, false);
		}
		void finishAction(Brick *o)
		{
			if (o->isActionMove())
			{
				o->finishMove();
			}
			else if (o->isActionRotate())
			{
				o->finishMove();
			}
		}
		void finishAction(Brick *o, Brick::ACTION_T _new_action)
		{
			if (o->action != _new_action)
			{
				finishAction(o);
			}
		}

		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->PriorityStep = false;
			stack->o->disablePhysics();

			stack->o->events.timer = true;
			stack->o->events.update = true;

			stack->o->requests.timer = true;
			stack->o->requests.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["PriorityStep"] = spec->PriorityStep;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			if (stack->o->isActionMove())
			{
				stack->o->Step();
			}
			else if (stack->o->isActionRotate())
			{
				if (stack->o->isAction(Brick::ROTATE_LEFT))
				{
					stack->o->RotationLeft();
				}
				else
				{
					stack->o->RotationRight();
				}
			}
			stack->o->requests.timer = true;
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			if (!stack->o->IsMove() && !stack->o->isRotate())
			{
				if (!spec->PriorityStep)
				{
					if (CanTurnLeft(stack->o))
					{
						spec->PriorityStep = true;
						finishAction(stack->o, Brick::ROTATE_LEFT);
						stack->o->doRotate(Brick::ROTATE_LEFT);
						return;
					}
				}
				Type::Coord to = stack->o->GetForwardCoord();
				if (CanExlosive(to, stack->o, true))
				{
					stack->o->scene->blowup(stack->o);
					return;
				}
				else if (CanMoveForward(to, stack->o))
				{
					spec->PriorityStep = false;
					Brick::ACTION_T new_action = Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())];
					finishAction(stack->o, new_action);
					stack->o->doMove(new_action, ObjectID::Space);
					return;
				}
				else if (updateType == Brick::UPDATE_ASC)
				{
					stack->o->requests.update = true;
					return;
				}
				else
				{
					spec->PriorityStep = true;
					finishAction(stack->o, Brick::ROTATE_RIGHT);
					stack->o->doRotate(Brick::ROTATE_RIGHT);
					return;
				}
			}
		}
	}
}