#include "SceneConfiguration.h"
#include "MainEvent.h"
#include "Tools.h"
#include "Murphy.h"

#include <fstream>

#include <KIR/sys/KIR5_files.h>
#include <KIR/sys/KIR5_system.h>

namespace UI::Scene
{
	void Replay::failure(const std::string &msg)
	{
		finishGame();
	}
	Replay::Replay(const std::string &uuid_)
	{
		logger = std::shared_ptr<Res::Logger>(new Res::Logger(*this, nullptr, nullptr));
		std::string replayPath = KIR5::pathCombine(KIR5::getModuleDirectory(), "Hamika", "replays", uuid_ + ".log");
		try
		{
			logger->load(replayPath);
		}
		catch (const std::exception &e)
		{
			hide();
			failure(concatenate("Could not load replay: ", replayPath, ", ", e.what()));
			return;
		}

		if (!logger->bluePrint)
		{
			failure(concatenate("Could not find blueprint"));
			return;
		}

		keyboardController = std::shared_ptr<KeyboardController>(new KeyboardLoopReplay(*this, *logger));
		rngController = std::shared_ptr<RngController>(new RngReplay(*this, *logger));

		initialize(logger->bluePrint, nullptr);

		*activeMoved = [&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			background->move(0, 0, width(), height());
			drawnerBar->move(0, 0, width(), height());
			drawer.InitializeDrawOptions({drawnerBar->width(), drawnerBar->height()}, cameraSize);
		};

		fncMoved.push_back(activeMoved);

		al_hide_mouse_cursor(*display);

		*delayTimerStart = [&](FNC_TIMER_PARAMS) -> FNC_TIMER_RET
		{
			if (--delayLoop == 0)
			{
				drawnerBar->fncDraw.remove(delayDraw);
				fncTimer.remove(delayTimerStart);
				fncTimer.push_back(activeTimer);
			}
			else
			{
				delayAlpha = 255 * (delayLoop / (float)startDelay);
			}
		};
		delayLoop = startDelay;
		fncTimer.push_back(delayTimerStart);

		fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS) -> FNC_KEY_DOWN_RET
		{
			if (key_ == ALLEGRO_KEY_ESCAPE)
			{
				keyboardController->actionDestroy = true;
				return true;
			}
			return false;
		}
		);

		*delayTimerStop = [&](FNC_TIMER_PARAMS) -> FNC_TIMER_RET
		{
			if (--delayLoop == 0)
			{
				finishGame();
			}
			else
			{
				delayAlpha = 255 - (255 * (delayLoop / (float)stopDelay));
			}
		};

		*delayDraw = [&](FNC_DRAW_PARAMS) -> FNC_DRAW_RET
		{
			al_draw_filled_rectangle(x_, y_, x_ + w_, y_ + h_, KIR5::Color(0, 0, 0, delayAlpha).getAlphaColored());
		};

		drawnerBar->fncDraw.push_back(delayDraw);

		statusbar->hide();
	}

	void Replay::signGame()
	{
		delayLoop = stopDelay;
		fncTimer.push_back(delayTimerStop);
		drawnerBar->fncDraw.push_back(delayDraw);
	}
	void Replay::finishGame()
	{
		if (ifSync(finished, true))
		{
			al_show_mouse_cursor(*display);
			eventEngine->sendEvent((void *)ACTIVE_MAP_FINISHED, (void *)(0), (void *)0, (void *)0);
		}
	}


	Play::Play(const std::shared_ptr<Res::BluePrint> &bluePrint_, const std::shared_ptr<Res::Account> &account_)
	{
		logger = std::shared_ptr<Res::Logger>(new Res::Logger(*this, bluePrint_, account_));
		keyboardController = std::shared_ptr<KeyboardController>(new StandardKeyboardRecord(*this, *logger));
		rngController = std::shared_ptr<RngController>(new RngRecord(*this, *logger));

		initialize(bluePrint_, account_);

		fncMoved.push_back(activeMoved);
		fncKeyDown.push_back(activeKeyDown);
		fncKeyUp.push_back(activeKeyUp);

		al_hide_mouse_cursor(*display);

		*delayTimerStart = [&](FNC_TIMER_PARAMS) -> FNC_TIMER_RET
		{
			if (--delayLoop == 0)
			{
				drawnerBar->fncDraw.remove(delayDraw);
				fncTimer.remove(delayTimerStart);
				fncTimer.push_back(activeTimer);
			}
			else
			{
				delayAlpha = 255 * (delayLoop / (float)startDelay);
			}
		};
		delayLoop = startDelay;
		fncTimer.push_back(delayTimerStart);

		*delayTimerStop = [&](FNC_TIMER_PARAMS) -> FNC_TIMER_RET
		{
			if (--delayLoop == 0)
			{
				finishGame();
			}
			else
			{
				delayAlpha = 255 - (255 * (delayLoop / (float)stopDelay));
			}
		};

		*delayDraw = [&](FNC_DRAW_PARAMS) -> FNC_DRAW_RET
		{
			al_draw_filled_rectangle(x_, y_, x_ + w_, y_ + h_, KIR5::Color(0, 0, 0, delayAlpha).getAlphaColored());
		};

		drawnerBar->fncDraw.push_back(delayDraw);
	}

	void Play::signGame()
	{
		delayLoop = stopDelay;
		fncTimer.push_back(delayTimerStop);
		drawnerBar->fncDraw.push_back(delayDraw);

		_account->addPlayedTime(loopCounter * CA * 1000);

		KIR4::time date;
		logger->save(KIR5::pathCombine(KIR5::getModuleDirectory(), "Hamika", "replays", logger->_uuid + ".log"));
		if (victory)
		{
			_account->addBluePrint(_bluePrint, loopCounter * CA * 1000, date, logger->_uuid);
		}
	}
	void Play::finishGame()
	{
		if (ifSync(finished, true))
		{
			al_show_mouse_cursor(*display);
			eventEngine->sendEvent((void *)ACTIVE_MAP_FINISHED, (void *)(victory), (void *)0, (void *)0);
		}
	}
	void Play::failure(const std::string &msg)
	{

	}





	Developer::Developer(const std::shared_ptr<Res::BluePrint> &bluePrint_)
	{
		logger = std::shared_ptr<Res::Logger>(new Res::Logger(*this, bluePrint_, nullptr));
		keyboardController = std::shared_ptr<KeyboardController>(new StandardKeyboardRecord(*this, *logger));
		rngController = std::shared_ptr<RngController>(new RngRecord(*this, *logger));
		validator = std::shared_ptr<ValidatorRecord>(new ValidatorRecord(*this, *logger, *this));

		initialize(bluePrint_, nullptr);

		fncMoved.push_back(activeMoved);
		fncKeyDown.push_back(activeKeyDown);
		fncKeyUp.push_back(activeKeyUp);

		*delayTimerStart = [&](FNC_TIMER_PARAMS) -> FNC_TIMER_RET
		{
			if (--delayLoop == 0)
			{
				drawnerBar->fncDraw.remove(delayDraw);
				fncTimer.remove(delayTimerStart);
				if (!paused)
				{
					fncTimer.push_back(activeTimer);
				}
			}
			else
			{
				delayAlpha = 255 * (delayLoop / (float)startDelay);
			}
		};
		delayLoop = startDelay;
		fncTimer.push_back(delayTimerStart);

		*delayTimerStop = [&](FNC_TIMER_PARAMS) -> FNC_TIMER_RET
		{
			if (!paused)
			{
				if (--delayLoop == 0)
				{
					finishGame();
				}
				else
				{
					delayAlpha = 255 - (255 * (delayLoop / (float)stopDelay));
				}
			}
		};

		*delayDraw = [&](FNC_DRAW_PARAMS) -> FNC_DRAW_RET
		{
			al_draw_filled_rectangle(x_, y_, x_ + w_, y_ + h_, KIR5::Color(0, 0, 0, delayAlpha).getAlphaColored());
		};

		drawnerBar->fncDraw.push_back(delayDraw);


		fncKeyChar.push_back([&](FNC_KEY_CHAR_PARAMS) -> FNC_KEY_CHAR_RET
		{
			if (key_ == 'p')
			{
				if (paused)
				{
					fncTimer.push_back(activeTimer);
				}
				else
				{
					fncTimer.remove(activeTimer);
				}
				paused = !paused;
				return true;
			}
			else if (key_ == 'f')
			{
				if (paused)
				{
					(*activeTimer)(nullptr, 0.);
					return true;
				}
			}
			else if (key_ == 't')
			{
				murphyDead(murphy);
			}
			return false;
		});

		drawnerBar->fncMouseButtonDown.push_back([&](FNC_MOUSE_BUTTON_DOWN_PARAMS) -> FNC_MOUSE_BUTTON_DOWN_RET
		{
			if (eventEngine->getTargetPanel().get() == statusbar.get())
			{
				if (button_ == KIR5::MOUSE_BUTTON_LEFT)
				{
					Json json = printStatusBar();
					validator->validate(Res::Log::ObjectValidator::STATUSBAR_T, json);

					std::cout << "::: STATUSBAR :::" << std::endl;
					std::cout << std::setw(4) << std::setprecision(3) << json << std::endl;
					std::cout << "- - - - - - - - - - - - - - -" << std::endl;
					return true;
				}
			}

			return false;
		});

		drawnerBar->fncMouseButtonDown.push_back([&](FNC_MOUSE_BUTTON_DOWN_PARAMS) -> FNC_MOUSE_BUTTON_DOWN_RET
		{
			if (eventEngine->getTargetPanel().get() == drawnerBar.get())
			{
				Type::Coord coord = drawer.GetFromCursor(x_, y_);
				if (map->Test(coord))
				{
					if (button_ == KIR5::MOUSE_BUTTON_LEFT)
					{
						Json json = reach(map)[coord].object->print();
						validator->validate(Res::Log::ObjectValidator::OBJECT_T, json);

						if (reach(map)[coord].object->isExists)
						{
							std::cout << "::: OBJECT :::" << std::endl;
							std::cout << std::setw(4) << std::setprecision(3) << json << std::endl;
							std::cout << "- - - - - - - - - - - - - - -" << std::endl;
						}
						return true;
					}
					if (button_ == KIR5::MOUSE_BUTTON_RIGHT)
					{
						Json json = reach(map)[coord].remain->print();
						validator->validate(Res::Log::ObjectValidator::REMAIN_T, json);

						if (reach(map)[coord].remain->isExists)
						{
							std::cout << "::: REMAIN :::" << std::endl;
							std::cout << std::setw(4) << std::setprecision(3) << json << std::endl;
							std::cout << "- - - - - - - - - - - - - - -" << std::endl;
						}
						return true;
					}
				}
			}

			return false;
		});
	}

	void Developer::signGame()
	{
		validator->validate(Res::Log::ObjectValidator::RESULT_T, printResult());
		validator->validate(Res::Log::ObjectValidator::TERMINATE_T, Json());

		delayLoop = stopDelay;
		fncTimer.push_back(delayTimerStop);
		drawnerBar->fncDraw.push_back(delayDraw);

		KIR4::time date;
		logger->save(KIR5::pathCombine(KIR5::getModuleDirectory(), "Hamika", "replays", logger->_uuid + ".dev.log"));
		logger->save(KIR5::pathCombine(KIR5::getModuleDirectory(), "Hamika", "replays", logger->_uuid + ".dev.json"));
	}
	void Developer::finishGame()
	{
		if (ifSync(finished, true))
		{
			eventEngine->sendEvent((void *)ACTIVE_MAP_FINISHED, (void *)(victory), (void *)0, (void *)0);
		}
	}
	void Developer::failure(const std::string &msg)
	{

	}


	InteractiveMultitest::InteractiveMultitest(const std::string &replayPath, std::int8_t speed_):
		speed(speed_)
	{
		logger = std::shared_ptr<Res::Logger>(new Res::Logger(*this, nullptr, nullptr));
		try
		{
			logger->load(replayPath);
		}
		catch (const std::exception &e)
		{
			hide();
			failure(concatenate("Could not load replay: ", replayPath, ", ", e.what()));
			return;
		}

		if (!logger->bluePrint)
		{
			failure(concatenate("Could not find blueprint"));
			return;
		}

		keyboardController = std::shared_ptr<KeyboardController>(new KeyboardLoopReplay(*this, *logger));
		rngController = std::shared_ptr<RngController>(new RngValidator(*this, *logger));
		validator = std::shared_ptr<ValidatorReplay>(new ValidatorReplay(*this, *logger, *this));

		initialize(logger->bluePrint, nullptr);

		*activeMoved = [&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			background->move(0, 0, width(), height());
			drawnerBar->move(0, 0, width(), height());
			drawer.InitializeDrawOptions({drawnerBar->width(), drawnerBar->height()}, cameraSize);
		};

		fncMoved.push_back(activeMoved);

		fncTimer.push_back([&](FNC_TIMER_PARAMS)
		{
			for (std::int8_t i = 0; i < speed; ++i)
			{
				validator->validate();
				(*activeTimer)(obj_, cps_);
			}
		});

		fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS) -> FNC_KEY_DOWN_RET
		{
			if (key_ == ALLEGRO_KEY_ESCAPE)
			{
				keyboardController->actionDestroy = true;
				setResult(Validator::Interface::RESULT::FAILED);
				return true;
			}
			return false;
		}
		);

		statusbar->hide();
	}
	void InteractiveMultitest::signGame()
	{
		validator->finalize();
		finishGame();
	}
	void InteractiveMultitest::finishGame()
	{
		if (ifSync(finished, true))
		{
			deplayTimer = 2 * CPS;
			fncTimer.push_back([&](FNC_TIMER_PARAMS)
			{
				if (deplayTimer-- == 0)
				{
					eventEngine->sendEvent((void *)ACTIVE_MAP_FINISHED, (void *)(0), (void *)0, (void *)0);
				}
			});
		}
	}
	void InteractiveMultitest::failure(const std::string &msg)
	{
		clog << KIR4::LRED << " - " << msg << KIR4::eol;
		setResult(Validator::Interface::RESULT::FAILED);
		finishGame();
	}


	Multitest::Multitest(const std::string &replayPath)
	{
		logger = std::shared_ptr<Res::Logger>(new Res::Logger(*this, nullptr, nullptr));
		try
		{
			logger->load(replayPath);
		}
		catch (const std::exception &e)
		{
			failure(concatenate("Could not load replay: ", replayPath, ", ", e.what()));
			return;
		}

		if (!logger->bluePrint)
		{
			failure(concatenate("Could not find blueprint"));
			return;
		}

		keyboardController = std::shared_ptr<KeyboardController>(new KeyboardLoopReplay(*this, *logger));
		rngController = std::shared_ptr<RngController>(new RngValidator(*this, *logger));
		validator = std::shared_ptr<ValidatorReplay>(new ValidatorReplay(*this, *logger, *this));

		initialize(logger->bluePrint, nullptr);

		while (!finished)
		{
			validator->validate();
			(*activeTimer)(nullptr, CPS);
		}
	}
	void Multitest::signGame()
	{
		validator->finalize();
		finishGame();
	}
	void Multitest::finishGame()
	{
		ifSync(finished, true);
	}
	void Multitest::failure(const std::string &msg)
	{
		clog << KIR4::LRED << " - " << msg << KIR4::eol;
		setResult(Validator::Interface::RESULT::FAILED);
		finishGame();
	}
}



