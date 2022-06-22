#pragma once

#include "Murphy.h"
#include "Global.h"
#include "OriginalObjects.h"

#include <KIR/KIR4_console.h>

namespace Object
{
	namespace Murphy
	{
		const char *name = "*** - Murphy";

		KIR5::SubBitmap Base;
		Slides MovingSlides[4];
		Slides PassInSlides[4];
		Slides PassOutSlides[4];
		Slides Push;
		Slides Sniff;

		const float PutUnityWaitTime = 0.8f;
		const float PushEffectWaitTime = 0.15f;
		const float PushEffectRestTime = 0.15f;
		const float SniffEffectTime = 0.15f;
		const float moveSpeed = 4.401544f;

		constexpr float passageSpeed = 4.401544f;
		constexpr float passageTime = 1.f / passageSpeed;

		enum Flag:Type::Flags
		{
			F_None = 1 << 0,
			F_Move = 1 << 1,
			F_Passage = 1 << 2,
			F_PassageDisappear = 1 << 3 | F_Passage,
			F_Sniff = 1 << 4,
			F_Push = 1 << 5,
			F_PushTry = 1 << 6 | F_Push,
			F_PushRest = 1 << 7 | F_Push,
			F_PutUnityU = 1 << 8 | F_None,
			F_PutUnityR = 1 << 9 | F_None,
			F_PutUnityD = 1 << 10 | F_None,
			F_PutUnityL = 1 << 11 | F_None,
		};

		struct Specific
		{
			bool Spell;
			bool MoveUp;
			bool MoveDown;
			bool MoveLeft;
			bool MoveRight;

			float PutUnityWaitTimer;
			float EffectTimer;

			int DrawNum;

			Type::Flags flag;

			float passageTimer;
		};



		void ObjectArrived(ObjectBase *o)
		{
			maks(o);
			gets(Specific, s);

			if (s->flag == F_Push)
			{
				o->ief.GetObjectOut(o->GetCoord())->SetMove({0,0});
				o->ief.GetObjectOut(o->GetCoord())->Arrived();
				o->DisableLimitSpeed();
				o->Arrived();
				s->flag = F_PushRest;
				s->EffectTimer = PushEffectRestTime;
			}
			else
				s->flag = F_None;
		}

		void EffectTimerTick(ObjectBase *o)
		{
			maks(o);
			gets(Specific, s);
			s->EffectTimer -= 1 / CPS;
		}

		bool CanSniff(ObjectBase *o, Type::Coord coord)
		{
			return
				!o->GetObject(coord)->IsMoving()
				&&
				o->GetObject(coord)->GetFlags() & ObjectBase::MurphyCanSniff
				&&
				o->GetRemain(coord)->GetFlags() & ObjectBase::StepOn
				;
		}

		void Eat(ObjectBase *o, ObjectBase *object)
		{
			if (object->GetFlags() & ObjectBase::MurphyDies)
			{
				o->blowUp(o->GetCoord());
			}
			if (object->GetFlags() & ObjectBase::Give1Aim)
			{
				o->ief.AddAim(1);
				if (o->ief.GetAimRemaining() == 0)
				{
					Exit_015::Open(o);
				}
			}
			if (object->GetFlags() & ObjectBase::Give1Unity)
			{
				o->ief.AddUnity(1);
			}
		}

		void SetSpell(ObjectBase *o, bool Spell)
		{
			maks(o);
			gets(Specific, s);
			s->Spell = Spell;
		}
		void SetMoveUp(ObjectBase *o, bool MoveUp)
		{
			maks(o);
			gets(Specific, s);
			s->MoveUp = MoveUp;
		}
		void SetMoveDown(ObjectBase *o, bool MoveDown)
		{
			maks(o);
			gets(Specific, s);
			s->MoveDown = MoveDown;
		}
		void SetMoveLeft(ObjectBase *o, bool MoveLeft)
		{
			maks(o);
			gets(Specific, s);
			s->MoveLeft = MoveLeft;
		}
		void SetMoveRight(ObjectBase *o, bool MoveRight)
		{
			maks(o);
			gets(Specific, s);
			s->MoveRight = MoveRight;
		}

