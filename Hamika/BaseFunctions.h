#pragma once

#include "Types.h"
#include "Bitmap.h"

#include <functional>

namespace Object
{
	struct Brick;
}

#define OBJECT_CREATER_PARAM Object::Brick &_brick
#define OBJECT_CREATER_CALL _brick

#define OBJECT_INITIALIZER_PARAM

#define OBJECT_UPDATE_PARAM Object::Brick &_brick, UpdateType updateType
#define OBJECT_UPDATE_CALL _brick, updateType

#define OBJECT_FINALIZE_PARAM Object::Brick &_brick
#define OBJECT_FINALIZE_CALL _brick

#define OBJECT_TIMER_PARAM Object::Brick &_brick
#define OBJECT_TIMER_CALL _brick

#define OBJECT_PRINTER_PARAM Object::Brick &_brick
#define OBJECT_PRINTER_CALL _brick
#define OBJECT_PRINTER_RET Json

#define OBJECT_DRAWNER_PARAM Object::Brick &_brick, Type::Coord::base x,Type::Coord::base y,Type::Coord::base w,Type::Coord::base h
#define OBJECT_DRAWNER_CALL _brick, x, y, w, h

#define OBJECT_SIMPLE_DRAWNER_PARAM Type::Coord::base x,Type::Coord::base y,Type::Coord::base w,Type::Coord::base h
#define OBJECT_SIMPLE_DRAWNER_CALL x, y, w, h

namespace Object
{
	constexpr float ACTION_TIMER_START = -1.0f;
	bool ACTION_TIMER(float &timer,
					  const float &duration,
					  Brick &_brick,
					  const std::function<bool()> &triggerEvent,
					  const std::function<bool()> &timerStarted,
					  const std::function<bool()> &timerRunning,
					  const std::function<bool()> &timerExpired);

	void DRAW_NUMBER_INVALIDATE(DrawNumber &_draw_number);
	void DRAW_NUMBER_ASC_INIT(DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp);
	void DRAW_NUMBER_ASC(float timer, const float duration, DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp);
	void DRAW_NUMBER_DESC_INIT(DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp);
	void DRAW_NUMBER_DESC(float timer, const float duration, DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp);

	namespace Animator
	{
		struct EntityData
		{
			std::float_t time;
			std::float_t timer;
			std::int8_t numberOfFrames;
			DrawNumber draw_number_;

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
		void Update(Object::Brick &_brick, EntityData &_entity_data, UpdateType _updateType);
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
		void Update(Object::Brick &_brick, EntityData &_entity_data, UpdateType _updateType);
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
		void Update(Object::Brick &_brick, EntityData &_entity_data, UpdateType _updateType);
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
		void Update(Object::Brick &_brick, EntityData &_entity_data, UpdateType _updateType);
	}
}