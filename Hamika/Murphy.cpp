#pragma once

#include "Murphy.h"
#include "Object.h"
#include "Tools.h"
#include "OriginalEntities.h"
#include "BaseFunctions.h"
#include "KeyboardController.h"

#include <KIR/KIR4_console.h>

namespace Object::Entity
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
		constexpr float PushEffectTime = 0.08325f;
		constexpr float SuckEffectTime = 0.15f;
		constexpr float ExitEffectTime = 0.8f;
		constexpr float pushSpeed = CPS / 21.99f;
		constexpr float moveSpeed = CPS / 13.99f;
		constexpr float passageSpeed = moveSpeed * 3.f;

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

		bool CanSuck(Brick &_brick, Type::Coord coord)
		{
			return
				!_brick.GetObject(coord).isActionMove()
				&&
				_brick.GetObject(coord).GetFlags() & Flags::MurphyCanSuck
				&&
				_brick.GetRemain(coord).GetFlags() & Flags::StepOn
				;
		}

		void Eat(Brick &murphy, Brick &_brick, Brick &remain)
		{
			if (_brick.isExists)
			{
				if (_brick.GetFlags() & Flags::MurphyDies)
				{
					murphy.scene->blowup(murphy);
				}
				if (_brick.GetFlags() & Flags::Give1Score)
				{
					if (murphy.scene->getScoreToCollect() > 0)
					{
						murphy.scene->addScore(1);
						if (murphy.scene->getScoreToCollect() == 0)
						{
							Exit_015::Open(murphy);
						}
					}
				}
				if (_brick.GetFlags() & Flags::Give1Unity)
				{
					murphy.scene->addUnity(1);
				}
			}
			if (remain.isExists)
			{
				if (remain.GetFlags() & Flags::MurphyDies)
				{
					murphy.scene->blowup(murphy);
				}
			}
		}

		void SetController(Brick &_brick, KeyboardController *controller)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			entity_data.controller = controller;
		}

		bool CanMovePos(Brick &_brick, Type::Coord to, Type::Rotation rotation, Type::Flags _step_flags)
		{
			return
				!_brick.GetObject(to).isActionMove()
				&&
				_brick.GetObject(to).GetFlags() & _step_flags
				&&
				_brick.GetRemain(to).GetFlags() & _step_flags
				&&
				(
					_brick.GetObjectOut(to).GetFlags() & Flags::StepOn
					||
					(
						(rotation == Type::Rotations::Up && _brick.GetObjectOut(to).GetCoord().y() < to.y())
						||
						(rotation == Type::Rotations::Down && _brick.GetObjectOut(to).GetCoord().y() > to.y())
						||
						(rotation == Type::Rotations::Left && _brick.GetObjectOut(to).GetCoord().x() < to.x())
						||
						(rotation == Type::Rotations::Right && _brick.GetObjectOut(to).GetCoord().x() > to.x())
						)
					);
		}
		inline bool MurphyCanMovePos(Brick &_brick, Type::Coord to, Type::Rotation rotation)
		{
			return CanMovePos(_brick, to, rotation, Flags::MurphyStepOn | Flags::StepOn);
		}
		void timerCrawlTail(Brick &_tail, Brick &_head)
		{
			EntityData &entity_data = _tail.entity_data.murphy;

			if (_head.GetAbsMove() > 0)
			{
				DRAW_NUMBER_ASC(_head.GetAbsMove(), 2.f, entity_data.draw_number_, _tail, PassInSlides[_tail.getMoveDirection()]);
			}
			else
			{
				_tail.requests.remove = true;
			}
		}
		void createCrawlExit(Brick &_brick, Type::Direction _direction)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			Type::Direction direction = (_direction + 2) % 4;

			_brick._draw_type = Brick::DrawType::Top;
			entity_data._effect_type = EFFECTS::EXIT;
			entity_data._effect_timer = ExitEffectTime;
			_brick.SetRotation(Type::Rotations::getRotationOfIndex(direction));
			_brick.SetTranslationID(ObjectID::Infotron);

			_brick.SetFlags(Flags::CanBeExploded | Flags::CanBeKilled | Flags::ExplosionType3);
			_brick.disablePhysics();
			_brick.SetObjectIDremain(ObjectID::Space);

			_brick.events.timer = true;
			_brick.requests.timer = true;

			DRAW_NUMBER_ASC_INIT(entity_data.draw_number_, _brick, PassInSlides[direction]);
		}
		void createCrawlTail(Brick &_tail, Brick &_head)
		{
			EntityData &entity_data = _tail.entity_data.murphy;

			Type::Direction direction = (_head.getMoveDirection() + 2) % 4;

			_tail._draw_type = Brick::DrawType::Top;
			entity_data._effect_type = EFFECTS::CRAWL;
			_tail.SetRotation(Type::Rotations::getRotationOfIndex(direction));
			_tail.SetTranslationID(ObjectID::Space);

			_tail.SetFlags(Flags::CanBeExploded);
			_tail.disablePhysics();
			_tail.SetObjectIDremain(ObjectID::Space);

			DRAW_NUMBER_ASC_INIT(entity_data.draw_number_, _tail, PassInSlides[_tail.getMoveDirection()]);
		}

		void TimerCrawlTail(OBJECT_TIMER_PARAM)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			if (entity_data._effect_type == EFFECTS::EXIT)
			{
				entity_data._effect_timer -= CA;
				if (entity_data._effect_timer <= 0)
				{
					_brick.requests.remove = true;
				}
				else
				{
					DRAW_NUMBER_ASC(entity_data._effect_timer, ExitEffectTime, entity_data.draw_number_, _brick, PassInSlides[Type::Rotations::getIndexOfRotation(_brick.GetRotation())]);
					_brick.requests.timer = true;
				}
			}
		}
		void UpdateCrawlTail(OBJECT_UPDATE_PARAM)
		{

		}
		void DrawnerCrawlTail(OBJECT_DRAWNER_PARAM)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			PassInSlides[Type::Rotations::getIndexOfRotation(_brick.GetRotation())][entity_data.draw_number_].drawScaled(x, y, w, h);
		}
		void simpleDrawCrawlTail(OBJECT_SIMPLE_DRAWNER_PARAM)
		{

		}


		void TimeUpPlaceExplosive(Brick &_brick)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			if (!(entity_data._effect_type & EFFECTS::PLACE_FACE))
			{
				entity_data._effect_timer = PlaceFaceUnityTime;
				entity_data._effect_type = EFFECTS::PLACE | EFFECTS::PLACE_FACE | EFFECTS::EFFECT_TIMER;
				return;
			}

			const std::array<const KeyboardController::STATE *, 4> move_controllers{&entity_data.controller->actionUp, &entity_data.controller->actionRight, &entity_data.controller->actionDown, &entity_data.controller->actionLeft};

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
						Type::Coord to = _brick.GetCoord(direction);

						if (_brick.scene->GetSectionFlags(to) & Flags::StepOn)
						{
							_brick.scene->addUnity(-1);
							Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);

							_brick.SetRotation(rotation);
							entity_data._effect_timer = SuckEffectTime;
							entity_data._effect_type = EFFECTS::SUCK | EFFECTS::EFFECT_TIMER;
							_brick.requests.draw = true;

							_brick.scene->ObjectPut(to, ObjectID::Utility2);
							if (_brick.GetObject(to).id == ObjectID::Utility2)
							{
								Utility2_030::Activate(_brick.GetObject(to));
							}
							return;
						}
					}
				}

				entity_data._effect_type = EFFECTS::NONE;
			}
			else
			{
				_brick.scene->addUnity(-1);
				Type::Coord coord = _brick.GetCoord();

				_brick.scene->RemainPut(coord, ObjectID::Utility2);
				if (_brick.GetRemain(coord).id == ObjectID::Utility2)
				{
					Utility2_030::Activate(_brick.GetRemain(coord));
					_brick.scene->GetRemain(coord)._draw_type = Brick::DrawType::Top;
				}

				entity_data._effect_type = EFFECTS::NONE;
			}
		}
		bool ControllPlaceExplosive(Brick &_brick)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			if (entity_data._effect_type == EFFECTS::NONE && _brick.scene->getUnity() > 0)
			{
				entity_data._effect_timer = PlaceUnityTime;
				entity_data._effect_type = EFFECTS::PLACE | EFFECTS::EFFECT_TIMER;
				_brick.requests.draw = true;
				return true;
			}

			return false;
		}
		bool ControllSuck(Brick &_brick)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			if (!(entity_data._effect_type & EFFECTS::SUCK))
			{
				const std::array<const KeyboardController::STATE *, 4> move_controllers{&entity_data.controller->actionUp, &entity_data.controller->actionRight, &entity_data.controller->actionDown, &entity_data.controller->actionLeft};
				for (Type::Direction direction = 0; direction < 4; ++direction)
				{
					if (*move_controllers[direction])
					{
						Type::Coord to = _brick.GetCoord(direction);

						if (CanSuck(_brick, to))
						{
							Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);

							Brick &to_object = _brick.GetObject(to);
							Eat(_brick, to_object, _brick.scene->GetRemain(to));
							_brick.scene->ObjectDisappear(to);
							if (to_object.isExists && to_object.events.update)
							{
								to_object.RunUpdate(UpdateType::UPDATE_MURPHY);
							}
							_brick.SetRotation(rotation);
							entity_data._effect_timer = SuckEffectTime;
							entity_data._effect_type = EFFECTS::SUCK | EFFECTS::EFFECT_TIMER;
							_brick.requests.draw = true;

							return true;
						}
					}
				}
			}
			return false;
		}
		bool CheckGravity(Brick &_brick)
		{
			return _brick.scene->IsGlobalGravity() || _brick.scene->GetBlockFlags(_brick.GetCoord()) & GridFlags::Gravity;
		}
		bool CanFallDown(Brick &_brick)
		{
			static constexpr Type::Rotation rotation = Type::Rotations::Down;
			Type::Coord to = _brick.GetCoordDown();
			return CanMovePos(_brick, to, rotation, Flags::StepOn);
		}
		bool ControllMove(Brick &_brick)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			bool can_fall_down = CheckGravity(_brick) ? CanFallDown(_brick) : false;

			const std::array<const KeyboardController::STATE *, 4> move_controllers{&entity_data.controller->actionUp, &entity_data.controller->actionRight, &entity_data.controller->actionDown, &entity_data.controller->actionLeft};
			for (Type::Direction direction = 0; direction < 4; ++direction)
			{
				if (*move_controllers[direction])
				{
					Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);
					Type::Coord to = _brick.GetCoord(direction);

					if (MurphyCanMovePos(_brick, to, rotation))
					{
						Brick &to_object = _brick.GetObject(to);
						if (to_object.GetFlags() & Flags::GiveGravityDelay || !can_fall_down)
						{
							Eat(_brick, to_object, _brick.scene->GetRemain(to));
							_brick.SetMoveSpeed({moveSpeed,moveSpeed});
							_brick.doMove(Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(rotation)], ObjectID::Space);
							_brick.scene->murphyMoved(&_brick);
							_brick.SetRotation(rotation);
							entity_data._effect_type = EFFECTS::MOVE;
							return true;
						}
					}
				}
			}
			return false;
		}
		bool ControllFallDown(Brick &_brick)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			if (CheckGravity(_brick) && CanFallDown(_brick))
			{
				static constexpr Type::Rotation rotation = Type::Rotations::Down;
				Type::Coord to = _brick.GetCoordDown();

				Eat(_brick, _brick.GetObject(to), _brick.scene->GetRemain(to));
				_brick.SetMoveSpeed({moveSpeed,moveSpeed});
				_brick.doMove(Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(rotation)], ObjectID::Space);
				_brick.scene->murphyMoved(&_brick);
				_brick.SetRotation(rotation);
				entity_data._effect_type = EFFECTS::MOVE | EFFECTS::FALL;
				return true;
			}

			return false;
		}
		bool ControllPush(Brick &_brick)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			const std::array<const KeyboardController::STATE *, 4> move_controllers{&entity_data.controller->actionUp, &entity_data.controller->actionRight, &entity_data.controller->actionDown, &entity_data.controller->actionLeft};
			static constexpr std::array<Type::Flags, 4> move_push_flags{Flags::CanPushUp, Flags::CanPushRight, Flags::CanPushDown, Flags::CanPushLeft};

			if (entity_data._effect_type == EFFECTS::PUSH_TRY)
			{
				Type::Rotation rotation = _brick.GetRotation();
				Type::Direction direction = Type::Rotations::getIndexOfRotation(rotation);
				if (!*move_controllers[direction])
				{
					entity_data._effect_type = EFFECTS::NONE;
					_brick.requests.draw = true;
				}
			}
			if (entity_data._effect_type == EFFECTS::NONE)
			{
				for (Type::Direction direction = 0; direction < 4; ++direction)
				{
					if (*move_controllers[direction])
					{
						Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);
						Type::Coord to = _brick.GetCoord(direction);

						Brick &to_object = _brick.scene->GetObject(to);

						if (to_object.GetFlags() & move_push_flags[direction])
						{
							entity_data._effect_type = EFFECTS::PUSH_TRY | EFFECTS::EFFECT_TIMER;
							entity_data._effect_timer = PushEffectTime;
							_brick.SetRotation(rotation);
							_brick.requests.draw = true;
							return true;
						}
					}
				}
			}
			if (entity_data._effect_type == EFFECTS::PUSH)
			{
				Type::Rotation rotation = _brick.GetRotation();
				Type::Direction direction = Type::Rotations::getIndexOfRotation(rotation);
				if (*move_controllers[direction])
				{
					Type::Coord to = _brick.GetCoord(direction);

					Brick &to_object = _brick.scene->GetObject(to);
					Type::Coord to_object_next = to_object.GetCoord(direction);

					if (to_object.GetFlags() & move_push_flags[direction] &&
						!to_object.isActionMove() &&
						CanMovePos(to_object, to_object_next, rotation, Flags::StepOn))
					{
						to_object.SetRotation(rotation);

						to_object.SetMove(rotation);
						_brick.SetMove(rotation);

						to_object.enableFixSpeed(pushSpeed);
						_brick.enableFixSpeed(pushSpeed);

						to_object.doMove(Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(rotation)], ObjectID::Space);
						to_object.action = Brick::LINKED;
						_brick.doMove(Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(rotation)], _brick.GetObjectIDremain());
						_brick.scene->murphyMoved(&_brick);

						entity_data._effect_type = EFFECTS::PUSH;
						return true;
					}
				}
				else
				{
					entity_data._effect_type = EFFECTS::NONE;
					_brick.requests.draw = true;
				}
			}
			return false;
		}
		bool ControllButton(Brick &_brick)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			const std::array<const KeyboardController::STATE *, 4> move_controllers{&entity_data.controller->actionUp, &entity_data.controller->actionRight, &entity_data.controller->actionDown, &entity_data.controller->actionLeft};
			for (Type::Direction direction = 0; direction < 4; ++direction)
			{
				if (*move_controllers[direction])
				{
					Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);
					Type::Coord to = _brick.GetCoord(direction);

					if (_brick.scene->GetUnionFlags(to) & Flags::ButtonPush)
					{
						if (_brick.GetObject(to).id == ObjectID::Terminal)
						{
							Terminal_028::Pushed(_brick.GetObject(to));
						}
						_brick.SetRotation(rotation);
						entity_data._effect_timer = SuckEffectTime;
						entity_data._effect_type = EFFECTS::SUCK | EFFECTS::EFFECT_TIMER;
						_brick.requests.draw = true;
						return true;
					}
				}
			}
			return false;
		}
		bool ControllExit(Brick &_brick)
		{
			if (_brick.scene->getScoreToCollect() == 0)
			{
				EntityData &entity_data = _brick.entity_data.murphy;

				const std::array<const KeyboardController::STATE *, 4> move_controllers{&entity_data.controller->actionUp, &entity_data.controller->actionRight, &entity_data.controller->actionDown, &entity_data.controller->actionLeft};
				for (Type::Direction direction = 0; direction < 4; ++direction)
				{
					if (*move_controllers[direction])
					{
						Type::Coord to = _brick.GetCoord(direction);

						if (_brick.GetObject(to).id == ObjectID::Exit)
						{
							Type::Coord coord = _brick.GetCoord();

							_brick.scene->murphyVictory();
							_brick.scene->ObjectPut(coord, ObjectID::MurphyCrawlTail);
							createCrawlExit(_brick, direction);
							return true;
						}
					}
				}
			}
			return false;
		}
		bool ControllCrawl(Brick &_brick)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			const std::array<const KeyboardController::STATE *, 4> move_controllers{&entity_data.controller->actionUp, &entity_data.controller->actionRight, &entity_data.controller->actionDown, &entity_data.controller->actionLeft};
			static constexpr std::array<Type::Flags, 4> move_crawl_flags{Flags::PassageFromBottom, Flags::PassageFromLeft, Flags::PassageFromTop, Flags::PassageFromRight};

			for (Type::Direction direction = 0; direction < 4; ++direction)
			{
				if (*move_controllers[direction])
				{
					Type::Coord next = _brick.GetCoord(direction);
					Brick &next_object = _brick.scene->GetObject(next);

					if (next_object.GetFlags() & move_crawl_flags[direction])
					{
						Type::Coord to = next_object.GetCoord(direction);

						if (_brick.scene->GetSectionFlags(to) & Flags::StepOn)
						{
							Brick &to_object = _brick.scene->GetObject(to);
							Type::Coord coord = _brick.GetCoord();

							Type::Rotation rotation = Type::Rotations::getRotationOfIndex(direction);
							Eat(_brick, to_object, _brick.scene->GetRemain(to));

							if (next_object.GetFlags() & Flags::SwapsGravity)
							{
								_brick.scene->switchGravity();
							}

							_brick.SetMoveSpeed({passageSpeed,passageSpeed});
							_brick.doMoveEx(Brick::ACTION_MOVE[Type::Rotations::getIndexOfRotation(rotation)],
											ObjectID::MurphyCrawlTail,
											to,
											{Type::Move::base(coord.x() - to.x()), Type::Move::base(coord.y() - to.y())}
							);
							_brick.scene->murphyMoved(&_brick);
							_brick.setRoundDrawCoord();
							_brick.SetRotation(rotation);
							entity_data._effect_type = EFFECTS::CRAWL;
							Brick *tail = _brick.scene->GetObjectU(_brick.scene->GetComefrom(_brick.GetCoord()));
							createCrawlTail(*tail, _brick);
							DRAW_NUMBER_DESC_INIT(entity_data.draw_number_, _brick, PassOutSlides[_brick.getMoveDirection()]);
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
			EntityData &entity_data = _brick.entity_data.murphy;

			_brick.SetFlags(Flags::CanBeExploded | Flags::CanBeKilled | Flags::ExplosionType3);
			_brick.disablePhysics();

			_brick.events.timer = false;
			_brick.events.update = false;

			_brick.SetTranslationID(ObjectID::Infotron);
			_brick.SetObjectIDremain(ObjectID::Space);

			entity_data.draw_number_ = 0;
			entity_data.controller = nullptr;
			entity_data._effect_type = EFFECTS::NONE;
		}

		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			Json json;

			json["draw_number"] = entity_data.draw_number_;
			json["effect_timer"] = entity_data._effect_timer;
			json["effect_type"] = entity_data._effect_type;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			if (entity_data._effect_type & EFFECTS::EFFECT_TIMER)
			{
				entity_data._effect_timer -= CA;
				if (entity_data._effect_timer <= 0)
				{
					if (entity_data._effect_type & EFFECTS::PLACE)
					{
						TimeUpPlaceExplosive(_brick);
					}
					else if (entity_data._effect_type & EFFECTS::PUSH_TRY)
					{
						entity_data._effect_type = EFFECTS::PUSH;
					}
					else
					{
						entity_data._effect_type = EFFECTS::NONE;
					}
					_brick.requests.draw = true;
				}
			}

			if (_brick.isActionMove())
			{
				if (entity_data._effect_type == EFFECTS::CRAWL)
				{
					Brick *tail = _brick.scene->GetObjectU(_brick.scene->GetComefrom(_brick.GetCoord()));
					_brick.Step();
					timerCrawlTail(*tail, _brick);

					_brick.setRoundDrawCoord();
					DRAW_NUMBER_DESC(_brick.GetAbsMove(), 2.f, entity_data.draw_number_, _brick, PassOutSlides[_brick.getMoveDirection()]);
				}
				else if (entity_data._effect_type == EFFECTS::PUSH)
				{
					if (_brick.scene->IsObjectOut(_brick.GetCoord()))
					{
						Brick &push_object = _brick.GetObjectOut(_brick.GetCoord());
						push_object.Step();
						if (!push_object.IsMove())
						{
							push_object.disableFixSpeed();
						}
						push_object.requests.timer = true;
					}
					_brick.Step();
					if (!_brick.IsMove())
					{
						_brick.disableFixSpeed();
						entity_data._effect_type = EFFECTS::NONE;
					}
				}
				else
				{
					_brick.Step();
					DRAW_NUMBER_ASC(_brick.GetAbsMove(), 1.f, entity_data.draw_number_, _brick, MovingSlides[_brick.getMoveDirection()]);
				}
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			if (entity_data.controller->actionDestroy)
			{
				_brick.scene->blowup(_brick);
				return;
			}
			if (!_brick.IsMove())
			{
				if (entity_data.controller->actionSpecial)
				{
					if (ControllFallDown(_brick))
					{
						return;
					}

					if (_brick.isAction())
					{
						entity_data._effect_type = EFFECTS::NONE;
						_brick.finishMove();
					}

					if (ControllSuck(_brick))
					{
						return;
					}
					else if (ControllPlaceExplosive(_brick))
					{
						return;
					}
				}
				else
				{
					if (ControllMove(_brick))
					{
						return;
					}
					else if (ControllCrawl(_brick))
					{
						return;
					}
					else if (ControllFallDown(_brick))
					{
						return;
					}
					else if (ControllPush(_brick))
					{
						return;
					}
					else if (ControllButton(_brick))
					{

					}
					else if (ControllExit(_brick))
					{
						return;
					}

					if (_brick.isAction() || entity_data._effect_type & (EFFECTS::PLACE | EFFECTS::PLACE_FACE))
					{
						if (!(entity_data._effect_type & EFFECTS::PUSH))
						{
							entity_data._effect_type = EFFECTS::NONE;
						}
						_brick.finishMove();
						_brick.requests.draw = true;
					}
				}

			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData &entity_data = _brick.entity_data.murphy;

			if (_brick.IsMove())
			{
				if (entity_data._effect_type & EFFECTS::CRAWL)
				{
					PassOutSlides[Type::Rotations::getIndexOfRotation(_brick.GetRotation())][entity_data.draw_number_].drawScaled(x, y, w, h);
				}
				else if (entity_data._effect_type & EFFECTS::PUSH_TRY)
				{
					PushSlides[_brick.getMoveDirection()].drawScaled(x, y, w, h);
				}
				else if (entity_data._effect_type & EFFECTS::FALL)
				{
					MovingSlides[(_brick.getMoveDirection() + 2) % 4][entity_data.draw_number_].drawScaled(x, y, w, h);
				}
				else
				{
					MovingSlides[_brick.getMoveDirection()][entity_data.draw_number_].drawScaled(x, y, w, h);
				}
			}
			else
			{
				if (entity_data._effect_type & EFFECTS::SUCK)
				{
					SuckSlides[Type::Rotations::getIndexOfRotation(_brick.GetRotation())].drawScaled(x, y, w, h);
				}
				else if (entity_data._effect_type & EFFECTS::PLACE_FACE)
				{
					PlaceBase.drawScaled(x, y, w, h);
				}
				else if (entity_data._effect_type & EFFECTS::PUSH_TRY)
				{
					PushSlides[Type::Rotations::getIndexOfRotation(_brick.GetRotation())].drawScaled(x, y, w, h);
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
