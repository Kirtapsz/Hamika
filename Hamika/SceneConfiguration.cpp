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
			drawer.updateConfiguration({drawnerBar->width(), drawnerBar->height()}, cameraSize);
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

		layer_Panel = std::make_shared<LayerPanel>(&drawer);

		fncMoved.push_back(activeMoved);
		fncKeyDown.push_back(activeKeyDown);
		fncKeyUp.push_back(activeKeyUp);

		*this << layer_Panel << keyboard_Panel << loopcounter_Label;

		fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			keyboard_Panel->bringTop();
			keyboard_Panel->move(
				width() - decltype(keyboard_Panel)::element_type::ADJUSTER_WIDTH, 0,
				decltype(keyboard_Panel)::element_type::ADJUSTER_WIDTH, decltype(keyboard_Panel)::element_type::ADJUSTER_HEIGHT
			);
			loopcounter_Label->bringTop();
			loopcounter_Label->move(keyboard_Panel->x(), keyboard_Panel->y2(), keyboard_Panel->width(), 28);
			layer_Panel->bringTop();
			layer_Panel->move(
				width() - decltype(layer_Panel)::element_type::ADJUSTER_WIDTH, loopcounter_Label->y2(),
				decltype(layer_Panel)::element_type::ADJUSTER_WIDTH, decltype(layer_Panel)::element_type::ADJUSTER_HEIGHT
			);
		});

		*coreTimer = [&](FNC_TIMER_PARAMS) -> FNC_TIMER_RET
		{
			(*activeTimer)(obj_, cps_);
			loopcounter_Label->updateLoopcounter(loopCounter);
			keyboard_Panel->updateKeyboardState(keyboardController);
		};

		*delayTimerStart = [&](FNC_TIMER_PARAMS) -> FNC_TIMER_RET
		{
			if (--delayLoop == 0)
			{
				drawnerBar->fncDraw.remove(delayDraw);
				fncTimer.remove(delayTimerStart);
				if (!paused)
				{
					fncTimer.push_back(coreTimer);
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
					fncTimer.push_back(coreTimer);
				}
				else
				{
					fncTimer.remove(coreTimer);
				}
				paused = !paused;
				return true;
			}
			else if (key_ == 'f')
			{
				if (paused)
				{
					(*activeTimer)(nullptr, 0.);
					loopcounter_Label->updateLoopcounter(loopCounter);
					keyboard_Panel->updateKeyboardState(keyboardController);
					return true;
				}
			}
			else if (key_ == 't')
			{
				murphyDead(murphy);
				return true;
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
				Type::Coord coord = drawer.getTargetOfCursor({x_, y_});
				if (map->Test(coord))
				{
					if (button_ == KIR5::MOUSE_BUTTON_LEFT)
					{
						auto &block = reach(map)[coord];
						Json json = block.object->print();
						validator->validate(Res::Log::ObjectValidator::OBJECT_T, json);

						if (block.object->isExists)
						{
							std::cout << "::: BLOCK :::" << std::endl;
							std::cout << std::setw(4) << std::setprecision(3) << block.print() << std::endl;
							std::cout << "::: OBJECT :::" << std::endl;
							std::cout << std::setw(4) << std::setprecision(3) << json << std::endl;
							std::cout << "- - - - - - - - - - - - - - -" << std::endl;
						}
						else
						{
							std::cout << "::: OBJECT ::: - nullptr" << std::endl;
						}
						return true;
					}
					if (button_ == KIR5::MOUSE_BUTTON_RIGHT)
					{
						auto &block = reach(map)[coord];
						Json json = block.remain->print();
						validator->validate(Res::Log::ObjectValidator::REMAIN_T, json);
						if (block.remain->isExists)
						{
							std::cout << "::: BLOCK :::" << std::endl;
							std::cout << std::setw(4) << std::setprecision(3) << block.print() << std::endl;
							std::cout << "::: REMAIN :::" << std::endl;
							std::cout << std::setw(4) << std::setprecision(3) << json << std::endl;
							std::cout << "- - - - - - - - - - - - - - -" << std::endl;
						}
						else
						{
							std::cout << "::: REMAIN ::: - nullptr" << std::endl;
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



	KeyboardPanel::Icon::Icon()
	{
		fncDraw.push_back(FNC_DRAW([&](FNC_DRAW_PARAMS)->FNC_DRAW_RET
		{
			static const KIR5::Color color_active{200,200,200};
			static const KIR5::Color color_inactive{30,30,30};

			const KIR5::Color &color = is_active_ ? color_active : color_inactive;

			bitmap_.drawTintedScaled(x_, y_, w_, h_, 0, 0, bitmap_.width(), bitmap_.height(), color);
		}));
	}

	void KeyboardPanel::Icon::setActive(bool _is_active)
	{
		is_active_ = _is_active;
	}
	void KeyboardPanel::Icon::setBitmap(const KIR5::Bitmap &bitmap)
	{
		bitmap_ = bitmap;
	}

	KeyboardPanel::KeyboardPanel()
	{
		keyboard_state[1][0]->setBitmap(Res::uielements[Res::UIElements::StickUp]);
		keyboard_state[0][1]->setBitmap(Res::uielements[Res::UIElements::StickLeft]);
		keyboard_state[1][1]->setBitmap(Res::uielements[Res::UIElements::Ball]);
		keyboard_state[2][1]->setBitmap(Res::uielements[Res::UIElements::StickRight]);
		keyboard_state[1][2]->setBitmap(Res::uielements[Res::UIElements::StickDown]);

		for (int x = 0; x < 3; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				keyboard_state[x][y]->show();
				*this << keyboard_state[x][y];
			}
		}

		keyboard_state[0][0]->hide();
		keyboard_state[0][2]->hide();
		keyboard_state[2][0]->hide();
		keyboard_state[2][2]->hide();
	}

	void KeyboardPanel::updateKeyboardState(const std::shared_ptr<KeyboardController> &_keyboard_controller)
	{
		keyboard_state[1][0]->setActive(_keyboard_controller->actionUp.isLocked());
		keyboard_state[0][1]->setActive(_keyboard_controller->actionLeft.isLocked());
		keyboard_state[1][1]->setActive(_keyboard_controller->actionSpecial.isLocked());
		keyboard_state[2][1]->setActive(_keyboard_controller->actionRight.isLocked());
		keyboard_state[1][2]->setActive(_keyboard_controller->actionDown.isLocked());
	}



	LoopCounter::LoopCounter()
	{
		setTextColor(KIR5::Color(230, 230, 230));
	}
	void LoopCounter::updateLoopcounter(unsigned long long _loop_counter)
	{
		setText(std::to_string(_loop_counter));
	}

	InteractiveMultitest::InteractiveMultitest(const std::string &replayPath, std::int8_t speed_):
		speed(speed_)
	{
		*this << keyboard_Panel << loopcounter_Label;

		fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			keyboard_Panel->bringTop();
			keyboard_Panel->move(
				width() - decltype(keyboard_Panel)::element_type::ADJUSTER_WIDTH, 0,
				decltype(keyboard_Panel)::element_type::ADJUSTER_WIDTH, decltype(keyboard_Panel)::element_type::ADJUSTER_HEIGHT
			);
			loopcounter_Label->bringTop();
			loopcounter_Label->move(keyboard_Panel->x(), keyboard_Panel->y2(), keyboard_Panel->width(), 28);
		});

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
			drawer.updateConfiguration({drawnerBar->width(), drawnerBar->height()}, cameraSize);
		};

		fncMoved.push_back(activeMoved);

		*coreTimer = [&](FNC_TIMER_PARAMS) -> FNC_TIMER_RET
		{
			for (std::int8_t i = 0; i < speed; ++i)
			{
				validator->validate();
				(*activeTimer)(obj_, cps_);
			}
			loopcounter_Label->updateLoopcounter(loopCounter);
			keyboard_Panel->updateKeyboardState(keyboardController);
		};

		fncTimer.push_back(coreTimer);

		fncKeyChar.push_back([&](FNC_KEY_CHAR_PARAMS) -> FNC_KEY_CHAR_RET
		{
			if (key_ == 'p')
			{
				if (paused)
				{
					fncTimer.push_back(coreTimer);
				}
				else
				{
					fncTimer.remove(coreTimer);
				}
				paused = !paused;
				return true;
			}
			else if (key_ == 'f')
			{
				if (paused)
				{
					(*activeTimer)(nullptr, 0.);
					loopcounter_Label->updateLoopcounter(loopCounter);
					keyboard_Panel->updateKeyboardState(keyboardController);
					return true;
				}
			}
			return false;
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

		drawnerBar->fncMouseButtonDown.push_back([&](FNC_MOUSE_BUTTON_DOWN_PARAMS) -> FNC_MOUSE_BUTTON_DOWN_RET
		{
			if (eventEngine->getTargetPanel().get() == drawnerBar.get())
			{
				Type::Coord coord = drawer.getTargetOfCursor({x_, y_});
				if (map->Test(coord))
				{
					if (button_ == KIR5::MOUSE_BUTTON_LEFT)
					{
						auto &block = reach(map)[coord];
						Json json = block.object->print();

						std::cout << "::: BLOCK :::" << std::endl;
						std::cout << std::setw(4) << std::setprecision(3) << block.print() << std::endl;
						std::cout << "::: OBJECT :::" << std::endl;
						std::cout << std::setw(4) << std::setprecision(3) << json << std::endl;
						std::cout << "- - - - - - - - - - - - - - -" << std::endl;

						return true;
					}
					if (button_ == KIR5::MOUSE_BUTTON_RIGHT)
					{
						auto &block = reach(map)[coord];
						Json json = block.remain->print();

						std::cout << "::: BLOCK :::" << std::endl;
						std::cout << std::setw(4) << std::setprecision(3) << block.print() << std::endl;
						std::cout << "::: REMAIN :::" << std::endl;
						std::cout << std::setw(4) << std::setprecision(3) << json << std::endl;
						std::cout << "- - - - - - - - - - - - - - -" << std::endl;

						return true;
					}
				}
			}

			return false;
		});

		statusbar->hide();
		callbackMove();
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