		bool MurphyCanMovePos(ObjectBase *o, Type::Coord to, Type::Rotation rotation)
		{
			return
				!o->GetObject(to)->IsMoving()
				&&
				o->GetObject(to)->GetFlags() & (ObjectBase::MurphyStepOn | ObjectBase::StepOn)
				&&
				o->GetRemain(to)->GetFlags() & (ObjectBase::MurphyStepOn | ObjectBase::StepOn)
				&&
				(
					o->GetObjectOut(to)->GetFlags() & ObjectBase::StepOn
					||
					(
						(rotation == Type::Rotations::Up && o->GetObjectOut(to)->GetCoord().y < to.y)
						||
						(rotation == Type::Rotations::Down && o->GetObjectOut(to)->GetCoord().y > to.y)
						||
						(rotation == Type::Rotations::Left && o->GetObjectOut(to)->GetCoord().x < to.x)
						||
						(rotation == Type::Rotations::Right && o->GetObjectOut(to)->GetCoord().x > to.x)
						)
					);
		}
		bool Move(ObjectBase::Stack *stack, Type::Coord to, Type::Coord to2, Type::Coord to2i, Type::Rotation rotation, Type::Flags passage, Type::Flags push)
		{
			gets(Specific, s);
			if (MurphyCanMovePos(stack->o, to, rotation))
			{
				s->flag = F_Move;
				Eat(stack->o, stack->o->GetObject(to));
				stack->o->ief.ObjectMove(stack->o->GetCoord(), to, ObjectID::Space);
				stack->o->ief.murphyMoved(stack->o);
				regets(Specific, s);
				stack->o->SetMove(rotation);
				stack->o->SetRotation(rotation);
				return true;
			}
			else if (stack->o->GetObject(to)->id == ObjectID::Exit && stack->o->ief.GetAimRemaining() == 0)
			{
				s->DrawNum = 0;
				s->passageTimer = passageTime;
				stack->o->SetRotation(stack->o->GetRoundRotation(stack->o->GetRealRotation(rotation + Type::Rotations::_180)));
				s->flag = F_PassageDisappear;
				stack->o->requests.draw = true;
				stack->o->ief.murphyVictory();
			}
			else
			{
				ObjectBase *toObject = stack->o->GetObject(to);
				if (toObject->GetFlags() & passage && stack->o->ief.GetSectionFlags(to2) & ObjectBase::StepOn)
				{
					Type::Coord
						source = stack->o->GetCoord();
					s->DrawNum = PassOutSlides[Type::Rotations::getRotationIndex(rotation)].getCount() - 1;
					s->flag = F_Passage;
					Eat(stack->o, stack->o->GetObject(to2));
					stack->o->ief.ObjectMove(source, to2, ObjectID::MurphyPlus);
					stack->o->ief.murphyMoved(stack->o);
					regets(Specific, s);
					stack->o->SetRotation(rotation);
					stack->o->SetMoveUnsafe(rotation, {2,2});
					stack->o->ief.GetObject(source)->SetRotation(stack->o->GetRoundRotation(stack->o->GetRealRotation(rotation + Type::Rotations::_180)));
					s->passageTimer = passageTime;
					return true;
				}
				else if (!toObject->IsMoving() && toObject->GetFlags() & push && toObject->CanMovePos(to2, rotation))
				{
					if (s->flag != F_PushTry || stack->o->GetRotation() != rotation)
					{
						s->flag = F_PushTry;
						s->EffectTimer = PushEffectWaitTime;
						stack->o->requests.draw = true;
						stack->o->SetRotation(rotation);
					}
					else if (s->flag == F_PushTry)
					{
						EffectTimerTick(stack->o);
						if (s->EffectTimer <= 0)
						{
							s->flag = F_Push;

							toObject->SetRotation(rotation);
							stack->o->SetRotation(rotation);

							toObject->SetMove(rotation);
							stack->o->SetMove(rotation);

							if (rotation == Type::Rotations::Left || rotation == Type::Rotations::Right)
								stack->o->EnebleLimitSpeed(stack->o->GetMoveSpeed().x / (Type::Move::Type)(1.5));
							else
								stack->o->EnebleLimitSpeed(stack->o->GetMoveSpeed().y / (Type::Move::Type)(1.5));

							stack->o->ief.ObjectMove(to, to2, ObjectID::Space);
							stack->o->ief.ObjectMove(stack->o->GetCoord(), to, stack->o->GetObjectIDremain());
							stack->o->ief.murphyMoved(stack->o);
							regets(Specific, s);
						}
					}
					return true;
				}
			}
			return false;
		}
		void Controll(ObjectBase::Stack *stack)
		{
			gets(Specific, s);
			if (s->flag == F_Move)
			{
				stack->o->Step();
				if (!stack->o->IsMove())
					ObjectArrived(stack->o);

				float position = 0.f;

				if (stack->o->IsMoveLeft())
				{
					position = 1 - stack->o->GetMove().x;
				}
				else if (stack->o->IsMoveRight())
				{
					position = -stack->o->GetMove().x;
				}
				else if (stack->o->IsMoveUp())
				{
					position = 1 - stack->o->GetMove().y;
				}
				else if (stack->o->IsMoveDown())
				{
					position = -stack->o->GetMove().y;
				}

				int DrawNum = MovingSlides[Type::Rotations::getRotationIndex(stack->o->GetRotation())].getDrawNumber(position);

				if (s->DrawNum != DrawNum)
				{
					stack->o->requests.draw = true;
					s->DrawNum = DrawNum;
				}
			}
			else if (s->flag == F_Passage)
			{
				s->passageTimer -= CA;
				if (s->passageTimer <= 0)
				{
					stack->o->SetMoveUnsafe({0,0});
					stack->o->ief.ObjectArrived(stack->o->GetCoord());
					s->flag = F_None;
				}
				else
				{
					Type::Move::Type move = s->passageTimer / passageTime * 2;
					stack->o->SetMoveUnsafe(stack->o->GetRotation(), {move,move});

					int DrawNum = PassOutSlides[Type::Rotations::getRotationIndex(stack->o->GetRotation())].getDrawNumber(s->passageTimer / passageTime);

					if (s->DrawNum != DrawNum)
					{
						stack->o->requests.draw = true;
						s->DrawNum = DrawNum;
					}
				}
			}
			else if (s->flag == F_Push)
			{
				stack->o->GetObjectOut(stack->o->GetCoord())->SetMove({0,0});
				stack->o->Step();
				if (!stack->o->IsMove())
					ObjectArrived(stack->o);
				stack->o->GetObjectOut(stack->o->GetCoord())->SetMove(stack->o->GetMove());
			}
			if (!stack->o->IsMoving())
			{
				if (s->flag == F_Sniff)
				{
					EffectTimerTick(stack->o);
					if (s->EffectTimer <= 0)
					{
						s->flag = F_None;
						stack->o->requests.draw = true;
					}
				}
				else if (s->flag == F_PushRest)
				{
					EffectTimerTick(stack->o);
					if (s->EffectTimer <= 0)
					{
						s->flag = F_None;
						stack->o->requests.draw = true;
					}
				}

				bool
					FallDown = false;
				if (stack->o->ief.GetBlockFlags(stack->o->GetCoord()) & GridFlags::Gravity && stack->o->CanMoveDown() && stack->o->ief.GetObjectOut(stack->o->GetCoordDown())->GetAbsMove() <= 0.5f)
					FallDown = true;

				if (s->Spell && !FallDown)
				{
					if (s->flag != F_Sniff)
					{
						if (s->MoveUp && CanSniff(stack->o, stack->o->GetCoordUp()))
						{
							Eat(stack->o, stack->o->GetObject(stack->o->GetCoordUp()));
							stack->o->ief.ObjectDisappear(stack->o->GetCoordUp());
							stack->o->SetRotation(Type::Rotations::Up);
							s->EffectTimer = SniffEffectTime;
							s->flag = F_Sniff;
							stack->o->requests.draw = true;
						}
						else if (s->MoveDown && CanSniff(stack->o, stack->o->GetCoordDown()))
						{
							Eat(stack->o, stack->o->GetObject(stack->o->GetCoordDown()));
							stack->o->ief.ObjectDisappear(stack->o->GetCoordDown());
							stack->o->SetRotation(Type::Rotations::Down);
							s->EffectTimer = SniffEffectTime;
							s->flag = F_Sniff;
							stack->o->requests.draw = true;
						}
						else if (s->MoveLeft && CanSniff(stack->o, stack->o->GetCoordLeft()))
						{
							Eat(stack->o, stack->o->GetObject(stack->o->GetCoordLeft()));
							stack->o->ief.ObjectDisappear(stack->o->GetCoordLeft());
							stack->o->SetRotation(Type::Rotations::Left);
							s->EffectTimer = SniffEffectTime;
							s->flag = F_Sniff;
							stack->o->requests.draw = true;
						}
						else if (s->MoveRight && CanSniff(stack->o, stack->o->GetCoordRight()))
						{
							Eat(stack->o, stack->o->GetObject(stack->o->GetCoordRight()));
							stack->o->ief.ObjectDisappear(stack->o->GetCoordRight());
							stack->o->SetRotation(Type::Rotations::Right);
							s->EffectTimer = SniffEffectTime;
							s->flag = F_Sniff;
							stack->o->requests.draw = true;
						}
						else if (stack->o->ief.GetUnityCount() > 0)
						{
							if (s->MoveUp && stack->o->ief.GetObject(stack->o->GetCoordUp())->GetFlags() & ObjectBase::StepOn && stack->o->ief.GetGoto(stack->o->GetCoordUp()) == stack->o->GetCoordUp())
							{
								if (s->flag != F_PutUnityU)
								{
									s->flag = F_PutUnityU;
									s->PutUnityWaitTimer = PutUnityWaitTime;
								}
								else
								{
									s->PutUnityWaitTimer -= 1 / CPS;
									if (s->PutUnityWaitTimer <= 0)
									{
										stack->o->ief.AddUnity(-1);
										stack->o->ief.ObjectPut(stack->o->GetCoordUp(), ObjectID::Utility2);
										if (stack->o->GetObject(stack->o->GetCoordUp())->id == ObjectID::Utility2)
											Utility2_030::Activate(stack->o->GetObject(stack->o->GetCoordUp()));

										stack->o->SetRotation(Type::Rotations::Up);
										s->EffectTimer = SniffEffectTime;
										s->flag = F_Sniff;
										stack->o->requests.draw = true;
									}
								}
							}
							else if (s->MoveDown && stack->o->ief.GetObject(stack->o->GetCoordDown())->GetFlags() & ObjectBase::StepOn && stack->o->ief.GetGoto(stack->o->GetCoordDown()) == stack->o->GetCoordDown())
							{
								if (s->flag != F_PutUnityD)
								{
									s->flag = F_PutUnityD;
									s->PutUnityWaitTimer = PutUnityWaitTime;
								}
								else
								{
									s->PutUnityWaitTimer -= 1 / CPS;
									if (s->PutUnityWaitTimer <= 0)
									{
										stack->o->ief.AddUnity(-1);
										stack->o->ief.ObjectPut(stack->o->GetCoordDown(), ObjectID::Utility2);
										if (stack->o->GetObject(stack->o->GetCoordDown())->id == ObjectID::Utility2)
											Utility2_030::Activate(stack->o->GetObject(stack->o->GetCoordDown()));

										stack->o->SetRotation(Type::Rotations::Down);
										s->EffectTimer = SniffEffectTime;
										s->flag = F_Sniff;
										stack->o->requests.draw = true;
									}
								}
							}
							else if (s->MoveLeft && stack->o->ief.GetObject(stack->o->GetCoordLeft())->GetFlags() & ObjectBase::StepOn && stack->o->ief.GetGoto(stack->o->GetCoordLeft()) == stack->o->GetCoordLeft())
							{
								if (s->flag != F_PutUnityL)
								{
									s->flag = F_PutUnityL;
									s->PutUnityWaitTimer = PutUnityWaitTime;
								}
								else
								{
									s->PutUnityWaitTimer -= 1 / CPS;
									if (s->PutUnityWaitTimer <= 0)
									{
										stack->o->ief.AddUnity(-1);
										stack->o->ief.ObjectPut(stack->o->GetCoordLeft(), ObjectID::Utility2);
										if (stack->o->GetObject(stack->o->GetCoordLeft())->id == ObjectID::Utility2)
											Utility2_030::Activate(stack->o->GetObject(stack->o->GetCoordLeft()));

										stack->o->SetRotation(Type::Rotations::Left);
										s->EffectTimer = SniffEffectTime;
										s->flag = F_Sniff;
										stack->o->requests.draw = true;
									}
								}
							}
							else if (s->MoveRight && stack->o->ief.GetObject(stack->o->GetCoordRight())->GetFlags() & ObjectBase::StepOn && stack->o->ief.GetGoto(stack->o->GetCoordRight()) == stack->o->GetCoordRight())
							{
								if (s->flag != F_PutUnityR)
								{
									s->flag = F_PutUnityR;
									s->PutUnityWaitTimer = PutUnityWaitTime;
								}
								else
								{
									s->PutUnityWaitTimer -= 1 / CPS;
									if (s->PutUnityWaitTimer <= 0)
									{
										stack->o->ief.AddUnity(-1);
										stack->o->ief.ObjectPut(stack->o->GetCoordRight(), ObjectID::Utility2);
										if (stack->o->GetObject(stack->o->GetCoordRight())->id == ObjectID::Utility2)
											Utility2_030::Activate(stack->o->GetObject(stack->o->GetCoordRight()));

										stack->o->SetRotation(Type::Rotations::Right);
										s->EffectTimer = SniffEffectTime;
										s->flag = F_Sniff;
										stack->o->requests.draw = true;
									}
								}
							}
							else if (
								s->flag == F_PutUnityU ||
								s->flag == F_PutUnityR ||
								s->flag == F_PutUnityD ||
								s->flag == F_PutUnityL)
								s->flag = F_None;
						}
					}
				}
				else
				{
					if (s->MoveUp && (!FallDown || stack->o->GetObject(stack->o->GetCoordUp())->GetFlags() & ObjectBase::GiveGravityDelay))
					{
						if (Move(stack, stack->o->GetCoordUp(), {stack->o->GetCoord().x,stack->o->GetCoord().y - 2}, {stack->o->GetCoord().x,stack->o->GetCoord().y + 2}, Type::Rotations::Up, ObjectBase::PassageFromBottom, ObjectBase::CanPushUp))
							return;
					}
					if (s->MoveDown && (!FallDown || stack->o->GetObject(stack->o->GetCoordDown())->GetFlags() & ObjectBase::GiveGravityDelay))
					{
						if (Move(stack, stack->o->GetCoordDown(), {stack->o->GetCoord().x,stack->o->GetCoord().y + 2}, {stack->o->GetCoord().x,stack->o->GetCoord().y - 2}, Type::Rotations::Down, ObjectBase::PassageFromTop, ObjectBase::CanPushDown))
							return;
					}
					if (s->MoveLeft && (!FallDown || stack->o->GetObject(stack->o->GetCoordLeft())->GetFlags() & ObjectBase::GiveGravityDelay))
					{
						if (Move(stack, stack->o->GetCoordLeft(), {stack->o->GetCoord().x - 2,stack->o->GetCoord().y}, {stack->o->GetCoord().x + 2,stack->o->GetCoord().y}, Type::Rotations::Left, ObjectBase::PassageFromRight, ObjectBase::CanPushLeft))
							return;
					}
					if (s->MoveRight && (!FallDown || stack->o->GetObject(stack->o->GetCoordRight())->GetFlags() & ObjectBase::GiveGravityDelay))
					{
						if (Move(stack, stack->o->GetCoordRight(), {stack->o->GetCoord().x + 2,stack->o->GetCoord().y}, {stack->o->GetCoord().x - 2,stack->o->GetCoord().y}, Type::Rotations::Right, ObjectBase::PassageFromLeft, ObjectBase::CanPushRight))
							return;
					}

					if (FallDown)
					{
						s->flag = F_Move;
						Eat(stack->o, stack->o->GetObject(stack->o->GetCoordDown()));
						stack->o->ief.ObjectMove(stack->o->GetCoord(), stack->o->GetCoordDown(), ObjectID::Space);
						stack->o->ief.murphyMoved(stack->o);
						regets(Specific, s);
						stack->o->SetMove({stack->o->GetMove().x,-1});
						//o->SetRotation(rotation);
						return;
					}

					if (s->flag != F_Sniff)
					{
						if (s->MoveUp && stack->o->GetObject(stack->o->GetCoordUp())->GetFlags() & ObjectBase::ButtonPush)
						{
							if (stack->o->GetObject(stack->o->GetCoordUp())->id == ObjectID::Terminal)
								Terminal_028::Pushed(stack->o->GetObject(stack->o->GetCoordUp()));
							stack->o->SetRotation(Type::Rotations::Up);
							s->EffectTimer = SniffEffectTime;
							s->flag = F_Sniff;
							stack->o->requests.draw = true;
							return;
						}
						if (s->MoveDown && stack->o->GetObject(stack->o->GetCoordDown())->GetFlags() & ObjectBase::ButtonPush)
						{
							if (stack->o->GetObject(stack->o->GetCoordDown())->id == ObjectID::Terminal)
								Terminal_028::Pushed(stack->o->GetObject(stack->o->GetCoordDown()));
							stack->o->SetRotation(Type::Rotations::Down);
							s->EffectTimer = SniffEffectTime;
							s->flag = F_Sniff;
							stack->o->requests.draw = true;
							return;
						}
						if (s->MoveLeft && stack->o->GetObject(stack->o->GetCoordLeft())->GetFlags() & ObjectBase::ButtonPush)
						{
							if (stack->o->GetObject(stack->o->GetCoordLeft())->id == ObjectID::Terminal)
								Terminal_028::Pushed(stack->o->GetObject(stack->o->GetCoordLeft()));
							stack->o->SetRotation(Type::Rotations::Left);
							s->EffectTimer = SniffEffectTime;
							s->flag = F_Sniff;
							stack->o->requests.draw = true;
							return;
						}
						if (s->MoveRight && stack->o->GetObject(stack->o->GetCoordRight())->GetFlags() & ObjectBase::ButtonPush)
						{
							if (stack->o->GetObject(stack->o->GetCoordRight())->id == ObjectID::Terminal)
								Terminal_028::Pushed(stack->o->GetObject(stack->o->GetCoordRight()));
							stack->o->SetRotation(Type::Rotations::Right);
							s->EffectTimer = SniffEffectTime;
							s->flag = F_Sniff;
							stack->o->requests.draw = true;
							return;
						}
					}
				}
				if (s->flag == F_PushTry)
				{
					s->flag = F_PushRest;
					s->EffectTimer = PushEffectRestTime;
				}
			}
		}

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Base = ObjectBase::bitmapPool.get("Pleyer-Base"), ObjectBase::bitmapPool.get("Error");

