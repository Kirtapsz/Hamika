#pragma once

#include "Murphy.h"
#include "Tools.h"
#include "OriginalObjects.h"
#include "BaseFunctions.h"

#include <KIR/KIR4_console.h>

namespace Object
{
	namespace Murphy
	{
		const char *name = "*** - Murphy";

		KIR5::SubBitmap Base;
		KIR5::SubBitmap PlaceBase;
		Res::Slides MovingSlides[4];
		Res::Slides PassInSlides[4];
		Res::Slides PassOutSlides[4];
		Res::Slides PushSlides;
		Res::Slides SuckSlides;

		constexpr float PlaceUnityTime = 0.4f;
		constexpr float PlaceFaceUnityTime = 0.4f;
		constexpr float PushEffectTime = 0.15f;
		constexpr float SuckEffectTime = 0.15f;
		constexpr float ExitEffectTime = 0.8f;
		constexpr float pushSpeed = CPS / 21.99f;
		constexpr float moveSpeed = CPS / 13.99f;
		constexpr float passageSpeed = moveSpeed * 2.f;

		enum EFFECTS: Type::Flags
		{
			NONE = 0,

			EFFECT_TIMER = 1 << 0,

			SUCK = 1 << 1,
			PLACE = 1 << 2,
			PLACE_FACE = 1 << 4,

			CRAWL = 1 << 5,
			PUSH_TRY = 1 << 6,
			PUSH = (1 << 7) | PUSH_TRY,
			MOVE = 1 << 8,
			FALL = 1 << 9,
			EXIT = 1 << 10,
		};

		struct Specific
		{
			KeyboardController *controller;
			DRAW_NUMBER_T draw_number_;

			float _effect_timer;
			Type::Flags _effect_type;

			bool _fall_down;
		};

		bool CanSuck(Brick *o, Type::Coord coord)
		{
			return
				!o->GetObject(coord)->isActionMove()
				&&
				o->GetObject(coord)->GetFlags() & Brick::MurphyCanSuck
				&&
				o->GetRemain(coord)->GetFlags() & Brick::StepOn
				;
		}

		void Eat(Brick *murphy, Brick *object)
		{
			if (object->GetFlags() & Brick::MurphyDies)
			{
				murphy->blowUp(murphy->GetCoord());
			}
			if (object->GetFlags() & Brick::Give1Score)
			{
				if (murphy->scene->getScoreToCollect() > 0)
				{
					murphy->scene->addScore(1);
					if (murphy->scene->getScoreToCollect() == 0)
					{
						Exit_015::Open(murphy);
					}
				}
			}
			if (object->GetFlags() & Brick::Give1Unity)
			{
				murphy->scene->addUnity(1);
			}
		}

		void SetController(Brick *o, KeyboardController *controller)
		{
			Brick::Stack stack(o);
			Specific *spec = stack;
			spec->controller = controller;
		}

