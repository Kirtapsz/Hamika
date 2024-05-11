#include "BaseFunctions.h"
#include "Object.h"

namespace Object
{
	constexpr float ACTION_TIMER_START = -1.0f;
	constexpr bool ACTION_TIMER(float &timer,
								const float &duration,
								Brick &_brick,
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
				_brick.requests.timer = true;
				return timerRunning();
			}
		}
		else
		{
			if (triggerEvent())
			{
				timer = duration;
				_brick.requests.timer = true;
				return timerStarted();
			}
		}
		return false;
	}

	inline void DRAW_NUMBER_INVALIDATE(DrawNumber &_draw_number)
	{
		_draw_number = std::numeric_limits<DrawNumber>::max();
	}
	inline void DRAW_NUMBER_ASC_INIT(DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp)
	{
		_draw_number = 0;
		_brick.requests.draw = true;
	}
	inline void DRAW_NUMBER_ASC(float timer, const float duration, DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp)
	{
		DrawNumber draw_number = static_cast<DrawNumber>(bmp.getDrawNumber(1 - (timer / duration)));
		if (_draw_number != draw_number)
		{
			_brick.requests.draw = true;
			_draw_number = draw_number;
		}
	}
	inline void DRAW_NUMBER_DESC_INIT(DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp)
	{
		_draw_number = bmp.getCount() - 1;
		_brick.requests.draw = true;
	}
	inline void DRAW_NUMBER_DESC(float timer, const float duration, DrawNumber &_draw_number, Brick &_brick, const Res::Slides &bmp)
	{
		DrawNumber draw_number = static_cast<DrawNumber>(bmp.getDrawNumber(timer / duration));
		if (_draw_number != draw_number)
		{
			_brick.requests.draw = true;
			_draw_number = draw_number;
		}
	}
}