			MovingSlides[Type::Rotations::getRotationIndex(Type::Rotations::Right)].initialize(ObjectBase::bitmapPool.get("Pleyer-MovingRight"), ObjectBase::bitmapPool.get("Error"));
			MovingSlides[Type::Rotations::getRotationIndex(Type::Rotations::Up)].initialize(ObjectBase::bitmapPool.get("Pleyer-MovingUp"), ObjectBase::bitmapPool.get("Error"));
			MovingSlides[Type::Rotations::getRotationIndex(Type::Rotations::Down)].initialize(ObjectBase::bitmapPool.get("Pleyer-MovingDown"), ObjectBase::bitmapPool.get("Error"));
			MovingSlides[Type::Rotations::getRotationIndex(Type::Rotations::Left)].initialize(ObjectBase::bitmapPool.get("Pleyer-MovingLeft"), ObjectBase::bitmapPool.get("Error"));

			PassInSlides[Type::Rotations::getRotationIndex(Type::Rotations::Right)].initialize(ObjectBase::bitmapPool.get("Pleyer-PassInRight"), ObjectBase::bitmapPool.get("Error"));
			PassInSlides[Type::Rotations::getRotationIndex(Type::Rotations::Up)].initialize(ObjectBase::bitmapPool.get("Pleyer-PassInUp"), ObjectBase::bitmapPool.get("Error"));
			PassInSlides[Type::Rotations::getRotationIndex(Type::Rotations::Down)].initialize(ObjectBase::bitmapPool.get("Pleyer-PassInDown"), ObjectBase::bitmapPool.get("Error"));
			PassInSlides[Type::Rotations::getRotationIndex(Type::Rotations::Left)].initialize(ObjectBase::bitmapPool.get("Pleyer-PassInLeft"), ObjectBase::bitmapPool.get("Error"));

