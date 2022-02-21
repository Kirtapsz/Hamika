#include "EditorMainEvent.h"

namespace Editor
{
	std::shared_ptr<MainEvent> mainEvent;

	MainEvent::MainEvent():KIR5::Display("Hamika Builder", 640, 480, ALLEGRO_RESIZABLE)
	{
		Objects::RunInitializer();

		*this << activeMap;
		activeMap->show();

		*this << mapList;
		mapList->show();

		*this << miniMap;
		miniMap->show();

		*this << controlPanel;
		controlPanel->show();

		*this << mapList->savePanelBackground;
		mapList->savePanelBackground->hide();

		fncDraw = [&](FNC_DRAW_PARAMS)
		{
			al_clear_to_color(KIR5::Color(20, 20, 20));
		};

		fncMoved = [&](FNC_MOVED_PARAMS)
		{
			mapList->savePanelBackground->move(0, 0, width(), height());

			miniMap->move(0, height() - 280, 280, 280);
			controlPanel->move(miniMap->virtualx2(), miniMap->virtualy1(), width() - miniMap->virtualx2(), miniMap->height());

			mapList->move(0, 0, miniMap->width(), controlPanel->virtualy1());
			activeMap->move(mapList->virtualx2(), 0, width() - mapList->virtualx2(), controlPanel->virtualy1());
		};

		fncMoved(this);
	}

	MainEvent::~MainEvent()
	{
	}
}