#pragma once

#include "Murphy.h"
#include "Tools.h"
#include "OriginalObjects.h"

#include <KIR/KIR4_console.h>

namespace Object
{
	namespace Murphy
	{
		const char *name = "*** - Murphy";

		KIR5::SubBitmap Base;
		Res::Slides MovingSlides[4];
		Res::Slides PassInSlides[4];
		Res::Slides PassOutSlides[4];
		Res::Slides Push;
		Res::Slides Sniff;

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
			F_Destroy = 1 << 12,
		};

		struct Specific
		{
			KeyboardController *controller;

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

		void SetController(ObjectBase *o, KeyboardController *controller)
		{
			maks(o);
			gets(Specific, s);
			s->controller = controller;
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
					s->DrawNum = PassOutSlides[Type::Rotations::getIndexOfRotation(rotation)].getCount() - 1;
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
		void Controll(OBJECT_TIMER_PARAM)
		{
			gets(Specific, s);
			if (s->flag != F_Destroy)
			{
				if (s->controller->actionDestroy)
				{
					stack->o->blowUp(stack->o->GetCoord());
					s->flag = F_Destroy;
				}
				else if (s->flag == F_Move)
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

					int DrawNum = MovingSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())].getDrawNumber(position);

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

						int DrawNum = PassOutSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())].getDrawNumber(s->passageTimer / passageTime);

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

					bool forceFallDown = false;
					if (
						(stack->o->ief.IsGlobalGravity() || stack->o->ief.GetBlockFlags(stack->o->GetCoord()) & GridFlags::Gravity) &&
						stack->o->CanMoveDown() && stack->o->ief.GetObjectOut(stack->o->GetCoordDown())->GetAbsMove() <= 0.5f)
					{
						forceFallDown = true;
					}

