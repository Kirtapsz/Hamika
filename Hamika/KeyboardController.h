#pragma once

#include "Logger.h"

#include <KIR/KIR4_console.h>

struct StandardKeyboardLog_
{
	static constexpr size_t _actionKeyIndex = 0;
	static constexpr size_t _actionStateIndex = 1;

	enum ACTION_KEY: unsigned __int8
	{
		ACTION_DESTROY_UPDATE = 1,
		ACTION_SPECIAL_UPDATE = 2,
		ACTION_UP_UPDATE = 3,
		ACTION_DOWN_UPDATE = 4,
		ACTION_LEFT_UPDATE = 5,
		ACTION_RIGHT_UPDATE = 6,
	};

	typedef std::tuple<ACTION_KEY, bool> Type;
};
typedef LogBaseData<LogID::_StandardKeyboard, StandardKeyboardLog_::Type> StandardKeyboardLog;


struct KeyboardController
{
	LoopControllerInterface &loopControllerInterface;

	bool actionDestroy;
	bool actionSpecial;
	bool actionUp;
	bool actionDown;
	bool actionLeft;
	bool actionRight;

	KeyboardController(LoopControllerInterface &loopControllerInterface);

	virtual void keyUp(int key) = 0;
	virtual void keyDown(int key) = 0;
	virtual void loop() = 0;
};

template <typename L, typename KEYBOARD_CONTROLLER>
struct KeyboardRecord: KEYBOARD_CONTROLLER
{
	Logger &logger;

	bool actionDestroyPrev;
	bool actionSpecialPrev;
	bool actionUpPrev;
	bool actionDownPrev;
	bool actionLeftPrev;
	bool actionRightPrev;

	KeyboardRecord(LoopControllerInterface &loopControllerInterface, Logger &logger);

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
struct StandardKeyboardRecord: KeyboardRecord<StandardKeyboardLog, StandardKeyboard>
{
	inline StandardKeyboardRecord(LoopControllerInterface &loopControllerInterface, Logger &logger):
		KeyboardRecord<StandardKeyboardLog, StandardKeyboard>(loopControllerInterface, logger)
	{

	}
};


struct KeyboardLoopReplay: KeyboardController
{
	std::vector<StandardKeyboardLog *> rows;
	std::vector<StandardKeyboardLog *>::iterator it;

	KeyboardLoopReplay(LoopControllerInterface &loopControllerInterface, Logger &logger);

	virtual void keyUp(int key);
	virtual void keyDown(int key);
	virtual void loop();
};





template <typename L, typename KEYBOARD_CONTROLLER>
KeyboardRecord<L, KEYBOARD_CONTROLLER>::KeyboardRecord(LoopControllerInterface &loopControllerInterface, Logger &logger):
	KEYBOARD_CONTROLLER(loopControllerInterface),
	logger(logger),
	actionSpecialPrev(false), actionUpPrev(false), actionDownPrev(false), actionLeftPrev(false), actionRightPrev(false), actionDestroyPrev(false)
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
	if (actionDestroyPrev != actionDestroy)
	{
		actionDestroyPrev = actionDestroy;
		logger.record<L>(StandardKeyboardLog_::ACTION_DESTROY_UPDATE, actionDestroy);
	}
	if (actionSpecialPrev != actionSpecial)
	{
		actionSpecialPrev = actionSpecial;
		logger.record<L>(StandardKeyboardLog_::ACTION_SPECIAL_UPDATE, actionSpecial);
	}
	if (actionUpPrev != actionUp)
	{
		actionUpPrev = actionUp;
		logger.record<L>(StandardKeyboardLog_::ACTION_UP_UPDATE, actionUp);
	}
	if (actionDownPrev != actionDown)
	{
		actionDownPrev = actionDown;
		logger.record<L>(StandardKeyboardLog_::ACTION_DOWN_UPDATE, actionDown);
	}
	if (actionLeftPrev != actionLeft)
	{
		actionLeftPrev = actionLeft;
		logger.record<L>(StandardKeyboardLog_::ACTION_LEFT_UPDATE, actionLeft);
	}
	if (actionRightPrev != actionRight)
	{
		actionRightPrev = actionRight;
		logger.record<L>(StandardKeyboardLog_::ACTION_RIGHT_UPDATE, actionRight);
	}
}
