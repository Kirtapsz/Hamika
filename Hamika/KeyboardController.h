#pragma once

#include "Logger.h"

#include <KIR/KIR4_console.h>

namespace Res::Log
{
	struct StandardKeyboard: Base, KIR5::StreamRecords::Record<
		std::uint32_t,
		std::uint32_t,
		std::uint8_t,
		bool
	>
	{
		enum ACTION_KEY: std::uint8_t
		{
			ACTION_DESTROY_UPDATE = 1,
			ACTION_SPECIAL_UPDATE = 2,
			ACTION_UP_UPDATE = 3,
			ACTION_DOWN_UPDATE = 4,
			ACTION_LEFT_UPDATE = 5,
			ACTION_RIGHT_UPDATE = 6,
		};

		constexpr static std::size_t loopCounter = 0;
		constexpr static std::size_t currentTimeMS = 1;
		static constexpr size_t actionKey = 2;
		static constexpr size_t actionState = 3;

		constexpr static std::array<const char *, 4> keys{{"LoopCounter", "CurrentTimeMS", "ActionKey", "ActionState"}};

		inline StandardKeyboard()
		{

		}
		inline StandardKeyboard(std::uint32_t loopCounter_, std::uint32_t currentTimeMS_, std::uint8_t actionKey_, bool actionState_)
		{
			std::get<loopCounter>(*this) = loopCounter_;
			std::get<currentTimeMS>(*this) = currentTimeMS_;
			std::get<actionKey>(*this) = actionKey_;
			std::get<actionState>(*this) = actionState_;
		}
	};
}


struct KeyboardController
{
	LoopControllerInterface &loopControllerInterface;

	struct STATE
	{
		static constexpr std::uint8_t RELEASED = 1 << 0;
		static constexpr std::uint8_t LOCKED = 1 << 1;
		static constexpr std::uint8_t UPDATED = 1 << 2;

		std::uint8_t state_ = RELEASED;

		inline constexpr void operator=(bool _state)
		{
			state_ = (_state ? LOCKED : RELEASED) | UPDATED;
		}
		inline constexpr bool isUpdated() const
		{
			return state_ & UPDATED;
		}
		inline constexpr bool isLocked() const
		{
			return state_ & LOCKED;
		}
		inline constexpr bool isReleased() const
		{
			return state_ & RELEASED;
		}
		inline constexpr void clearUpdate()
		{
			state_ &= ~UPDATED;
		}
		inline constexpr operator bool() const
		{
			return isLocked();
		}
	};

	STATE actionDestroy;
	STATE actionSpecial;
	STATE actionUp;
	STATE actionDown;
	STATE actionLeft;
	STATE actionRight;

	KeyboardController(LoopControllerInterface &loopControllerInterface);

	virtual void keyUp(int key) = 0;
	virtual void keyDown(int key) = 0;
	virtual void loop() = 0;
	void finalize();
};

template <typename L, typename KEYBOARD_CONTROLLER>
struct KeyboardRecord: KEYBOARD_CONTROLLER
{
	Res::Logger &logger;

	KeyboardRecord(LoopControllerInterface &loopControllerInterface, Res::Logger &logger);

	virtual void keyUp(int key);
	virtual void keyDown(int key);
	virtual void loop();
};


struct StandardKeyboard: KeyboardController
{
	using KeyboardController::KeyboardController;
	void keyAction(int key, bool state);
	virtual void keyUp(int key);
	virtual void keyDown(int key);
	virtual void loop();
};
struct StandardKeyboardRecord: KeyboardRecord<Res::Log::StandardKeyboard, StandardKeyboard>
{
	inline StandardKeyboardRecord(LoopControllerInterface &loopControllerInterface, Res::Logger &logger):
		KeyboardRecord<Res::Log::StandardKeyboard, StandardKeyboard>(loopControllerInterface, logger)
	{

	}
};


struct KeyboardLoopReplay: KeyboardController
{
	std::vector<Res::Log::StandardKeyboard *> rows;
	std::vector<Res::Log::StandardKeyboard *>::iterator it;

	KeyboardLoopReplay(LoopControllerInterface &loopControllerInterface, Res::Logger &logger);

	virtual void keyUp(int key);
	virtual void keyDown(int key);
	virtual void loop();
};





template <typename L, typename KEYBOARD_CONTROLLER>
KeyboardRecord<L, KEYBOARD_CONTROLLER>::KeyboardRecord(LoopControllerInterface &loopControllerInterface, Res::Logger &logger):
	KEYBOARD_CONTROLLER(loopControllerInterface),
	logger(logger)
{

}

template <typename L, typename KEYBOARD_CONTROLLER>
void KeyboardRecord<L, KEYBOARD_CONTROLLER>::keyUp(int key)
{
	KEYBOARD_CONTROLLER::keyUp(key);
}
template <typename L, typename KEYBOARD_CONTROLLER>
void KeyboardRecord<L, KEYBOARD_CONTROLLER>::keyDown(int key)
{
	KEYBOARD_CONTROLLER::keyDown(key);
}
template <typename L, typename KEYBOARD_CONTROLLER>
void KeyboardRecord<L, KEYBOARD_CONTROLLER>::loop()
{
	KEYBOARD_CONTROLLER::loop();
	if (actionDestroy.isUpdated())
	{
		logger.record<L>(Res::Log::StandardKeyboard::ACTION_DESTROY_UPDATE, actionDestroy);
	}
	if (actionSpecial.isUpdated())
	{
		logger.record<L>(Res::Log::StandardKeyboard::ACTION_SPECIAL_UPDATE, actionSpecial);
	}
	if (actionUp.isUpdated())
	{
		logger.record<L>(Res::Log::StandardKeyboard::ACTION_UP_UPDATE, actionUp);
	}
	if (actionDown.isUpdated())
	{
		logger.record<L>(Res::Log::StandardKeyboard::ACTION_DOWN_UPDATE, actionDown);
	}
	if (actionLeft.isUpdated())
	{
		logger.record<L>(Res::Log::StandardKeyboard::ACTION_LEFT_UPDATE, actionLeft);
	}
	if (actionRight.isUpdated())
	{
		logger.record<L>(Res::Log::StandardKeyboard::ACTION_RIGHT_UPDATE, actionRight);
	}
}
