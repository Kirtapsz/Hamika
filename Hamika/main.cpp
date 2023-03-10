#include <KIR/AL/KIR5_main.h>
#include <KIR/KIR5_Shared.h>

#include "MainEvent.h"
#include "EditorMainEvent.h"
#include "Cursor.h"
#include "Resource.h"

std::shared_ptr<KIR5::Display> display;
std::shared_ptr<KIR5::EventEngine> eventEngine;
std::shared_ptr<KIR5::Event> cursor;

std::random_device rd;
std::default_random_engine generator(rd());


int main(int argc, char *argv[])
{
	int ret = 0;
	if (argc >= 2 && strcmp("--blueprints", argv[1]) == 0)
	{
	}
	if (argc >= 2 && strcmp("--reset", argv[1]) == 0)
	{
		Res::initialize(Res::RESET);
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
				ret = -1;
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
				ret = -1;
			}
		}

		if (ret == 0)
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
	UI::MainEvent::shutdown();
	display = nullptr;

	return ret;
}