#pragma once

#include "Object.h"

#include <functional>

namespace Object
{
	constexpr float ACTION_TIMER_START = -1.0f;
	constexpr bool ACTION_TIMER(float &timer,
								const float &duration,
								Brick *obj,
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
				obj->requests.timer = true;
				return timerRunning();
			}
		}
		else
		{
			if (triggerEvent())
			{
				timer = duration;
				obj->requests.timer = true;
				return timerStarted();
			}
		}
		return false;
	}

	typedef std::uint8_t DRAW_NUMBER_T;

	inline void DRAW_NUMBER_INVALIDATE(DRAW_NUMBER_T &_draw_number)
	{
		_draw_number = std::numeric_limits<DRAW_NUMBER_T>::max();
	}
	inline void DRAW_NUMBER_ASC_INIT(DRAW_NUMBER_T &_draw_number, Brick *obj, const Res::Slides &bmp)
	{
		_draw_number = 0;
		obj->requests.draw = true;
	}
	inline void DRAW_NUMBER_ASC(float timer, const float duration, DRAW_NUMBER_T &_draw_number, Brick *obj, const Res::Slides &bmp)
	{
		DRAW_NUMBER_T draw_number = static_cast<DRAW_NUMBER_T>(bmp.getDrawNumber(1 - (timer / duration)));
		if (_draw_number != draw_number)
		{
			obj->requests.draw = true;
			_draw_number = draw_number;
		}
	}
	inline void DRAW_NUMBER_DESC_INIT(DRAW_NUMBER_T &_draw_number, Brick *obj, const Res::Slides &bmp)
	{
		_draw_number = bmp.getCount() - 1;
		obj->requests.draw = true;
	}
	inline void DRAW_NUMBER_DESC(float timer, const float duration, DRAW_NUMBER_T &_draw_number, Brick *obj, const Res::Slides &bmp)
	{
		DRAW_NUMBER_T draw_number = static_cast<DRAW_NUMBER_T>(bmp.getDrawNumber(timer / duration));
		if (_draw_number != draw_number)
		{
			obj->requests.draw = true;
			_draw_number = draw_number;
		}
	}

	namespace Animator
	{
		struct EntityData
		{
			std::float_t time;
			std::float_t timer;
			std::int8_t numberOfFrames;
			DRAW_NUMBER_T draw_number_;

			void Initialize();
			void SetAnimationTime(std::float_t AnimationTime);
			void SetNumberOfFrames(std::int8_t FramesNumber);
			std::int8_t GetDrawNumber();
			bool UpdateDrawNumber();
			void UpdateTimer();
		};

		void Create(Object::Brick &_brick, EntityData &_entity_data);
		OBJECT_PRINTER_RET Print(Object::Brick &_brick, EntityData &_entity_data);
		void Timer(Object::Brick &_brick, EntityData &_entity_data);
		void Update(Object::Brick &_brick, EntityData &_entity_data, Object::Brick::UpdateType _updateType);
	}

	namespace Fall
	{
		struct EntityData
		{
			bool heavy_object_;

			void setHeavy(bool _heavy_object);
		};

		void Create(Object::Brick &_brick, EntityData &_entity_data);
		OBJECT_PRINTER_RET Print(Object::Brick &_brick, EntityData &_entity_data);
		void Timer(Object::Brick &_brick, EntityData &_entity_data);
		void Update(Object::Brick &_brick, EntityData &_entity_data, Object::Brick::UpdateType _updateType);
	}
	namespace FallAndRoll
	{
		struct EntityData: public Fall::EntityData
		{
			std::int8_t roll_preference_;
		};

		void Create(Object::Brick &_brick, EntityData &_entity_data);
		OBJECT_PRINTER_RET Print(Object::Brick &_brick, EntityData &_entity_data);
		void Timer(Object::Brick &_brick, EntityData &_entity_data);
		void Update(Object::Brick &_brick, EntityData &_entity_data, Object::Brick::UpdateType _updateType);
	}

	namespace MoveLeftWay
	{
		void Blasting(Type::Coord coord, Brick *o);
		bool CanMoveForward(Type::Coord to, Brick *o);
		bool CanExlosive(Type::Coord coord, Brick *o);
		bool CanTurnLeft(Brick *o);

		struct EntityData
		{
			bool PriorityStep;
		};

		void Create(Object::Brick &_brick, EntityData &_entity_data);
		OBJECT_PRINTER_RET Print(Object::Brick &_brick, EntityData &_entity_data);
		void Timer(Object::Brick &_brick, EntityData &_entity_data);
		void Update(Object::Brick &_brick, EntityData &_entity_data, Object::Brick::UpdateType _updateType);
	}
}