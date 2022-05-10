#pragma once

#include "Object.h"

#include <functional>

namespace Object
{
	constexpr float ACTION_TIMER_START = -1.0f;
	inline bool ACTION_TIMER(float &timer,
							 const float &duration,
							 ObjectBase *obj,
							 std::function<bool()> triggerEvent,
							 std::function<bool()> timerStarted,
							 std::function<bool()> timerRunning,
							 std::function<bool()> timerExpired)
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

	inline void DRAW_NUMBER(float timer, const float duration, int &drawNumber, ObjectBase *obj, const Slides &bmp)
	{
		int drawNumber_ = bmp.getDrawNumber(1 - (timer / duration));
		if (drawNumber != drawNumber_)
		{
			obj->requests.draw=true;
			drawNumber = drawNumber_;
		}
	}

	inline void DRAW_NUMBER_R(float timer, const float duration, int &drawNumber, ObjectBase *obj, const Slides &bmp)
	{
		int drawNumber_ = bmp.getDrawNumber(timer / duration);
		if (drawNumber != drawNumber_)
		{
			obj->requests.draw=true;
			drawNumber = drawNumber_;
		}
	}

	namespace Animator
	{
		struct Specific
		{
			float time;
			float timer;
			int numberOfFrames;
			int drawNumber;

			void SetAnimationTime(float AnimationTime);
			void SetNumberOfFrames(int FramesNumber);
			int GetDrawNumber();
			bool UpdateDrawNumber();
		};


		void Create(OBJECT_CREATER_PARAM);
		void Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Tick(OBJECT_TICK_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
	}


	namespace MoveDown
	{
		struct Specific
		{
			int active;
		};

		void Create(OBJECT_CREATER_PARAM);
		void Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Tick(OBJECT_TICK_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
	}

	namespace MoveDownHeavy
	{
		void Blasting(ObjectBase *o);

		void Create(OBJECT_CREATER_PARAM);
		void Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Tick(OBJECT_TICK_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
	}

	namespace MoveLeftWay
	{
		void Blasting(Type::Coord coord, ObjectBase *o);
		bool CanMoveForward(Type::Coord to, ObjectBase *o);
		bool CanExlosive(Type::Coord coord, ObjectBase *o);
		bool CanTurnLeft(ObjectBase *o);

		enum TypeActive
		{
			F_Step = 1,
			F_TurnLeft,
			F_TurnRight,
		};

		struct Specific
		{
			int active;
			bool PriorityStep;
			int myNumber;
		};

		void Create(OBJECT_CREATER_PARAM);
		void Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Tick(OBJECT_TICK_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
	}

	namespace RollDown
	{
		struct Specific
		{
			int active;
			int rollCounter;
		};

		bool CanRollAffect(ObjectBase *o, Type::Coord coord1);
		bool CanRoll(ObjectBase *o, Type::Coord coord1, Type::Coord coord2);
		bool CanRollOff(ObjectBase *o);

		void Create(OBJECT_CREATER_PARAM);
		void Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Tick(OBJECT_TICK_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
	}
}