#include "TestMainEvent.h"
#include "Font.h"

#include "OriginalObjects.h"
#include "Worlds.h"

#include <KIR\AL\KIR5_bitmap_target.h>
#include <KIR\AL\KIR5_event_engine.h>
#include <KIR\sys\KIR5_files.h>

namespace UI::Multitest
{
	void MainEvent::initialize(std::shared_ptr<KIR5::Panel> _parent, const std::list<std::string> &replayPaths, std::int8_t speed)
	{
		new MainEvent(display, replayPaths, speed);
	}
	void MainEvent::shutdown()
	{
		s_object = nullptr;
	}
	MainEvent::MainEvent(const std::shared_ptr<KIR5::Panel> &_parent, const std::list<std::string> &replayPaths, std::int8_t speed):
		speed(speed),
		parent(_parent),
		replayPaths(std::move(replayPaths))
	{
		replayIterator = this->replayPaths.begin();
		s_object = std::shared_ptr<MainEvent>(this);

		parent->fncMoved.push_back([&](FNC_MOVED_PARAMS)
		{
			if (scene_)
			{
				scene_->move(0, 0, parent->width(), parent->height());
			}
		});

		parent->fncUpdate.push_back([&](FNC_UPDATE_PARAMS)
		{
			if (ALLEGRO_EVENT_TYPE_IS_USER(ptr_->type))
			{
				switch (ptr_->user.data1)
				{
					case Scene::ACTIVE_MAP_FINISHED:
					{
						finishGame();

						break;
					}
					default:
					{
						break;
					}
				}
			}
		});

		testGame(*replayIterator);
	}
	MainEvent::~MainEvent()
	{
	}

	std::shared_ptr<MainEvent> MainEvent::s_object;

	void MainEvent::testGame(const std::string &replayPath)
	{
		scene_ = std::shared_ptr<Scene::InteractiveMultitest>(new Scene::InteractiveMultitest(replayPath, speed));
		*parent << scene_;
		scene_->show();
		scene_->move(0, 0, parent->width(), parent->height());
	}
	void MainEvent::finishGame()
	{
		if (!scene_->isPassed())
		{
			processRet = -1;
			clog << KIR4::LWHITE << (*replayIterator) << " - " << KIR4::LRED << "FAILED" << KIR4::eol;
		}
		else
		{
			clog << KIR4::LWHITE << (*replayIterator) << " - " << KIR4::LGREEN << "PASSED" << KIR4::eol;
		}
		if (++replayIterator == replayPaths.end())
		{
			eventEngine->stop();
		}
		else
		{
			testGame(*replayIterator);
		}
	}
}