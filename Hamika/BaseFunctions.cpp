#include "BaseFunctions.h"
#include "Global.h"
#include <KIR/KIR4_console.h>

namespace Object
{
	namespace Animator
	{
		void Specific::SetAnimationTime(float time)
		{
			std::uniform_real_distribution<float> distribution(0.f, time);

			this->time = time;
			this->timer = distribution(generator);
		}
		void Specific::SetNumberOfFrames(int numberOfFrames)
		{
			this->numberOfFrames = numberOfFrames;
		}
		int Specific::GetDrawNumber()
		{
			return drawNumber;
		}


		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->timer = 0.f;
			s->time = 0.f;
			s->numberOfFrames = 0;
			s->drawNumber = 0;

			stack->o->events.timer = true;

			stack->o->requests.timer = true;
		}
		void Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			clog << "Animator::timer: " << s->timer << "\n";
			clog << "Animator::time: " << s->time << "\n";
			clog << "Animator::numberOfFrames: " << s->numberOfFrames << "\n";
			clog << "Animator::drawNumber: " << s->drawNumber << "\n";
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);
			s->timer += CA;
			if (s->timer > s->time)
			{
				s->timer -= s->time;
			}

			int drawNumber_ = (std::max)(0, (std::min)((int)(s->numberOfFrames - 1), (int)((1 - (s->timer / s->time)) * s->numberOfFrames)));
			if (s->drawNumber != drawNumber_)
			{
				stack->o->requests.draw = true;
				s->drawNumber = drawNumber_;
			}

			stack->o->requests.timer = true;
		}
		void Tick(OBJECT_TICK_PARAM)
		{
			pops(Specific, s);
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
		void Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			clog << "Move Down Is Active: " << s->active << "\n";
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
		void Tick(OBJECT_TICK_PARAM)
		{
			pops(Specific, s);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			pops(Specific, s);

			if (s->active <= 0 && !stack->o->IsMoving())
			{
				if (stack->o->CanMoveDown())
				{
					//clog << ief.GetObjectOut(GetCoordDown())->GetAbsMove() << KIR4::eol;
					if (stack->o->ief.GetObjectOut(stack->o->GetCoordDown())->GetAbsMove() <= 0.5f)
					{
						s->active = 1;
						stack->o->ief.ObjectMove(stack->o->GetCoord(), stack->o->GetCoordDown(), 0);
						regets(Specific, s);
						stack->o->SetMove({stack->o->GetMove().x,-1});
					}
					else if (!stack->o->ief.EnableUpdateSkip())
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
			o->ief.BlowUpBlock(o->GetCoordDown());
		}

		void Create(OBJECT_CREATER_PARAM)
		{
			MoveDown::Create(OBJECT_CREATER_CALL);
		}
		void Print(OBJECT_PRINTER_PARAM)
		{
			MoveDown::Print(OBJECT_PRINTER_CALL);
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
		void Tick(OBJECT_TICK_PARAM)
		{
			MoveDown::Tick(OBJECT_TICK_CALL);
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
			o->ief.BlowUpBlock(coord);
		}
		bool CanMoveForward(Type::Coord to, ObjectBase *o)
		{
			return
				o->ief.GetObject(to)->GetFlags() & ObjectBase::Flags::StepOn
				&&
				o->ief.GetRemain(to)->GetFlags() & ObjectBase::Flags::StepOn
				&&
				o->ief.GetObjectOut(to)->GetFlags() & ObjectBase::Flags::StepOn
				;
		}
		bool CanExlosive(Type::Coord coord, ObjectBase *o)
		{
			ObjectBase *object;

			object = o->ief.GetObject(coord);
			if (object->GetFlags() & ObjectBase::Flags::CanBeKilled)
				return true;

			object = o->ief.GetObjectOut(coord);
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
		void Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			clog << "Move Active: " << s->active << "\n";
			clog << "PriorityStep: " << s->PriorityStep << "\n";
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
					stack->o->ief.ObjectArrived(stack->o->GetCoord());
				}
			}
			else if (s->active == F_TurnLeft)
			{
				if (stack->o->RotationLeft())
				{
					s->active = 0;
					stack->o->ief.ObjectArrived(stack->o->GetCoord());
				}
			}
			else if (s->active == F_TurnRight)
			{
				if (stack->o->RotationRight())
				{
					s->active = 0;
					stack->o->ief.ObjectArrived(stack->o->GetCoord());
				}
			}
			stack->o->requests.timer = true;
		}
		void Tick(OBJECT_TICK_PARAM)
		{
			pops(Specific, s);
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
						stack->o->ief.ObjectMove(stack->o->GetCoord(), to, 0);
						regets(Specific, s);
						stack->o->SetMove(stack->o->GetRotation());
						s->PriorityStep = false;
					}
					else if (stack->o->ief.EnableUpdateSkip())
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
							stack->o->ief.ObjectMove(stack->o->GetCoord(), to, 0);
							regets(Specific, s);
							stack->o->SetMove(stack->o->GetRotation());
							s->PriorityStep = false;
						}
						else if (stack->o->ief.EnableUpdateSkip())
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
				!o->ief.IsObjectOut(o->GetCoordUp())
				||
				!o->ief.IsObjectOut({o->GetCoord().x,o->GetCoord().y - 2})
				||
				o->ief.GetGoto({o->GetCoord().x, o->GetCoord().y - 2}).x != coord1.x
				;
		}
		bool CanRoll(ObjectBase *o, Type::Coord coordNeighbor, Type::Coord coordDiagonal)
		{
			return
				(
					(
						o->ief.GetObject(coordNeighbor)->GetFlags() & ObjectBase::StepOn//a mellette l�v� object-re r� tud l�pni
						&&
						o->ief.GetRemain(coordNeighbor)->GetFlags() & ObjectBase::StepOn//a mellette l�v� remain-re r� tud l�pni
						&&
						(
							!o->ief.IsObjectOut(coordNeighbor)
							||
							(
								o->ief.GetObjectOut(coordNeighbor)->GetFlags() & ObjectBase::StepOn//a mellette l�v� kimen� object-ra r� tud l�pni
								||
								o->ief.GetObjectOut(coordNeighbor)->GetCoord().x != coordNeighbor.x//a mellette l�v� kimen� object horizont�lisan t�vozik
								)
							)
						&&
						(
							o->ief.GetObject(coordDiagonal)->GetFlags() & ObjectBase::StepOn//�tl�s object-re r� tud l�pni
							&&
							o->ief.GetRemain(coordDiagonal)->GetFlags() & ObjectBase::StepOn//�tl�s remain-re r� tud l�pni
							&&
							(
								!o->ief.IsObjectOut(coordDiagonal)
								||
								(
									o->ief.GetObjectOut(coordDiagonal)->GetFlags() & ObjectBase::StepOn//�tl�s kimen� object-ra r� tud l�pni
									||
									o->ief.GetObjectOut(coordDiagonal)->IsMoving()//�tl�s kimen� object t�vozik
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
				o->ief.GetSectionFlags(o->GetCoordDown()) & ObjectBase::RollOffTop;
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
		void Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			clog << "ObjectRollDown::active: " << s->active << "\n";
			clog << "ObjectRollDown::lastRoll: " << s->active << "\n";
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
		void Tick(OBJECT_TICK_PARAM)
		{
			pops(Specific, s);
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
						stack->o->ief.ObjectMove(stack->o->GetCoord(), stack->o->GetCoordLeft(), 0);
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
						stack->o->ief.ObjectMove(stack->o->GetCoord(), stack->o->GetCoordRight(), 0);
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
					else if (stack->o->ief.EnableUpdateSkip())
					{
						stack->o->requests.update = true;
					}
					else if (s->rollCounter <= 0 && left)
					{
						s->active = 1;
						stack->o->ief.ObjectMove(stack->o->GetCoord(), stack->o->GetCoordLeft(), 0);
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