			PassOutSlides[Type::Rotations::getRotationIndex(Type::Rotations::Right)].initialize(ObjectBase::bitmapPool.get("Pleyer-PassOutRight"), ObjectBase::bitmapPool.get("Error"));
			PassOutSlides[Type::Rotations::getRotationIndex(Type::Rotations::Up)].initialize(ObjectBase::bitmapPool.get("Pleyer-PassOutUp"), ObjectBase::bitmapPool.get("Error"));
			PassOutSlides[Type::Rotations::getRotationIndex(Type::Rotations::Down)].initialize(ObjectBase::bitmapPool.get("Pleyer-PassOutDown"), ObjectBase::bitmapPool.get("Error"));
			PassOutSlides[Type::Rotations::getRotationIndex(Type::Rotations::Left)].initialize(ObjectBase::bitmapPool.get("Pleyer-PassOutLeft"), ObjectBase::bitmapPool.get("Error"));

			Sniff.initialize(ObjectBase::bitmapPool.get("Pleyer-Sniff"), ObjectBase::bitmapPool.get("Error"));
			Push.initialize(ObjectBase::bitmapPool.get("Pleyer-Push"), ObjectBase::bitmapPool.get("Error"));
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			gets(Specific, s);
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::CanBeKilled | ObjectBase::ExplosionType3);
			stack->o->DisablePhysics();
			stack->o->SetMoveSpeed({moveSpeed,moveSpeed});

