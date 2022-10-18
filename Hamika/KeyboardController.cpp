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



KeyboardLoopReplay::KeyboardLoopReplay(LoopControllerInterface &loopControllerInterface, Logger &logger):
	KeyboardController(loopControllerInterface)
{
	auto &rows_ = logger.getLogs(LogID::_StandardKeyboard);
	rows.resize(rows_.size());

	for (size_t i = 0; i < rows_.size(); ++i)
	{
		rows[i] = static_cast<StandardKeyboardLog *>(rows_[i]);
	}

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
			if ((*it)->loopCounter < loopControllerInterface.loopCounter)
			{
				++it;
				clog << KIR4::LRED << "RngReplay loop was jumped!" << KIR4::eol;
				continue;
			}
			else if ((*it)->loopCounter == loopControllerInterface.loopCounter)
			{
				if (std::get<StandardKeyboardLog_::_actionKeyIndex >((*it)->data) == StandardKeyboardLog_::ACTION_DESTROY_UPDATE)
				{
					actionDestroy = std::get<StandardKeyboardLog_::_actionStateIndex >((*it)->data);
				}
				else if (std::get<StandardKeyboardLog_::_actionKeyIndex >((*it)->data) == StandardKeyboardLog_::ACTION_SPECIAL_UPDATE)
				{
					actionSpecial = std::get<StandardKeyboardLog_::_actionStateIndex >((*it)->data);
				}
				else if (std::get<StandardKeyboardLog_::_actionKeyIndex >((*it)->data) == StandardKeyboardLog_::ACTION_UP_UPDATE)
				{
					actionUp = std::get<StandardKeyboardLog_::_actionStateIndex >((*it)->data);
				}
				else if (std::get<StandardKeyboardLog_::_actionKeyIndex >((*it)->data) == StandardKeyboardLog_::ACTION_DOWN_UPDATE)
				{
					actionDown = std::get<StandardKeyboardLog_::_actionStateIndex >((*it)->data);
				}
				else if (std::get<StandardKeyboardLog_::_actionKeyIndex >((*it)->data) == StandardKeyboardLog_::ACTION_LEFT_UPDATE)
				{
					actionLeft = std::get<StandardKeyboardLog_::_actionStateIndex >((*it)->data);
				}
				else if (std::get<StandardKeyboardLog_::_actionKeyIndex >((*it)->data) == StandardKeyboardLog_::ACTION_RIGHT_UPDATE)
				{
					actionRight = std::get<StandardKeyboardLog_::_actionStateIndex >((*it)->data);
				}
				++it;
				continue;
			}
		}
		break;
	}
}