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
			drawNumber = 0;
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
			return drawNumber;
		}
		bool Specific::UpdateDrawNumber()
		{
			std::int8_t drawNumber_ = limiter<std::int8_t>(0, numberOfFrames - 1,
														   static_cast<std::int8_t>((timer / time) * numberOfFrames));
			if (drawNumber != drawNumber_)
			{
				drawNumber = drawNumber_;
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
			pops(Specific, s);
			s->Initialize();

			stack->o->events.timer = true;

			stack->o->requests.timer = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["timer"] = s->timer;
			json["time"] = s->time;
			json["numberOfFrames"] = s->numberOfFrames;
			json["drawNumber"] = s->drawNumber;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);

			s->UpdateTimer();
			if (s->UpdateDrawNumber())
			{
				stack->o->requests.draw = true;
			}

			stack->o->requests.timer = true;
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			pops(Specific, s);
		}
	}


	namespace MoveDown
	{
		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->active = 0;
			stack->o->EnablePhysics();

			stack->o->events.timer = true;
			stack->o->events.update = true;

			stack->o->requests.timer = true;
			stack->o->requests.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["active"] = s->active;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);

			if (s->active)
			{
				if (s->active == 1)
				{
					stack->o->StepDown();
					if (!stack->o->IsMove())
						s->active = -1;
				}
				else if (s->active == -1)
				{
					s->active = 0;
					if (!stack->o->IsMoving())
						stack->o->StopStep();
				}
			}
			stack->o->requests.timer = true;
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			pops(Specific, s);

			if (s->active <= 0 && !stack->o->IsMoving())
			{
				if (stack->o->CanMoveDown())
				{
					//clog <<scene->GetObjectOut(GetCoordDown())->GetAbsMove() <<KIR4::eol;
					if (stack->o->scene->GetObjectOut(stack->o->GetCoordDown())->GetAbsMove() <= 0.5f)
					{
						s->active = 1;
						stack->o->scene->ObjectMove(stack->o->GetCoord(), stack->o->GetCoordDown(), 0);
						regets(Specific, s);
						stack->o->SetMove({stack->o->GetMove().x,-1});
					}
					else if (!stack->o->scene->EnableUpdateSkip())
					{
						stack->o->requests.update = true;
						return;
					}
				}
			}
		}
	}

	namespace MoveDownHeavy
	{
		void Blasting(ObjectBase *o)
		{
			o->scene->BlowUpBlock(o->GetCoordDown());
		}

		void Create(OBJECT_CREATER_PARAM)
		{
			MoveDown::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\MoveDown"] = MoveDown::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(MoveDown::Specific, s);

			if (s->active)
			{
				if (s->active == 1)
				{
					stack->o->StepDown();
					if (!stack->o->IsMove())
						s->active = -1;
				}
				else if (s->active == -1)
				{
					Blasting(stack->o);
					s->active = 0;
					if (!stack->o->IsMoving())
						stack->o->StopStep();
				}
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			MoveDown::Update(OBJECT_UPDATE_CALL);
		}
	}


	namespace MoveLeftWay
	{
		void Blasting(Type::Coord coord, ObjectBase *o)
		{
			o->scene->BlowUpBlock(coord);
		}
		bool CanMoveForward(Type::Coord to, ObjectBase *o)
		{
			return
				o->scene->GetObject(to)->GetFlags() & ObjectBase::Flags::StepOn
				&&
				o->scene->GetRemain(to)->GetFlags() & ObjectBase::Flags::StepOn
				&&
				o->scene->GetObjectOut(to)->GetFlags() & ObjectBase::Flags::StepOn
				;
		}
		bool CanExlosive(Type::Coord coord, ObjectBase *o)
		{
			ObjectBase *object;

			object = o->scene->GetObject(coord);
			if (object->GetFlags() & ObjectBase::Flags::CanBeKilled)
				return true;

			object = o->scene->GetObjectOut(coord);
			if (object->GetFlags() & ObjectBase::Flags::CanBeKilled && object->GetAbsMove() > 0.5f)
				return true;
			return false;
		}
		bool CanTurnLeft(ObjectBase *o)
		{
			Type::Coord
				CoordLeft = o->GetForwardCoord(o->GetRealRotation(o->GetRotation() - Type::Rotations::_90));
			return o->CanMovePos(CoordLeft, o->GetRealRotation(o->GetRotation() - Type::Rotations::_90)) || CanExlosive(CoordLeft, o);
		}

		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->active = 0;
			s->PriorityStep = false;
			stack->o->DisablePhysics();

			stack->o->events.timer = true;
			stack->o->events.update = true;

			stack->o->requests.timer = true;
			stack->o->requests.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["active"] = s->active;
			json["PriorityStep"] = s->PriorityStep;
			json["myNumber"] = s->myNumber;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);
			if (s->active == F_Step)
			{
				stack->o->Step();
				if (!stack->o->IsMove())
				{
					s->active = 0;
					stack->o->scene->ObjectArrived(stack->o->GetCoord());
				}
			}
			else if (s->active == F_TurnLeft)
			{
				if (stack->o->RotationLeft())
				{
					s->active = 0;
					stack->o->scene->ObjectArrived(stack->o->GetCoord());
				}
			}
			else if (s->active == F_TurnRight)
			{
				if (stack->o->RotationRight())
				{
					s->active = 0;
					stack->o->scene->ObjectArrived(stack->o->GetCoord());
				}
			}
			stack->o->requests.timer = true;
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			pops(Specific, s);
			s->myNumber = 123456789;
			if (s->active == 0 && !stack->o->IsMoving() && !stack->o->IsRotating())
			{
				if (s->PriorityStep)
				{
					Type::Coord
						to = stack->o->GetForwardCoord();
					if (CanExlosive(to, stack->o))
					{
						Blasting(to, stack->o);
					}
					else if (CanMoveForward(to, stack->o))
					{
						s->active = F_Step;
						stack->o->scene->ObjectMove(stack->o->GetCoord(), to, 0);
						regets(Specific, s);
						stack->o->SetMove(stack->o->GetRotation());
						s->PriorityStep = false;
					}
					else if (stack->o->scene->EnableUpdateSkip())
					{
						stack->o->requests.update = true;
						return;
					}
					else
					{
						stack->o->StartRotation();
						s->active = F_TurnRight;
						s->PriorityStep = false;
					}
				}
				else
				{
					if (CanTurnLeft(stack->o))
					{
						stack->o->StartRotation();
						s->active = F_TurnLeft;
						s->PriorityStep = true;
					}
					else
					{
						Type::Coord
							to = stack->o->GetForwardCoord();
						if (CanExlosive(to, stack->o))
						{
							Blasting(to, stack->o);
						}
						else if (CanMoveForward(to, stack->o))
						{
							s->active = F_Step;
							stack->o->scene->ObjectMove(stack->o->GetCoord(), to, 0);
							regets(Specific, s);
							stack->o->SetMove(stack->o->GetRotation());
							s->PriorityStep = false;
						}
						else if (stack->o->scene->EnableUpdateSkip())
						{
							stack->o->requests.update = true;
							return;
						}
						else
						{
							stack->o->StartRotation();
							s->active = F_TurnRight;
							s->PriorityStep = false;
						}
					}
				}
			}
		}
	}

	namespace RollDown
	{
		bool CanRollAffect(ObjectBase *o, Type::Coord coord1)
		{
			return
				!o->scene->IsObjectOut(o->GetCoordUp())
				||
				!o->scene->IsObjectOut({o->GetCoord().x,o->GetCoord().y - 2})
				||
				o->scene->GetGoto({o->GetCoord().x, o->GetCoord().y - 2}).x != coord1.x
				;
		}
		bool CanRoll(ObjectBase *o, Type::Coord coordNeighbor, Type::Coord coordDiagonal)
		{
			return
				(
					(
						o->scene->GetObject(coordNeighbor)->GetFlags() & ObjectBase::StepOn//a mellette l�v� object-re r� tud l�pni
						&&
						o->scene->GetRemain(coordNeighbor)->GetFlags() & ObjectBase::StepOn//a mellette l�v� remain-re r� tud l�pni
						&&
						(
							!o->scene->IsObjectOut(coordNeighbor)
							||
							(
								o->scene->GetObjectOut(coordNeighbor)->GetFlags() & ObjectBase::StepOn//a mellette l�v� kimen� object-ra r� tud l�pni
								||
								o->scene->GetObjectOut(coordNeighbor)->GetCoord().x != coordNeighbor.x//a mellette l�v� kimen� object horizont�lisan t�vozik
								)
							)
						&&
						(
							o->scene->GetObject(coordDiagonal)->GetFlags() & ObjectBase::StepOn//�tl�s object-re r� tud l�pni
							&&
							o->scene->GetRemain(coordDiagonal)->GetFlags() & ObjectBase::StepOn//�tl�s remain-re r� tud l�pni
							&&
							(
								!o->scene->IsObjectOut(coordDiagonal)
								||
								(
									o->scene->GetObjectOut(coordDiagonal)->GetFlags() & ObjectBase::StepOn//�tl�s kimen� object-ra r� tud l�pni
									||
									o->scene->GetObjectOut(coordDiagonal)->IsMoving()//�tl�s kimen� object t�vozik
									//GetObjectOut(coord2)->GetCoord().y > GetCoord().y//�tl�s kimen� object lefel� t�vozik
									)
								)
							)
						)
					)
				;
		}
		bool CanRollOff(ObjectBase *o)
		{
			return
				o->scene->GetSectionFlags(o->GetCoordDown()) & ObjectBase::RollOffTop;
		}

		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->active = 0;
			s->rollCounter = 0;

			stack->o->events.timer = true;
			stack->o->events.update = true;

			stack->o->requests.timer = true;
			stack->o->requests.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["active"] = s->active;
			json["rollCounter"] = s->rollCounter;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);
			if (s->active == 1)
			{
				stack->o->Step();
				if (!stack->o->IsMove())
					s->active = -1;
			}
			else if (s->active == -1)
			{
				s->active = 0;
				s->rollCounter = 0;
				if (!stack->o->IsMoving())
					stack->o->StopStep();
			}
			stack->o->requests.timer = true;
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			pops(Specific, s);

			if (s->active <= 0 && !stack->o->IsMoving())
			{
				if (CanRollOff(stack->o))
				{
					bool left = CanRoll(stack->o, stack->o->GetCoordLeft(), {stack->o->GetCoord().x - 1,stack->o->GetCoord().y + 1});
					if (s->rollCounter <= 0 && left && CanRollAffect(stack->o, stack->o->GetCoordLeft()))
					{
						s->active = 1;
						stack->o->scene->ObjectMove(stack->o->GetCoord(), stack->o->GetCoordLeft(), 0);
						stack->o->SetMove({1,stack->o->GetMove().y});
						if (s->rollCounter >= 0)
						{
							s->rollCounter = -1;
						}
						else
						{
							--s->rollCounter;
						}
					}
					else if (s->rollCounter >= 0 && CanRoll(stack->o, stack->o->GetCoordRight(), {stack->o->GetCoord().x + 1,stack->o->GetCoord().y + 1}) && CanRollAffect(stack->o, stack->o->GetCoordRight()))
					{
						s->active = 1;
						stack->o->scene->ObjectMove(stack->o->GetCoord(), stack->o->GetCoordRight(), 0);
						stack->o->SetMove({-1,stack->o->GetMove().y});
						if (s->rollCounter <= 0)
						{
							s->rollCounter = 1;
						}
						else
						{
							++s->rollCounter;
						}
					}
					else if (stack->o->scene->EnableUpdateSkip())
					{
						stack->o->requests.update = true;
					}
					else if (s->rollCounter <= 0 && left)
					{
						s->active = 1;
						stack->o->scene->ObjectMove(stack->o->GetCoord(), stack->o->GetCoordLeft(), 0);
						stack->o->SetMove({1,stack->o->GetMove().y});
						if (s->rollCounter >= 0)
						{
							s->rollCounter = -1;
						}
						else
						{
							--s->rollCounter;
						}
					}
					else
					{
						s->rollCounter = 0;
					}
				}
			}
		}
	}
}