					if (s->controller->actionSpecial && !forceFallDown)
					{
						if (s->flag != F_Sniff)
						{
							if (s->controller->actionUp && CanSniff(stack->o, stack->o->GetCoordUp()))
							{
								Eat(stack->o, stack->o->GetObject(stack->o->GetCoordUp()));
								stack->o->ief.ObjectDisappear(stack->o->GetCoordUp());
								stack->o->SetRotation(Type::Rotations::Up);
								s->EffectTimer = SniffEffectTime;
								s->flag = F_Sniff;
								stack->o->requests.draw = true;
							}
							else if (s->controller->actionDown && CanSniff(stack->o, stack->o->GetCoordDown()))
							{
								Eat(stack->o, stack->o->GetObject(stack->o->GetCoordDown()));
								stack->o->ief.ObjectDisappear(stack->o->GetCoordDown());
								stack->o->SetRotation(Type::Rotations::Down);
								s->EffectTimer = SniffEffectTime;
								s->flag = F_Sniff;
								stack->o->requests.draw = true;
							}
							else if (s->controller->actionLeft && CanSniff(stack->o, stack->o->GetCoordLeft()))
							{
								Eat(stack->o, stack->o->GetObject(stack->o->GetCoordLeft()));
								stack->o->ief.ObjectDisappear(stack->o->GetCoordLeft());
								stack->o->SetRotation(Type::Rotations::Left);
								s->EffectTimer = SniffEffectTime;
								s->flag = F_Sniff;
								stack->o->requests.draw = true;
							}
							else if (s->controller->actionRight && CanSniff(stack->o, stack->o->GetCoordRight()))
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
								if (s->controller->actionUp && stack->o->ief.GetObject(stack->o->GetCoordUp())->GetFlags() & ObjectBase::StepOn && stack->o->ief.GetGoto(stack->o->GetCoordUp()) == stack->o->GetCoordUp())
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
								else if (s->controller->actionDown && stack->o->ief.GetObject(stack->o->GetCoordDown())->GetFlags() & ObjectBase::StepOn && stack->o->ief.GetGoto(stack->o->GetCoordDown()) == stack->o->GetCoordDown())
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
								else if (s->controller->actionLeft && stack->o->ief.GetObject(stack->o->GetCoordLeft())->GetFlags() & ObjectBase::StepOn && stack->o->ief.GetGoto(stack->o->GetCoordLeft()) == stack->o->GetCoordLeft())
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
								else if (s->controller->actionRight && stack->o->ief.GetObject(stack->o->GetCoordRight())->GetFlags() & ObjectBase::StepOn && stack->o->ief.GetGoto(stack->o->GetCoordRight()) == stack->o->GetCoordRight())
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
						if (s->controller->actionUp && (!forceFallDown || stack->o->GetObject(stack->o->GetCoordUp())->GetFlags() & ObjectBase::GiveGravityDelay))
						{
							if (Move(stack, stack->o->GetCoordUp(), {stack->o->GetCoord().x,stack->o->GetCoord().y - 2}, {stack->o->GetCoord().x,stack->o->GetCoord().y + 2}, Type::Rotations::Up, ObjectBase::PassageFromBottom, ObjectBase::CanPushUp))
								return;
						}
						if (s->controller->actionDown && (!forceFallDown || stack->o->GetObject(stack->o->GetCoordDown())->GetFlags() & ObjectBase::GiveGravityDelay))
						{
							if (Move(stack, stack->o->GetCoordDown(), {stack->o->GetCoord().x,stack->o->GetCoord().y + 2}, {stack->o->GetCoord().x,stack->o->GetCoord().y - 2}, Type::Rotations::Down, ObjectBase::PassageFromTop, ObjectBase::CanPushDown))
								return;
						}
						if (s->controller->actionLeft && (!forceFallDown || stack->o->GetObject(stack->o->GetCoordLeft())->GetFlags() & ObjectBase::GiveGravityDelay))
						{
							if (Move(stack, stack->o->GetCoordLeft(), {stack->o->GetCoord().x - 2,stack->o->GetCoord().y}, {stack->o->GetCoord().x + 2,stack->o->GetCoord().y}, Type::Rotations::Left, ObjectBase::PassageFromRight, ObjectBase::CanPushLeft))
								return;
						}
						if (s->controller->actionRight && (!forceFallDown || stack->o->GetObject(stack->o->GetCoordRight())->GetFlags() & ObjectBase::GiveGravityDelay))
						{
							if (Move(stack, stack->o->GetCoordRight(), {stack->o->GetCoord().x + 2,stack->o->GetCoord().y}, {stack->o->GetCoord().x - 2,stack->o->GetCoord().y}, Type::Rotations::Right, ObjectBase::PassageFromLeft, ObjectBase::CanPushRight))
								return;
						}

						if (forceFallDown)
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
							if (s->controller->actionUp && stack->o->GetObject(stack->o->GetCoordUp())->GetFlags() & ObjectBase::ButtonPush)
							{
								if (stack->o->GetObject(stack->o->GetCoordUp())->id == ObjectID::Terminal)
									Terminal_028::Pushed(stack->o->GetObject(stack->o->GetCoordUp()));
								stack->o->SetRotation(Type::Rotations::Up);
								s->EffectTimer = SniffEffectTime;
								s->flag = F_Sniff;
								stack->o->requests.draw = true;
								return;
							}
							if (s->controller->actionDown && stack->o->GetObject(stack->o->GetCoordDown())->GetFlags() & ObjectBase::ButtonPush)
							{
								if (stack->o->GetObject(stack->o->GetCoordDown())->id == ObjectID::Terminal)
									Terminal_028::Pushed(stack->o->GetObject(stack->o->GetCoordDown()));
								stack->o->SetRotation(Type::Rotations::Down);
								s->EffectTimer = SniffEffectTime;
								s->flag = F_Sniff;
								stack->o->requests.draw = true;
								return;
							}
							if (s->controller->actionLeft && stack->o->GetObject(stack->o->GetCoordLeft())->GetFlags() & ObjectBase::ButtonPush)
							{
								if (stack->o->GetObject(stack->o->GetCoordLeft())->id == ObjectID::Terminal)
									Terminal_028::Pushed(stack->o->GetObject(stack->o->GetCoordLeft()));
								stack->o->SetRotation(Type::Rotations::Left);
								s->EffectTimer = SniffEffectTime;
								s->flag = F_Sniff;
								stack->o->requests.draw = true;
								return;
							}
							if (s->controller->actionRight && stack->o->GetObject(stack->o->GetCoordRight())->GetFlags() & ObjectBase::ButtonPush)
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
		}

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Base = Res::tiles[Res::Tiles::_Player_Base];

			MovingSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Right)].initialize(Res::tiles[Res::Tiles::_Player_MovingRight], Res::tiles[Res::BitmapPool::Fallback]);
			MovingSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Up)].initialize(Res::tiles[Res::Tiles::_Player_MovingUp], Res::tiles[Res::BitmapPool::Fallback]);
			MovingSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Down)].initialize(Res::tiles[Res::Tiles::_Player_MovingDown], Res::tiles[Res::BitmapPool::Fallback]);
			MovingSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Left)].initialize(Res::tiles[Res::Tiles::_Player_MovingLeft], Res::tiles[Res::BitmapPool::Fallback]);

			PassInSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Right)].initialize(Res::tiles[Res::Tiles::_Player_PassInRight], Res::tiles[Res::BitmapPool::Fallback]);
			PassInSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Up)].initialize(Res::tiles[Res::Tiles::_Player_PassInUp], Res::tiles[Res::BitmapPool::Fallback]);
			PassInSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Down)].initialize(Res::tiles[Res::Tiles::_Player_PassInDown], Res::tiles[Res::BitmapPool::Fallback]);
			PassInSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Left)].initialize(Res::tiles[Res::Tiles::_Player_PassInLeft], Res::tiles[Res::BitmapPool::Fallback]);

			PassOutSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Right)].initialize(Res::tiles[Res::Tiles::_Player_PassOutRight], Res::tiles[Res::BitmapPool::Fallback]);
			PassOutSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Up)].initialize(Res::tiles[Res::Tiles::_Player_PassOutUp], Res::tiles[Res::BitmapPool::Fallback]);
			PassOutSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Down)].initialize(Res::tiles[Res::Tiles::_Player_PassOutDown], Res::tiles[Res::BitmapPool::Fallback]);
			PassOutSlides[Type::Rotations::getIndexOfRotation(Type::Rotations::Left)].initialize(Res::tiles[Res::Tiles::_Player_PassOutLeft], Res::tiles[Res::BitmapPool::Fallback]);

			Sniff.initialize(Res::tiles[Res::Tiles::_Player_Sniff], Res::tiles[Res::BitmapPool::Fallback]);
			Push.initialize(Res::tiles[Res::Tiles::_Player_Push], Res::tiles[Res::BitmapPool::Fallback]);
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

			s->DrawNum = 0;
			s->controller = nullptr;
			s->flag = F_None;

			stack->o->requests.timer = true;
		}
		void CreatePassage(OBJECT_CREATER_PARAM)
		{
			Create(OBJECT_CREATER_CALL);

			stack->o->SetTranslationID(ObjectID::Space);

			gets(Specific, s);

			s->passageTimer = passageTime;
			s->flag = F_PassageDisappear;

			stack->o->events.topDraw = true;
		}

		void Print(OBJECT_PRINTER_PARAM)
		{
			gets(Specific, s);
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
			if (s->controller)
			{
				Controll(stack);
				stack->o->requests.timer = true;
			}
			regets(Specific, s);
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
				int DrawNum = PassInSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())].getDrawNumber(1 - (s->passageTimer / passageTime));

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
				MovingSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())][s->DrawNum].drawScaled(x, y, w, h);

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
					PassInSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())][s->DrawNum].drawScaled(x, y, w, h);

					if (stack->o->GetRotation() != Type::Rotations::Up &&
						stack->o->GetRotation() != Type::Rotations::Down &&
						stack->o->GetRotation() != Type::Rotations::Right &&
						stack->o->GetRotation() != Type::Rotations::Left)
						clog << KIR4::LRED << "Murphy Draw ERROR because F_PassageDisappear draw flag but options not valid" << KIR4::eol;
				}
				else
				{
					PassOutSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())][s->DrawNum].drawScaled(x, y, w, h);

					if (stack->o->GetRotation() != Type::Rotations::Up &&
						stack->o->GetRotation() != Type::Rotations::Down &&
						stack->o->GetRotation() != Type::Rotations::Right &&
						stack->o->GetRotation() != Type::Rotations::Left)
						clog << KIR4::LRED << "Murphy Draw ERROR because F_Passage draw flag but options not valid" << KIR4::eol;
				}
			}
			else if (s->flag & F_Sniff)
			{
				Sniff[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())].drawScaled(x, y, w, h);
			}
			else if (s->flag & F_Push)
			{
				Push[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())].drawScaled(x, y, w, h);
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