#include <KIR/AL/KIR5_main.h>

#include "MainEvent.h"
#include "EditorMainEvent.h"

std::shared_ptr<KIR5::EventEngine> eventEngine;

std::random_device rd;
std::default_random_engine generator(rd());

int main(int argc, char *argv[])
{
	KIR5::initialize();

	if (argc >= 2 && strcmp("--editor", argv[1]) == 0)
	{
		Editor::mainEvent = std::shared_ptr<Editor::MainEvent>(new Editor::MainEvent());
		eventEngine = std::shared_ptr<KIR5::EventEngine>(new KIR5::EventEngine(Editor::mainEvent, CPS));
	}
	else
	{
		mainEvent = std::shared_ptr<MainEvent>(new MainEvent());
		eventEngine = std::shared_ptr<KIR5::EventEngine>(new KIR5::EventEngine(mainEvent, CPS));
	}

	eventEngine->start();

	eventEngine = nullptr;
	Editor::mainEvent = nullptr;
	mainEvent = nullptr;

	return 0;
}