		bool CanMovePos(Brick *o, Type::Coord to, Type::Rotation rotation, Type::Flags _step_flags)
		{
			return
				!o->GetObject(to)->isActionMove()
				&&
				o->GetObject(to)->GetFlags() & _step_flags
				&&
				o->GetRemain(to)->GetFlags() & _step_flags
				&&
				(
					o->GetObjectOut(to)->GetFlags() & Brick::StepOn
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
		inline bool MurphyCanMovePos(Brick *o, Type::Coord to, Type::Rotation rotation)
		{
			return CanMovePos(o, to, rotation, Brick::MurphyStepOn | Brick::StepOn);
		}
		void timerCrawlTail(Brick *_tail, Brick *_head)
		{
			Brick::Stack stack(_tail);
			Specific *spec = stack;

			if (_head->GetAbsMove() > 0)
			{
				DRAW_NUMBER_ASC(_head->GetAbsMove(), 2.f, spec->draw_number_, _tail, PassInSlides[_tail->getMoveDirection()]);
			}
			else
			{
				_tail->requests.remove = true;
			}
		}
		void createCrawlExit(Brick *_object, Type::Direction _direction)
		{
			Brick::Stack stack(_object);
			Specific *spec = stack;
			Type::Direction direction = (_direction + 2) % 4;

			spec->_effect_type = EFFECTS::EXIT;
			spec->_effect_timer = ExitEffectTime;
			_object->SetRotation(Type::Rotations::getRotationOfIndex(direction));

			_object->events.timer = true;
			_object->requests.timer = true;
			DRAW_NUMBER_ASC_INIT(spec->draw_number_, _object, PassInSlides[direction]);
		}
		void createCrawlTail(Brick *_tail, Brick *_head)
		{
			Brick::Stack stack(_tail);
			Specific *spec = stack;
			Type::Direction direction = (_head->getMoveDirection() + 2) % 4;

			spec->_effect_type = EFFECTS::CRAWL;
			_tail->SetRotation(Type::Rotations::getRotationOfIndex(direction));
			DRAW_NUMBER_ASC_INIT(spec->draw_number_, _tail, PassInSlides[_tail->getMoveDirection()]);
		}

		void InitializerCrawlTail(OBJECT_INITIALIZER_PARAM)
		{

		}
		void CreateCrawlTail(OBJECT_CREATER_PARAM)
		{
			Specific *spec = *stack;

			stack->o->SetFlags(Brick::StepOn | Brick::MurphyStepOn | Brick::CanBeExplosion);
			stack->o->events.topDraw = true;
		}
		void TimerCrawlTail(OBJECT_TIMER_PARAM)
		{
			Specific *spec = *stack;

			if (spec->_effect_type == EFFECTS::EXIT)
			{
				spec->_effect_timer -= CA;
				if (spec->_effect_timer <= 0)
				{
					stack->o->requests.remove = true;
				}
				else
				{
					DRAW_NUMBER_ASC(spec->_effect_timer, ExitEffectTime, spec->draw_number_, stack->o, PassInSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())]);
					stack->o->requests.timer = true;
				}
			}
		}
		void UpdateCrawlTail(OBJECT_UPDATE_PARAM)
		{

		}
		void DrawnerCrawlTail(OBJECT_DRAWNER_PARAM)
		{
			Specific *spec = *stack;

			PassInSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())][spec->draw_number_].drawScaled(x, y, w, h);
		}
		void simpleDrawCrawlTail(OBJECT_SIMPLE_DRAWNER_PARAM)
		{

		}


		void TimeUpPlaceExplosive(Brick::Stack *stack)
		{
			Specific *spec = *stack;

			if (!(spec->_effect_type & EFFECTS::PLACE_FACE))
			{
				spec->_effect_timer = PlaceFaceUnityTime;
				spec->_effect_type = EFFECTS::PLACE | EFFECTS::PLACE_FACE | EFFECTS::EFFECT_TIMER;
				return;
			}

			const std::array<const KeyboardController::STATE *, 4> move_controllers{&spec->controller->actionUp, &spec->controller->actionRight, &spec->controller->actionDown, &spec->controller->actionLeft};

			if ([&]()->bool
			{
				for (Type::Direction direction = 0; direction < 4; ++direction)
				{
					if (*move_controllers[direction])
					{
						return true;
					}
				}
				return false;
			}())
			{
				for (Type::Direction direction = 0; direction < 4; ++direction)
				{
					if (*move_controllers[direction])
					{
						Type::Coord to = stack->o->GetCoord(direction);

						if (stack->o->scene->GetSectionFlags(to) & Brick::StepOn)
						{
							stack->o->scene->addUnity(-1);
							Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);

							stack->o->SetRotation(rotation);
							spec->_effect_timer = SuckEffectTime;
							spec->_effect_type = EFFECTS::SUCK | EFFECTS::EFFECT_TIMER;
							stack->o->requests.draw = true;

							stack->o->scene->ObjectPut(to, ObjectID::Utility2);
							if (stack->o->GetObject(to)->id == ObjectID::Utility2)
							{
								Utility2_030::Activate(stack->o->GetObject(to));
							}
							return;
						}
					}
				}

				spec->_effect_type = EFFECTS::NONE;
			}
			else
			{
				stack->o->scene->addUnity(-1);
				Type::Coord coord = stack->o->GetCoord();

				stack->o->scene->RemainPut(coord, ObjectID::Utility2);
				if (stack->o->GetRemain(coord)->id == ObjectID::Utility2)
				{
					Utility2_030::Activate(stack->o->GetRemain(coord));
					stack->o->GetRemain(coord)->events.topDraw = true;
				}

				spec->_effect_type = EFFECTS::NONE;
			}
		}
		bool ControllPlaceExplosive(Brick::Stack *stack)
		{
			Specific *spec = *stack;

			if (spec->_effect_type == EFFECTS::NONE && stack->o->scene->getUnity() > 0)
			{
				spec->_effect_timer = PlaceUnityTime;
				spec->_effect_type = EFFECTS::PLACE | EFFECTS::EFFECT_TIMER;
				stack->o->requests.draw = true;
				return true;
			}

			return false;
		}
		bool ControllSuck(Brick::Stack *stack)
		{
			Specific *spec = *stack;
			if (!(spec->_effect_type & EFFECTS::SUCK))
			{
				const std::array<const KeyboardController::STATE *, 4> move_controllers{&spec->controller->actionUp, &spec->controller->actionRight, &spec->controller->actionDown, &spec->controller->actionLeft};
				for (Type::Direction direction = 0; direction < 4; ++direction)
				{
					if (*move_controllers[direction])
					{
						Type::Coord to = stack->o->GetCoord(direction);

						if (CanSuck(stack->o, to))
						{
							Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);

							Brick *to_object = stack->o->GetObject(to);
							Eat(stack->o, to_object);
							stack->o->scene->ObjectDisappear(to);
							if (to_object->isExists && to_object->events.update)
							{
								to_object->RunUpdate(Brick::UPDATE_MURPHY);
							}
							stack->o->SetRotation(rotation);
							spec->_effect_timer = SuckEffectTime;
							spec->_effect_type = EFFECTS::SUCK | EFFECTS::EFFECT_TIMER;
							stack->o->requests.draw = true;

							return true;
						}
					}
				}
			}
			return false;
		}
		bool ControllMove(Brick::Stack *stack)
		{
			Specific *spec = *stack;

			const std::array<const KeyboardController::STATE *, 4> move_controllers{&spec->controller->actionUp, &spec->controller->actionRight, &spec->controller->actionDown, &spec->controller->actionLeft};
			for (Type::Direction direction = 0; direction < 4; ++direction)
			{
				if (*move_controllers[direction])
				{
					Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);
					Type::Coord to = stack->o->GetCoord(direction);

					if (MurphyCanMovePos(stack->o, to, rotation))
					{
						Brick *to_object = stack->o->GetObject(to);
						if (to_object->GetFlags() & Brick::GiveGravityDelay)
						{
							spec->_fall_down = false;
						}
						Eat(stack->o, to_object);
						stack->o->SetMoveSpeed({moveSpeed,moveSpeed});
						stack->o->doMove(Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(rotation)], ObjectID::Space);
						stack->o->scene->murphyMoved(stack->o);
						stack->o->SetRotation(rotation);
						spec->_effect_type = EFFECTS::MOVE;
						return true;
					}
				}
			}
			return false;
		}
		bool ControllFallDown(Brick::Stack *stack)
		{
			Specific *spec = *stack;

			if (stack->o->scene->IsGlobalGravity() || stack->o->scene->GetBlockFlags(stack->o->GetCoord()) & GridFlags::Gravity)
			{
				if (spec->_fall_down)
				{
					static constexpr Type::Rotation rotation = Type::Rotations::Down;
					Type::Coord to = stack->o->GetCoordDown();

					if (CanMovePos(stack->o, to, rotation, Brick::StepOn))
					{
						Eat(stack->o, stack->o->GetObject(to));
						stack->o->SetMoveSpeed({moveSpeed,moveSpeed});
						stack->o->doMove(Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(rotation)], ObjectID::Space);
						stack->o->scene->murphyMoved(stack->o);
						stack->o->SetRotation(rotation);
						spec->_effect_type = EFFECTS::MOVE | EFFECTS::FALL;
						return true;
					}
				}
				else
				{
					spec->_fall_down = true;
				}
			}

			return false;
		}
		bool ControllPush(Brick::Stack *stack)
		{
			Specific *spec = *stack;

			const std::array<const KeyboardController::STATE *, 4> move_controllers{&spec->controller->actionUp, &spec->controller->actionRight, &spec->controller->actionDown, &spec->controller->actionLeft};
			static constexpr std::array<Type::Flags, 4> move_push_flags{Brick::CanPushUp, Brick::CanPushRight, Brick::CanPushDown, Brick::CanPushLeft};

			if (spec->_effect_type == EFFECTS::PUSH_TRY)
			{
				Type::Rotation rotation = stack->o->GetRotation();
				Type::Direction direction = Type::Rotations::getIndexOfRotation(rotation);
				if (!*move_controllers[direction])
				{
					spec->_effect_type = EFFECTS::NONE;
					stack->o->requests.draw = true;
				}
			}
			if (spec->_effect_type == EFFECTS::NONE)
			{
				for (Type::Direction direction = 0; direction < 4; ++direction)
				{
					if (*move_controllers[direction])
					{
						Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);
						Type::Coord to = stack->o->GetCoord(direction);

						Brick *to_object = stack->o->scene->GetObject(to);

						if (to_object->GetFlags() & move_push_flags[direction])
						{
							spec->_effect_type = EFFECTS::PUSH_TRY | EFFECTS::EFFECT_TIMER;
							spec->_effect_timer = PushEffectTime;
							stack->o->SetRotation(rotation);
							stack->o->requests.draw = true;
							return true;
						}
					}
				}
			}
			if (spec->_effect_type == EFFECTS::PUSH)
			{
				Type::Rotation rotation = stack->o->GetRotation();
				Type::Direction direction = Type::Rotations::getIndexOfRotation(rotation);
				if (*move_controllers[direction])
				{
					Type::Coord to = stack->o->GetCoord(direction);

					Brick *to_object = stack->o->scene->GetObject(to);
					Type::Coord to_object_next = to_object->GetCoord(direction);

					if (to_object->GetFlags() & move_push_flags[direction] &&
						!to_object->isActionMove() &&
						CanMovePos(to_object, to_object_next, rotation, Brick::StepOn))
					{
						to_object->SetRotation(rotation);

						to_object->SetMove(rotation);
						stack->o->SetMove(rotation);

						to_object->enableFixSpeed(pushSpeed);
						stack->o->enableFixSpeed(pushSpeed);

						to_object->doMove(Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(rotation)], ObjectID::Space);
						to_object->action = Brick::LINKED;
						stack->o->doMove(Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(rotation)], stack->o->GetObjectIDremain());
						stack->o->scene->murphyMoved(stack->o);

						spec->_effect_type = EFFECTS::PUSH;
						return true;
					}
				}
				else
				{
					spec->_effect_type = EFFECTS::NONE;
					stack->o->requests.draw = true;
				}
			}
			return false;
		}
		bool ControllButton(Brick::Stack *stack)
		{
			Specific *spec = *stack;

			const std::array<const KeyboardController::STATE *, 4> move_controllers{&spec->controller->actionUp, &spec->controller->actionRight, &spec->controller->actionDown, &spec->controller->actionLeft};
			for (Type::Direction direction = 0; direction < 4; ++direction)
			{
				if (*move_controllers[direction])
				{
					Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);
					Type::Coord to = stack->o->GetCoord(direction);

					if (stack->o->scene->GetUnionFlags(to) & Brick::ButtonPush)
					{
						if (stack->o->GetObject(to)->id == ObjectID::Terminal)
						{
							Terminal_028::Pushed(stack->o->GetObject(to));
						}
						stack->o->SetRotation(rotation);
						spec->_effect_timer = SuckEffectTime;
						spec->_effect_type = EFFECTS::SUCK | EFFECTS::EFFECT_TIMER;
						stack->o->requests.draw = true;
						return true;
					}
				}
			}
			return false;
		}
		bool ControllExit(Brick::Stack *stack)
		{
			if (stack->o->scene->getScoreToCollect() == 0)
			{
				Specific *spec = *stack;

				const std::array<const KeyboardController::STATE *, 4> move_controllers{&spec->controller->actionUp, &spec->controller->actionRight, &spec->controller->actionDown, &spec->controller->actionLeft};
				for (Type::Direction direction = 0; direction < 4; ++direction)
				{
					if (*move_controllers[direction])
					{
						Type::Coord to = stack->o->GetCoord(direction);

						if (stack->o->GetObject(to)->id == ObjectID::Exit)
						{
							Type::Coord coord = stack->o->GetCoord();

							stack->o->scene->murphyVictory();
							stack->o->scene->ObjectPut(coord, ObjectID::MurphyCrawlTail);
							createCrawlExit(stack->o, direction);
							return true;
						}
					}
				}
			}
			return false;
		}
		bool ControllCrawl(Brick::Stack *stack)
		{
			Specific *spec = *stack;

			const std::array<const KeyboardController::STATE *, 4> move_controllers{&spec->controller->actionUp, &spec->controller->actionRight, &spec->controller->actionDown, &spec->controller->actionLeft};
			static constexpr std::array<Type::Flags, 4> move_crawl_flags{Brick::PassageFromBottom, Brick::PassageFromLeft, Brick::PassageFromTop, Brick::PassageFromRight};

			for (Type::Direction direction = 0; direction < 4; ++direction)
			{
				if (*move_controllers[direction])
				{
					Type::Coord next = stack->o->GetCoord(direction);
					Brick *next_object = stack->o->scene->GetObject(next);

					if (next_object->GetFlags() & move_crawl_flags[direction])
					{
						Type::Coord to = next_object->GetCoord(direction);

						if (stack->o->scene->GetSectionFlags(to) & Brick::StepOn)
						{
							Brick *to_object = stack->o->scene->GetObject(to);
							Type::Coord coord = stack->o->GetCoord();

							Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);
							Eat(stack->o, to_object);
							stack->o->SetMoveSpeed({passageSpeed,passageSpeed});
							stack->o->doMoveEx(Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(rotation)],
											   ObjectID::MurphyCrawlTail,
											   to,
											   {Type::Move::base(coord.x - to.x), Type::Move::base(coord.y - to.y)}
							);
							stack->o->scene->murphyMoved(stack->o);
							stack->o->setRoundDrawCoord();
							stack->o->SetRotation(rotation);
							spec->_effect_type = EFFECTS::CRAWL;
							Brick *tail = stack->o->scene->GetObjectU(stack->o->scene->GetComefrom(stack->o->GetCoord()));
							createCrawlTail(tail, stack->o);
							DRAW_NUMBER_DESC_INIT(spec->draw_number_, stack->o, PassOutSlides[stack->o->getMoveDirection()]);
							return true;
						}
					}
				}
			}
			return false;
		}


		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Base = Res::tiles[Res::Tiles::_Player_Base];
			PlaceBase = Res::tiles[Res::Tiles::_Player_PlaceBase];

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

			SuckSlides.initialize(Res::tiles[Res::Tiles::_Player_Suck], Res::tiles[Res::BitmapPool::Fallback]);
			PushSlides.initialize(Res::tiles[Res::Tiles::_Player_Push], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Specific *spec = *stack;

			stack->o->SetFlags(Brick::CanBeExplosion | Brick::CanBeKilled | Brick::ExplosionType3);
			stack->o->disablePhysics();

			stack->o->events.timer = false;
			stack->o->events.update = false;

			stack->o->SetTranslationID(ObjectID::Infotron);
			stack->o->SetObjectIDremain(ObjectID::Space);

			spec->draw_number_ = 0;
			spec->controller = nullptr;
			spec->_effect_type = EFFECTS::NONE;
			spec->_fall_down = true;
		}

		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Specific *spec = *stack;
			Json json;

			json["draw_number"] = spec->draw_number_;
			json["effect_timer"] = spec->_effect_timer;
			json["effect_type"] = spec->_effect_type;
			json["fall_down"] = spec->_fall_down;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Specific *spec = *stack;

			if (spec->_effect_type & EFFECTS::EFFECT_TIMER)
			{
				spec->_effect_timer -= CA;
				if (spec->_effect_timer <= 0)
				{
					if (spec->_effect_type & EFFECTS::PLACE)
					{
						TimeUpPlaceExplosive(stack);
					}
					else if (spec->_effect_type & EFFECTS::PUSH_TRY)
					{
						spec->_effect_type = EFFECTS::PUSH;
					}
					else
					{
						spec->_effect_type = EFFECTS::NONE;
					}
					stack->o->requests.draw = true;
				}
			}

			if (stack->o->isActionMove())
			{
				if (spec->_effect_type == EFFECTS::CRAWL)
				{
					Brick *tail = stack->o->scene->GetObjectU(stack->o->scene->GetComefrom(stack->o->GetCoord()));
					stack->o->Step();
					timerCrawlTail(tail, stack->o);

					stack->o->setRoundDrawCoord();
					DRAW_NUMBER_DESC(stack->o->GetAbsMove(), 2.f, spec->draw_number_, stack->o, PassOutSlides[stack->o->getMoveDirection()]);
				}
				else if (spec->_effect_type == EFFECTS::PUSH)
				{
					Brick *push_object = stack->o->GetObjectOut(stack->o->GetCoord());
					if (push_object)
					{
						push_object->Step();
						if (!push_object->IsMove())
						{
							push_object->disableFixSpeed();
						}
						push_object->requests.timer = true;
					}
					stack->o->Step();
					if (!stack->o->IsMove())
					{
						stack->o->disableFixSpeed();
						spec->_effect_type = EFFECTS::NONE;
					}
				}
				else
				{
					stack->o->Step();
					DRAW_NUMBER_ASC(stack->o->GetAbsMove(), 1.f, spec->draw_number_, stack->o, MovingSlides[stack->o->getMoveDirection()]);
				}
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Specific *spec = *stack;

			if (spec->controller->actionDestroy)
			{
				stack->o->blowUp(stack->o->GetCoord());
				return;
			}
			if (!stack->o->IsMove())
			{
				if (ControllFallDown(stack))
				{
					return;
				}
				if (spec->controller->actionSpecial)
				{
					if (stack->o->isAction())
					{
						spec->_effect_type = EFFECTS::NONE;
						stack->o->finishMove();
					}

					if (ControllSuck(stack))
					{
						return;
					}
					else if (ControllPlaceExplosive(stack))
					{
						return;
					}
				}
				else
				{
					if (ControllMove(stack))
					{
						return;
					}
					else if (ControllCrawl(stack))
					{
						return;
					}
					else if (ControllPush(stack))
					{
						return;
					}
					else if (ControllButton(stack))
					{

					}
					else if (ControllExit(stack))
					{
						return;
					}

					if (stack->o->isAction() || spec->_effect_type & EFFECTS::PLACE)
					{
						spec->_effect_type = EFFECTS::NONE;
						stack->o->finishMove();
					}
				}

			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Specific *spec = *stack;

			if (stack->o->IsMove())
			{
				if (spec->_effect_type & EFFECTS::CRAWL)
				{
					PassOutSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())][spec->draw_number_].drawScaled(x, y, w, h);
				}
				else if (spec->_effect_type & EFFECTS::PUSH_TRY)
				{
					PushSlides[stack->o->getMoveDirection()].drawScaled(x, y, w, h);
				}
				else if (spec->_effect_type & EFFECTS::FALL)
				{
					MovingSlides[(stack->o->getMoveDirection() + 2) % 4][spec->draw_number_].drawScaled(x, y, w, h);
				}
				else
				{
					MovingSlides[stack->o->getMoveDirection()][spec->draw_number_].drawScaled(x, y, w, h);
				}
			}
			else
			{
				if (spec->_effect_type & EFFECTS::SUCK)
				{
					SuckSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())].drawScaled(x, y, w, h);
				}
				else if (spec->_effect_type & EFFECTS::PLACE_FACE)
				{
					PlaceBase.drawScaled(x, y, w, h);
				}
				else if (spec->_effect_type & EFFECTS::PUSH_TRY)
				{
					PushSlides[Type::Rotations::getIndexOfRotation(stack->o->GetRotation())].drawScaled(x, y, w, h);
				}
				else
				{
					Base.drawScaled(x, y, w, h);
				}
			}
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Base.drawScaled(x, y, w, h);
		}
	}
}