			stack->o->events.timer = true;

			stack->o->SetTranslationID(ObjectID::Infotron);
			stack->o->SetObjectIDremain(ObjectID::Space);

			s->Spell = false;
			s->MoveUp = false;
			s->MoveDown = false;
			s->MoveLeft = false;
			s->MoveRight = false;

			s->DrawNum = 0;

			s->flag = F_None;
		}
		void CreatePassage(OBJECT_CREATER_PARAM)
		{
			Create(OBJECT_CREATER_CALL);

			stack->o->SetTranslationID(ObjectID::Space);

			gets(Specific, s);

			s->passageTimer = passageTime;
			s->flag = F_PassageDisappear;

			stack->o->events.topDraw = true;
			stack->o->requests.timer = true;
		}

		void Print(OBJECT_PRINTER_PARAM)
		{
			gets(Specific, s);
			clog << "(KEY)Spell Is Active: " << s->Spell << "\n";
			clog << "(KEY)MoveUp Is Active: " << s->MoveUp << "\n";
			clog << "(KEY)MoveDown Is Active: " << s->MoveDown << "\n";
			clog << "(KEY)MoveLeft Is Active: " << s->MoveLeft << "\n";
			clog << "(KEY)MoveRight Is Active: " << s->MoveRight << "\n";
			clog << "Flag: " << s->flag << "\n";
			clog << "s->EffectTimer: " << s->EffectTimer << "\n";
			clog << "passageTimer: " << s->passageTimer << "\n";
			clog << "PutUnityWaitTimer: " << s->PutUnityWaitTimer << "\n";
			clog << "(BITMAP)Base Is Loaded: " << Base << "\n";

			clog << "(BITMAP)Push Is Loaded: " << Push.getCount() << "\n";
			clog << "(BITMAP)Sniff Is Loaded: " << Sniff.getCount() << "\n";
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			gets(Specific, s);
			if (s->flag == F_PassageDisappear)
			{
				s->passageTimer -= CA;
				if (s->passageTimer <= 0)
				{
					stack->o->ief.ObjectDisappear(stack->o->GetCoord());
					stack->o->events.timer = true;
				}
				else
				{
					stack->o->requests.timer = true;
				}
				int DrawNum = PassInSlides[Type::Rotations::getRotationIndex(stack->o->GetRotation())].getDrawNumber(1 - (s->passageTimer / passageTime));

				if (s->DrawNum != DrawNum)
				{
					stack->o->requests.draw = true;
					s->DrawNum = DrawNum;
				}
			}
		}
		void Tick(OBJECT_TICK_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			gets(Specific, s);
			if (s->flag & F_None)
			{
				Base.drawScaled(x, y, w, h);
			}
			else if (s->flag & F_Move)
			{
				MovingSlides[Type::Rotations::getRotationIndex(stack->o->GetRotation())][s->DrawNum].drawScaled(x, y, w, h);

				if (stack->o->GetRotation() != Type::Rotations::Up &&
					stack->o->GetRotation() != Type::Rotations::Down &&
					stack->o->GetRotation() != Type::Rotations::Right &&
					stack->o->GetRotation() != Type::Rotations::Left)
					clog << KIR4::LRED << "Murphy Draw ERROR because F_Move draw flag but options not valid" << KIR4::eol;
			}
			else if (s->flag & F_Passage)
			{
				if (s->flag == F_PassageDisappear)
				{
					PassInSlides[Type::Rotations::getRotationIndex(stack->o->GetRotation())][s->DrawNum].drawScaled(x, y, w, h);

					if (stack->o->GetRotation() != Type::Rotations::Up &&
						stack->o->GetRotation() != Type::Rotations::Down &&
						stack->o->GetRotation() != Type::Rotations::Right &&
						stack->o->GetRotation() != Type::Rotations::Left)
						clog << KIR4::LRED << "Murphy Draw ERROR because F_PassageDisappear draw flag but options not valid" << KIR4::eol;
				}
				else
				{
					PassOutSlides[Type::Rotations::getRotationIndex(stack->o->GetRotation())][s->DrawNum].drawScaled(x, y, w, h);

					if (stack->o->GetRotation() != Type::Rotations::Up &&
						stack->o->GetRotation() != Type::Rotations::Down &&
						stack->o->GetRotation() != Type::Rotations::Right &&
						stack->o->GetRotation() != Type::Rotations::Left)
						clog << KIR4::LRED << "Murphy Draw ERROR because F_Passage draw flag but options not valid" << KIR4::eol;
				}
			}
			else if (s->flag & F_Sniff)
			{
				Sniff[Type::Rotations::getRotationIndex(stack->o->GetRotation())].drawScaled(x, y, w, h);
			}
			else if (s->flag & F_Push)
			{
				Push[Type::Rotations::getRotationIndex(stack->o->GetRotation())].drawScaled(x, y, w, h);
			}
			else
			{
				clog << KIR4::LRED << "Murphy Draw ERROR because flag not valid" << KIR4::eol;
			}
		}


		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Base.drawScaled(x, y, w, h);
		}
	}
}