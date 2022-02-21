#pragma once

#include "Object.h"

namespace Object
{
	namespace Player
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		void CreatePassage(OBJECT_CREATER_PARAM);
		void Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Tick(OBJECT_TICK_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);


		void Controll(ObjectBase::Stack *stack);

		void SetSpell(ObjectBase *o, bool Spell);
		void SetMoveUp(ObjectBase *o, bool MoveUp);
		void SetMoveDown(ObjectBase *o, bool MoveDown);
		void SetMoveLeft(ObjectBase *o, bool MoveLeft);
		void SetMoveRight(ObjectBase *o, bool MoveRight);
	}
}



//typedef Object BasePlayer;
//class ObjectPlayer :public BasePlayer
//{
//	static const char
//		*name;
//	bool
//		Spell = false,
//		MoveUp = false,
//		MoveDown = false,
//		MoveLeft = false,
//		MoveRight = false;
//	static Bitmap
//		Base,
//		MovingVertical,
//		MovingHorizontal,
//		PassageVertical,
//		PassageHorizontal,
//		Push,
//		Sniff;
//	float
//		PutUnityWaitTimer,
//		PassageTimer,
//		s->EffectTimer;
//	inline void s->EffectTimerTick()
//	{
//		s->EffectTimer -= 1 / stack->CPS;
//	}
//	static const float
//		PutUnityWaitTime,
//		PushEffectWaitTime,
//		PushEffectRestTime,
//		SniffEffectTime,
//		PassageTime;
//	static const Type::Move::Type
//		PassageSpeed;
//	int
//		s->DrawNum = 0;
//	enum Flag :Type::Flags
//	{
//		F_None = 1 << 0,
//		F_Move = 1 << 1,
//		F_Passage = 1 << 2,
//		F_PassageDisappear = 1 << 3 | F_Passage,
//		F_Sniff = 1 << 4,
//		F_Push = 1 << 5,
//		F_PushTry = 1 << 6 | F_Push,
//		F_PushRest = 1 << 7 | F_Push,
//		F_PutUnityU = 1 << 8 | F_None,
//		F_PutUnityR = 1 << 9 | F_None,
//		F_PutUnityD = 1 << 10 | F_None,
//		F_PutUnityL = 1 << 11 | F_None,
//	};
//	Type::Flags
//		flag = F_None;
//public:
//	using BasePlayer::BasePlayer;
//
//	virtual ~ObjectPlayer()
//	{
//		stack->o->ief.PlayerdDead(this);
//	}
//
//	virtual void Created()
//	{
//		BasePlayer::Created();
//		DisablePhysics();
//		SetMoveSpeed({ 4,4 });
//		stack->o->AddEvent(ObjectEvent::Timer);
//		SetFlags(CanBeExplosion | CanBeKilled | ExplosionType3);
//	}
//	static void Initialize()
//	{
//		Base.load("Hamika\\Texture\\Block\\Player\\Base.png", name);
//		MovingVertical.load("Hamika\\Texture\\Block\\Player\\MovingVertical.png", name);
//		MovingHorizontal.load("Hamika\\Texture\\Block\\Player\\MovingHorizontal.png", name);
//		PassageVertical.load("Hamika\\Texture\\Block\\Player\\PassageVertical.png", name);
//		PassageHorizontal.load("Hamika\\Texture\\Block\\Player\\PassageHorizontal.png", name);
//		Sniff.load("Hamika\\Texture\\Block\\Player\\Sniff.png", name);
//		Push.load("Hamika\\Texture\\Block\\Player\\Push.png", name);
//	}
//	static Object *Create(Type::Coord coord, Interface &ief) { return new ObjectPlayer(coord, ObjectID::Player, ief); }
//	static Object *CreatePassage(Type::Coord coord, Interface &ief)
//	{
//		ObjectPlayer
//			*object = new ObjectPlayer(coord, ObjectID::Player, ief);
//
//		object->PassageTimer = PassageTime;
//		object->flag = F_PassageDisappear;
//		object->AddEvent(ObjectEvent::Draw | ObjectEvent::HightLayer);
//
//		return object;
//	}
//
//	void Eat(Object *object)
//	{
//		if (object->GetFlags()&Give1Aim)
//			stack->o->ief.AddAim(1);
//		if (object->GetFlags()&Give1Unity)
//			stack->o->ief.AddUnity(1);
//	}
//
//	inline void SetSpell(bool Spell)
//	{
//		this->Spell = Spell;
//	}
//	inline void SetMoveUp(bool MoveUp)
//	{
//		this->MoveUp = MoveUp;
//	}
//	inline void SetMoveDown(bool MoveDown)
//	{
//		this->MoveDown = MoveDown;
//	}
//	inline void SetMoveLeft(bool MoveLeft)
//	{
//		this->MoveLeft = MoveLeft;
//	}
//	inline void SetMoveRight(bool MoveRight)
//	{
//		this->MoveRight = MoveRight;
//	}
//
//	virtual void Draw()
//	{
//		if (flag & F_None)
//		{
//			Base[0].drawScaled(x,y, w,h);
//		}
//		else if (flag & F_Move)
//		{
//			if (IsMoveLeft())
//			{
//				s->DrawNum = 9 * (1 - GetMove().x);
//				MovingHorizontal[s->DrawNum].drawScaled(x,y, w,h);
//			}
//			else if (IsMoveRight())
//			{
//				s->DrawNum = 9 * (-GetMove().x);
//				MovingHorizontal[s->DrawNum].drawScaled(x,y, w,h, ALLEGRO_FLIP_HORIZONTAL);
//			}
//			else if (IsMoveUp())
//			{
//				s->DrawNum = 9 * (1 - GetMove().y);
//				MovingVertical[s->DrawNum].drawScaled(x,y, w,h, ALLEGRO_FLIP_VERTICAL);
//			}
//			else if (IsMoveDown())
//			{
//				s->DrawNum = 9 * (-GetMove().y);
//				MovingVertical[s->DrawNum].drawScaled(x,y, w,h);
//			}
//			else
//				clog << KIR4::LRED << "Player Draw ERROR because F_Move draw flag but options not valid" << KIR4::eol;
//		}
//		else if (flag & F_Passage)
//		{
//			if (GetRotation() == Type::Rotations::Up)
//			{
//				PassageVertical[s->DrawNum].drawScaled(x,y, w,h, ALLEGRO_FLIP_VERTICAL);
//			}
//			else if (GetRotation() == Type::Rotations::Down)
//			{
//				PassageVertical[s->DrawNum].drawScaled(x,y, w,h);
//			}
//			else if (GetRotation() == Type::Rotations::Right)
//			{
//				PassageHorizontal[s->DrawNum].drawScaled(x,y, w,h, ALLEGRO_FLIP_HORIZONTAL);
//			}
//			else if (GetRotation() == Type::Rotations::Left)
//			{
//				PassageHorizontal[s->DrawNum].drawScaled(x,y, w,h);
//			}
//			else
//				clog << KIR4::LRED << "Player Draw ERROR because F_Passage draw flag but options not valid" << KIR4::eol;
//		}
//		else if (flag & F_Sniff)
//		{
//			Sniff[int(GetRotation()) / 90].drawScaled(x,y, w,h);
//		}
//		else if (flag & F_Push)
//		{
//			Push[int(GetRotation()) / 90].drawScaled(x,y, w,h);
//		}
//		else
//		{
//			clog << KIR4::LRED << "Player Draw ERROR because flag not valid" << KIR4::eol;
//		}
//	}
//	inline bool PlayerCanMovePos(Type::Coord to, Type::Rotation rotation)
//	{
//		return
//			!o->GetObject(to)->IsMoving()
//			&&
//			stack->o->GetObject(to)->GetFlags() & (PlayerStepOn | StepOn)
//			&&
//			GetRemain(to)->GetFlags() &(PlayerStepOn | StepOn)
//			&&
//			(
//				GetObjectOut(to)->GetFlags() & StepOn
//				||
//				(
//				(rotation == Type::Rotations::Up && GetObjectOut(to)->GetCoord().y < to.y)
//					||
//					(rotation == Type::Rotations::Down && GetObjectOut(to)->GetCoord().y > to.y)
//					||
//					(rotation == Type::Rotations::Left && GetObjectOut(to)->GetCoord().x < to.x)
//					||
//					(rotation == Type::Rotations::Right && GetObjectOut(to)->GetCoord().x > to.x)
//					)
//				);
//	}
//	bool Move(Type::Coord to, Type::Coord to2, Type::Coord to2i, Type::Rotation rotation, Type::Flags passage, Type::Flags push)
//	{
//		if (PlayerCanMovePos(to, rotation))
//		{
//			flag = F_Move;
//			Eat(stack->o->GetObject(to));
//			stack->o->ief.ObjectMove(GetCoord(), to, Object000::Create);
//			stack->o->SetMove(rotation);
//			stack->o->SetRotation(rotation);
//			return true;
//		}
//		else if (stack->o->GetObject(to)->id == ObjectID::Exit && stack->o->ief.GetAimRemaining() == 0)
//		{
//			s->DrawNum = 0;
//			PassageTimer = PassageTime;
//			stack->o->SetRotation(GetRoundRotation(GetRealRotation(rotation + Type::Rotations::_180)));
//			flag = F_PassageDisappear;
//			stack->o->requests.draw=true;
//		}
//		else
//		{
//			Object
//				*toObject = stack->o->GetObject(to);
//			if (toObject->GetFlags()&passage && stack->o->ief.GetSectionFlags(to2)&StepOn)
//			{
//				Type::Coord
//					source = GetCoord();
//				s->DrawNum = 9;
//				flag = F_Passage;
//				Eat(stack->o->GetObject(to2));
//				stack->o->ief.ObjectMove(source, to2, CreatePassage);
//				stack->o->SetRotation(rotation);
//				SetMoveUnsafe(rotation, { 2,2 });
//				stack->o->ief.o->GetObject(source)->SetRotation(GetRoundRotation(GetRealRotation(rotation + Type::Rotations::_180)));
//				PassageTimer = PassageTime;
//				return true;
//			}
//			else if (!toObject->IsMoving() && toObject->GetFlags()&push && toObject->CanMovePos(to2, rotation))
//			{
//				if (flag != F_PushTry || GetRotation() != rotation)
//				{
//					flag = F_PushTry;
//					s->EffectTimer = PushEffectWaitTime;
//					stack->o->requests.draw=true;
//					stack->o->SetRotation(rotation);
//				}
//				else if (flag == F_PushTry)
//				{
//					s->EffectTimerTick();
//					if (s->EffectTimer <= 0)
//					{
//						flag = F_Push;
//
//						toObject->SetRotation(rotation);
//						stack->o->SetRotation(rotation);
//
//						toObject->SetMove(rotation);
//						stack->o->SetMove(rotation);
//
//						if (rotation == Type::Rotations::Left || rotation == Type::Rotations::Right)
//							EnebleLimitSpeed(GetMoveSpeed().x / (Type::Move::Type)(2));
//						else
//							EnebleLimitSpeed(GetMoveSpeed().y / (Type::Move::Type)(2));
//
//						stack->o->ief.ObjectMove(to, to2, 0);
//						stack->o->ief.ObjectMove(GetCoord(), to, GetTranslationTo());
//					}
//				}
//				return true;
//			}
//		}
//		return false;
//	}
//	virtual void Timer()
//	{
//		if (flag == F_PassageDisappear)
//		{
//			PassageTimer -= (PassageSpeed + 0.08) / stack->CPS;
//			if (PassageTimer <= 0)
//			{
//				stack->o->ief.ObjectDisappear(GetCoord());
//				s->DrawNum = 9;
//			}
//			else
//			{
//				int
//					s->DrawNum = 9.999999*(1 - PassageTimer);
//				if (this->DrawNum != s->DrawNum)
//				{
//					stack->o->requests.draw=true;
//					this->DrawNum = s->DrawNum;
//				}
//			}
//		}
//		BasePlayer::Timer();
//	}
//	virtual void Controll()
//	{
//		BasePlayer::Timer();
//		if (flag == F_Move)
//		{
//			Step();
//			if (GetEvents()&ObjectEvent::Draw)
//				stack->o->AddEvent(ObjectEvent::DrawScreen);
//		}
//		else if (flag == F_Passage)
//		{
//			PassageTimer -= PassageSpeed / stack->CPS;
//			if (PassageTimer <= 0)
//			{
//				SetMoveUnsafe({ 0,0 });
//				stack->o->ief.ObjectArrived(GetCoord());
//				flag = F_None;
//			}
//			else
//			{
//				Type::Move::Type
//					move = PassageTimer / (float)PassageTime * 2;
//				SetMoveUnsafe(GetRotation(), { move,move });
//				int
//					s->DrawNum = 9.999999*PassageTimer;
//				if (this->DrawNum != s->DrawNum)
//				{
//					stack->o->AddEvent(ObjectEvent::DrawScreen);
//					this->DrawNum = s->DrawNum;
//				}
//			}
//		}
//		else if (flag == F_Push)
//		{
//			GetObjectOut(GetCoord())->SetMove({ 0,0 });
//			Step();
//			GetObjectOut(GetCoord())->SetMove(GetMove());
//		}
//		if (!IsMoving())
//		{
//			if (flag == F_Sniff)
//			{
//				s->EffectTimerTick();
//				if (s->EffectTimer <= 0)
//				{
//					flag = F_None;
//					stack->o->requests.draw=true;
//				}
//			}
//			else if (flag == F_PushRest)
//			{
//				s->EffectTimerTick();
//				if (s->EffectTimer <= 0)
//				{
//					flag = F_None;
//					stack->o->requests.draw=true;
//				}
//			}
//
//			bool
//				FallDown = false;
//			if (stack->o->ief.GetBlockFlags(GetCoord())&GridFlags::Gravity && CanMoveDown() && stack->o->ief.GetObjectOut(GetCoordDown())->GetAbsMove() <= 0.5f)
//				FallDown = true;
//
//			if (Spell && !FallDown)
//			{
//				if (flag != F_Sniff)
//				{
//					if (MoveUp && CanSniff(GetCoordUp()))
//					{
//						Eat(stack->o->GetObject(GetCoordUp()));
//						stack->o->ief.ObjectDisappear(GetCoordUp());
//						stack->o->SetRotation(Type::Rotations::Up);
//						s->EffectTimer = SniffEffectTime;
//						flag = F_Sniff;
//						stack->o->requests.draw=true;
//					}
//					else if (MoveDown && CanSniff(GetCoordDown()))
//					{
//						Eat(stack->o->GetObject(GetCoordDown()));
//						stack->o->ief.ObjectDisappear(GetCoordDown());
//						stack->o->SetRotation(Type::Rotations::Down);
//						s->EffectTimer = SniffEffectTime;
//						flag = F_Sniff;
//						stack->o->requests.draw=true;
//					}
//					else if (MoveLeft && CanSniff(GetCoordLeft()))
//					{
//						Eat(stack->o->GetObject(GetCoordLeft()));
//						stack->o->ief.ObjectDisappear(GetCoordLeft());
//						stack->o->SetRotation(Type::Rotations::Left);
//						s->EffectTimer = SniffEffectTime;
//						flag = F_Sniff;
//						stack->o->requests.draw=true;
//					}
//					else if (MoveRight && CanSniff(GetCoordRight()))
//					{
//						Eat(stack->o->GetObject(GetCoordRight()));
//						stack->o->ief.ObjectDisappear(GetCoordRight());
//						stack->o->SetRotation(Type::Rotations::Right);
//						s->EffectTimer = SniffEffectTime;
//						flag = F_Sniff;
//						stack->o->requests.draw=true;
//					}
//					else if (stack->o->ief.GetUnityCount() > 0)
//					{
//						if (MoveUp && stack->o->ief.o->GetObject(GetCoordUp())->GetFlags()&StepOn && stack->o->ief.GetGoto(GetCoordUp()) == GetCoordUp())
//						{
//							if (flag != F_PutUnityU)
//							{
//								flag = F_PutUnityU;
//								PutUnityWaitTimer = PutUnityWaitTime;
//							}
//							else
//							{
//								PutUnityWaitTimer -= 1 / stack->CPS;
//								if (PutUnityWaitTimer <= 0)
//								{
//									stack->o->ief.AddUnity(-1);
//									Object030
//										*Unity = (Object030*)Object030::Create(GetCoordUp(), ief);
//									Unity->Created();
//									Object_D_030::Activate(Unity);
//									stack->o->ief.ObjectPut(GetCoordUp(), Unity);
//
//									stack->o->SetRotation(Type::Rotations::Up);
//									s->EffectTimer = SniffEffectTime;
//									flag = F_Sniff;
//									stack->o->requests.draw=true;
//								}
//							}
//						}
//						else if (MoveDown && stack->o->ief.o->GetObject(GetCoordDown())->GetFlags()&StepOn && stack->o->ief.GetGoto(GetCoordDown()) == GetCoordDown())
//						{
//							if (flag != F_PutUnityD)
//							{
//								flag = F_PutUnityD;
//								PutUnityWaitTimer = PutUnityWaitTime;
//							}
//							else
//							{
//								PutUnityWaitTimer -= 1 / stack->CPS;
//								if (PutUnityWaitTimer <= 0)
//								{
//									stack->o->ief.AddUnity(-1);
//									Object030
//										*Unity = (Object030*)Object030::Create(GetCoordDown(), ief);
//									Unity->Created();
//									Object_D_030::Activate(Unity);
//									stack->o->ief.ObjectPut(GetCoordDown(), Unity);
//
//									stack->o->SetRotation(Type::Rotations::Down);
//									s->EffectTimer = SniffEffectTime;
//									flag = F_Sniff;
//									stack->o->requests.draw=true;
//								}
//							}
//						}
//						else if (MoveLeft && stack->o->ief.o->GetObject(GetCoordLeft())->GetFlags()&StepOn && stack->o->ief.GetGoto(GetCoordLeft()) == GetCoordLeft())
//						{
//							if (flag != F_PutUnityL)
//							{
//								flag = F_PutUnityL;
//								PutUnityWaitTimer = PutUnityWaitTime;
//							}
//							else
//							{
//								PutUnityWaitTimer -= 1 / stack->CPS;
//								if (PutUnityWaitTimer <= 0)
//								{
//									stack->o->ief.AddUnity(-1);
//									Object030
//										*Unity = (Object030*)Object030::Create(GetCoordLeft(), ief);
//									Unity->Created();
//									Object_D_030::Activate(Unity);
//									stack->o->ief.ObjectPut(GetCoordLeft(), Unity);
//
//									stack->o->SetRotation(Type::Rotations::Left);
//									s->EffectTimer = SniffEffectTime;
//									flag = F_Sniff;
//									stack->o->requests.draw=true;
//								}
//							}
//						}
//						else if (MoveRight && stack->o->ief.o->GetObject(GetCoordRight())->GetFlags()&StepOn && stack->o->ief.GetGoto(GetCoordRight()) == GetCoordRight())
//						{
//							if (flag != F_PutUnityR)
//							{
//								flag = F_PutUnityR;
//								PutUnityWaitTimer = PutUnityWaitTime;
//							}
//							else
//							{
//								PutUnityWaitTimer -= 1 / stack->CPS;
//								if (PutUnityWaitTimer <= 0)
//								{
//									stack->o->ief.AddUnity(-1);
//									Object030
//										*Unity = (Object030*)Object030::Create(GetCoordRight(), ief);
//									Unity->Created();
//									Object_D_030::Activate(Unity);
//									stack->o->ief.ObjectPut(GetCoordRight(), Unity);
//
//									stack->o->SetRotation(Type::Rotations::Right);
//									s->EffectTimer = SniffEffectTime;
//									flag = F_Sniff;
//									stack->o->requests.draw=true;
//								}
//							}
//						}
//						else if (
//							flag == F_PutUnityU ||
//							flag == F_PutUnityR ||
//							flag == F_PutUnityD ||
//							flag == F_PutUnityL)
//							flag = F_None;
//					}
//				}
//			}
//			else
//			{
//				if (MoveUp && (!FallDown || stack->o->GetObject(GetCoordUp())->GetFlags()&GiveGravityDelay))
//				{
//					if (Move(GetCoordUp(), { GetCoord().x,GetCoord().y - 2 }, { GetCoord().x,GetCoord().y + 2 }, Type::Rotations::Up, PassageFromBottom, CanPushUp))
//						return;
//				}
//				if (MoveDown && (!FallDown || stack->o->GetObject(GetCoordDown())->GetFlags()&GiveGravityDelay))
//				{
//					if (Move(GetCoordDown(), { GetCoord().x,GetCoord().y + 2 }, { GetCoord().x,GetCoord().y - 2 }, Type::Rotations::Down, PassageFromTop, CanPushDown))
//						return;
//				}
//				if (MoveLeft && (!FallDown || stack->o->GetObject(GetCoordLeft())->GetFlags()&GiveGravityDelay))
//				{
//					if (Move(GetCoordLeft(), { GetCoord().x - 2,GetCoord().y }, { GetCoord().x + 2,GetCoord().y }, Type::Rotations::Left, PassageFromRight, CanPushLeft))
//						return;
//				}
//				if (MoveRight && (!FallDown || stack->o->GetObject(GetCoordRight())->GetFlags()&GiveGravityDelay))
//				{
//					if (Move(GetCoordRight(), { GetCoord().x + 2,GetCoord().y }, { GetCoord().x - 2,GetCoord().y }, Type::Rotations::Right, PassageFromLeft, CanPushRight))
//						return;
//				}
//
//				if (FallDown)
//				{
//					flag = F_Move;
//					Eat(stack->o->GetObject(GetCoordDown()));
//					stack->o->ief.ObjectMove(GetCoord(), GetCoordDown(), Object000::Create);
//					stack->o->SetMove({ GetMove().x,-1 });
//					//o->SetRotation(rotation);
//					return;
//				}
//
//				if (flag != F_Sniff)
//				{
//					if (MoveUp && stack->o->GetObject(GetCoordUp())->GetFlags()&ButtonPush)
//					{
//						if (stack->o->GetObject(GetCoordUp())->id == ObjectID::Terminal)
//							Object_D_028::Pushed(stack->o->GetObject(GetCoordUp()));
//						stack->o->SetRotation(Type::Rotations::Up);
//						s->EffectTimer = SniffEffectTime;
//						flag = F_Sniff;
//						stack->o->requests.draw=true;
//						return;
//					}
//					if (MoveDown && stack->o->GetObject(GetCoordDown())->GetFlags()&ButtonPush)
//					{
//						if (stack->o->GetObject(GetCoordDown())->id == ObjectID::Terminal)
//							Object_D_028::Pushed(stack->o->GetObject(GetCoordDown()));
//						stack->o->SetRotation(Type::Rotations::Down);
//						s->EffectTimer = SniffEffectTime;
//						flag = F_Sniff;
//						stack->o->requests.draw=true;
//						return;
//					}
//					if (MoveLeft && stack->o->GetObject(GetCoordLeft())->GetFlags()&ButtonPush)
//					{
//						if (stack->o->GetObject(GetCoordLeft())->id == ObjectID::Terminal)
//							Object_D_028::Pushed(stack->o->GetObject(GetCoordLeft()));
//						stack->o->SetRotation(Type::Rotations::Left);
//						s->EffectTimer = SniffEffectTime;
//						flag = F_Sniff;
//						stack->o->requests.draw=true;
//						return;
//					}
//					if (MoveRight && stack->o->GetObject(GetCoordRight())->GetFlags()&ButtonPush)
//					{
//						if (stack->o->GetObject(GetCoordRight())->id == ObjectID::Terminal)
//							Object_D_028::Pushed(stack->o->GetObject(GetCoordRight()));
//						stack->o->SetRotation(Type::Rotations::Right);
//						s->EffectTimer = SniffEffectTime;
//						flag = F_Sniff;
//						stack->o->requests.draw=true;
//						return;
//					}
//				}
//			}
//			if (flag == F_PushTry)
//			{
//				flag = F_PushRest;
//				s->EffectTimer = PushEffectRestTime;
//			}
//		}
//	}
//	virtual void ObjectArrived()
//	{
//		if (flag == F_Push)
//		{
//			stack->o->ief.GetObjectOut(GetCoord())->SetMove({ 0,0 });
//			stack->o->ief.GetObjectOut(GetCoord())->Arrived();
//			DisableLimitSpeed();
//			Arrived();
//			flag = F_PushRest;
//			s->EffectTimer = PushEffectRestTime;
//		}
//		else
//			flag = F_None;
//	}
//	inline bool CanSniff(Type::Coord coord)
//	{
//		return
//			!o->GetObject(coord)->IsMoving()
//			&&
//			stack->o->GetObject(coord)->GetFlags() & PlayerCanSniff
//			&&
//			GetRemain(coord)->GetFlags() & StepOn
//			;
//	}
//
//	inline virtual const char* Name()
//	{
//		return name;
//	}
//	virtual void Print()
//	{
//		BasePlayer::Print();
//		clog << "(KEY)Spell Is Active: " << Spell << "\n";
//		clog << "(KEY)MoveUp Is Active: " << MoveUp << "\n";
//		clog << "(KEY)MoveDown Is Active: " << MoveDown << "\n";
//		clog << "(KEY)MoveLeft Is Active: " << MoveLeft << "\n";
//		clog << "(KEY)MoveRight Is Active: " << MoveRight << "\n";
//		clog << "Flag: " << flag << "\n";
//		clog << "s->EffectTimer: " << s->EffectTimer << "\n";
//		clog << "PassageTimer: " << PassageTimer << "\n";
//		clog << "PutUnityWaitTimer: " << PutUnityWaitTimer << "\n";
//		clog << "(BITMAP)Base Is Loaded: " << Base.GetCount() << "\n";
//		clog << "(BITMAP)MovingVertical Is Loaded: " << MovingVertical.GetCount() << "\n";
//		clog << "(BITMAP)MovingHorizontal Is Loaded: " << MovingHorizontal.GetCount() << "\n";
//		clog << "(BITMAP)PassageVertical Is Loaded: " << PassageVertical.GetCount() << "\n";
//		clog << "(BITMAP)PassageHorizontal Is Loaded: " << PassageHorizontal.GetCount() << "\n";
//		clog << "(BITMAP)Push Is Loaded: " << Push.GetCount() << "\n";
//		clog << "(BITMAP)Sniff Is Loaded: " << Sniff.GetCount() << "\n";
//	}
//};
//
//Bitmap ObjectPlayer::Base;
//Bitmap ObjectPlayer::MovingVertical;
//Bitmap ObjectPlayer::MovingHorizontal;
//Bitmap ObjectPlayer::PassageVertical;
//Bitmap ObjectPlayer::PassageHorizontal;
//Bitmap ObjectPlayer::Push;
//Bitmap ObjectPlayer::Sniff;
//const char *ObjectPlayer::name = "Player";
//
//const float ObjectPlayer::PutUnityWaitTime = 1.2;
//const float ObjectPlayer::SniffEffectTime = 0.2;
//const float ObjectPlayer::PushEffectWaitTime = 0.2;
//const float ObjectPlayer::PushEffectRestTime = 0.2;
//const float ObjectPlayer::PassageTime = 1;
//const Type::Move::Type ObjectPlayer::PassageSpeed = 0.5;