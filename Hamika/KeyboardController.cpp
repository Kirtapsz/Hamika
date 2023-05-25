#include "KeyboardController.h"

#include <KIR\AL\KIR5_event_engine.h>
#include <KIR/KIR4_console.h>

#include <algorithm>


KeyboardController::KeyboardController(LoopControllerInterface &loopControllerInterface):
	loopControllerInterface(loopControllerInterface),
	actionSpecial(false), actionUp(false), actionDown(false), actionLeft(false), actionRight(false), actionDestroy(false)
{

}


void StandardKeyboard::keyAction(int key, bool state)
{
	if (key == ALLEGRO_KEY_UP)
	{
		actionUp = state;
	}
	else if (key == ALLEGRO_KEY_DOWN)
	{
		actionDown = state;
	}
	else if (key == ALLEGRO_KEY_LEFT)
	{
		actionLeft = state;
	}
	else if (key == ALLEGRO_KEY_RIGHT)
	{
		actionRight = state;
	}
	else if (key == ALLEGRO_KEY_SPACE)
	{
		actionSpecial = state;
	}
	else if (key == ALLEGRO_KEY_ESCAPE)
	{
		if (state)
		{
			actionDestroy = true;
		}
	}
}
void StandardKeyboard::keyUp(int key)
{
	keyAction(key, false);
}
void StandardKeyboard::keyDown(int key)
{
	keyAction(key, true);
}
void StandardKeyboard::loop()
{

}



KeyboardLoopReplay::KeyboardLoopReplay(LoopControllerInterface &loopControllerInterface, Res::Logger &logger):
	KeyboardController(loopControllerInterface)
{
	rows = logger.getLogs<Res::Log::StandardKeyboard>();
	it = rows.begin();
}
void KeyboardLoopReplay::keyUp(int key)
{

}
void KeyboardLoopReplay::keyDown(int key)
{

}
void KeyboardLoopReplay::loop()
{
	while (true)
	{
		if (it != rows.end())
		{
			auto &loopCounter = std::get<Res::Log::StandardKeyboard::loopCounter>(**it);
			if (loopCounter < loopControllerInterface.loopCounter)
			{
				++it;
				clog << KIR4::LRED << "KeyboardLoopReplay loop was jumped!" << KIR4::eol;
				continue;
			}
			else if (loopCounter == loopControllerInterface.loopCounter)
			{
				auto &_actionKey = std::get<Res::Log::StandardKeyboard::actionKey>(**it);
				auto &_actionState = std::get<Res::Log::StandardKeyboard::actionState>(**it);
				if (_actionKey == Res::Log::StandardKeyboard::ACTION_DESTROY_UPDATE)
				{
					actionDestroy = _actionState;
				}
				else if (_actionKey == Res::Log::StandardKeyboard::ACTION_SPECIAL_UPDATE)
				{
					actionSpecial = _actionState;
				}
				else if (_actionKey == Res::Log::StandardKeyboard::ACTION_UP_UPDATE)
				{
					actionUp = _actionState;
				}
				else if (_actionKey == Res::Log::StandardKeyboard::ACTION_DOWN_UPDATE)
				{
					actionDown = _actionState;
				}
				else if (_actionKey == Res::Log::StandardKeyboard::ACTION_LEFT_UPDATE)
				{
					actionLeft = _actionState;
				}
				else if (_actionKey == Res::Log::StandardKeyboard::ACTION_RIGHT_UPDATE)
				{
					actionRight = _actionState;
				}
				++it;
				continue;
			}
		}
		break;
	}
}