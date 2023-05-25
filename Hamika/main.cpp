#include <KIR/AL/KIR5_main.h>
#include <KIR/KIR5_Shared.h>

#include "MainEvent.h"
#include "EditorMainEvent.h"
#include "TestMainEvent.h"
#include "Cursor.h"
#include "Resource.h"
#include "versions.h"

std::shared_ptr<KIR5::Display> display;
std::shared_ptr<KIR5::EventEngine> eventEngine;
std::shared_ptr<KIR5::Event> cursor;

std::random_device rd;
std::default_random_engine generator(rd());
int processRet = 0;

int main(int argc, char *argv[])
{
	if (argc >= 2 && strcmp("--info", argv[1]) == 0)
	{
		clog << "KIR LIB commit ID: " << Versions::ApplicationCommitID << KIR4::eol;
		clog << "Application commit ID: " << Versions::KIRLIBCommitID << KIR4::eol;
	}
	else if (argc >= 4 && strcmp("--multitest", argv[1]) == 0 && strcmp("-json", argv[2]) == 0)
	{
		Res::MultitestInput testinput{argv[3]};
		Res::Json::LoadResource_(testinput);
		if (Res::initialize(Res::TEST))
		{
			std::vector<std::string> &testPaths = std::get<0>(testinput);
			for (auto &testPath : testPaths)
			{
				if (!UI::Scene::Multitest(testPath).isPassed())
				{
					processRet = -1;
					clog << KIR4::LWHITE << testPath << " - " << KIR4::LRED << "FAILED" << KIR4::eol;
				}
				else
				{
					clog << KIR4::LWHITE << testPath << " - " << KIR4::LGREEN << "PASSED" << KIR4::eol;
				}
			}
		}
		else
		{
			processRet = -1;
		}
	}
	else if (argc >= 3 && strcmp("--multitest", argv[1]) == 0 && strcmp("-i", argv[2]) != 0)
	{
		if (Res::initialize(Res::TEST))
		{
			for (std::size_t i = 2; i < argc; ++i)
			{
				if (!UI::Scene::Multitest(argv[i]).isPassed())
				{
					processRet = -1;
					clog << KIR4::LWHITE << argv[i] << " - " << KIR4::LRED << "FAILED" << KIR4::eol;
				}
				else
				{
					clog << KIR4::LWHITE << argv[i] << " - " << KIR4::LGREEN << "PASSED" << KIR4::eol;
				}
			}
		}
		else
		{
			processRet = -1;
		}
	}
	else
	{
		KIR5::initialize();

		if (argc >= 2 && strcmp("--editor", argv[1]) == 0)
		{
			display = std::shared_ptr<KIR5::Display>(new KIR5::Display("Hamika Builder", 1340, 480, ALLEGRO_RESIZABLE));
			if (Res::initialize(Res::EDITOR))
			{
				UI::Editor::MainEvent::initialize(display);
			}
			else
			{
				processRet = -1;
			}
		}
		else if (argc >= 5 && strcmp("--multitest", argv[1]) == 0 && strcmp("-i", argv[2]) == 0)
		{
			display = std::shared_ptr<KIR5::Display>(new KIR5::Display("Interactive multitest", 640, 480 + 83, ALLEGRO_RESIZABLE | ALLEGRO_OPENGL));
			std::list<std::string> replayPaths;
			for (std::size_t i = 4; i < argc; ++i)
			{
				replayPaths.push_back(argv[i]);
			}
			if (Res::initialize(Res::ITEST))
			{
				UI::Multitest::MainEvent::initialize(display, replayPaths, std::atol(argv[3]));
			}
			else
			{
				processRet = -1;
			}
		}
		else
		{
			display = std::shared_ptr<KIR5::Display>(new KIR5::Display("Hamika", 640, 480 + 83, ALLEGRO_RESIZABLE | ALLEGRO_OPENGL));
			if (Res::initialize(Res::GAME))
			{
				UI::MainEvent::initialize(display);
			}
			else
			{
				processRet = -1;
			}
		}

		if (processRet == 0)
		{
			cursor = KIR5::Shared<Cursor>(display);
			*display << cursor;
			display->callbackMove();

			eventEngine = std::shared_ptr<KIR5::EventEngine>(new KIR5::EventEngine(display, CPS));
			eventEngine->start();
		}
	}

	Res::shutdown();

	cursor = nullptr;
	eventEngine = nullptr;
	UI::Editor::MainEvent::shutdown();
	UI::Multitest::MainEvent::shutdown();
	UI::MainEvent::shutdown();
	display = nullptr;

	return processRet;
}