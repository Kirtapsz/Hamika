#include "EditorMainEvent.h"

namespace Editor
{
	std::shared_ptr<MainEvent> mainEvent;

	MainEvent::MainEvent():KIR5::Display("Hamika Builder", 640, 480, ALLEGRO_RESIZABLE)
	{
		Objects::RunInitializer();

		*this << columnControl;
		columnControl->show();
		columnControl->setGap(3);
		(*columnControl) += KIR5::Column<>::WrapContent;

		activeMap->show();
		worldi->show();
		worldi->width(280);
		miniMap->show();
		miniMap->resize(280, 280);

		controlPanel->show();
		controlPanel->height(280);
		saveWorldDialog->hide();

		for (auto &it : rowsControl)
		{
			it->show();
			it->setGap(3);
			(*it) += KIR5::Row<>::WrapWidth;

			columnControl->pushBack(it);
		}

		(*rowsControl[0]) += KIR5::Row<>::FixHeight;

		rowsControl[0]->pushBack(worldi);
		rowsControl[0]->pushBack(KIR5::EVENT<UIline<UI_M>>());
		rowsControl[0]->pushBack(activeMap);

		rowsControl[1]->pushBack(KIR5::EVENT<UIline<UI_M>>());
		rowsControl[1]->pushBack(KIR5::EVENT<KIR5::Panel>());
		rowsControl[1]->pushBack(KIR5::EVENT<UIline<UI_M>>());

		rowsControl[2]->pushBack(miniMap);
		rowsControl[2]->pushBack(KIR5::EVENT<UIline<UI_M>>());
		rowsControl[2]->pushBack(controlPanel);

		*this << saveWorldDialog;
		saveWorldDialog->hide();

		fncDraw = [&](FNC_DRAW_PARAMS)
		{
			al_clear_to_color(KIR5::Color(20, 20, 20));
		};

		fncMoved = [&](FNC_MOVED_PARAMS)
		{
			saveWorldDialog->move(0, 0, width(), height());

			for (auto &it : rowsControl)
			{
				it->items()[0]->width(280);
				it->items()[2]->width(0);
				it->wrapItem(it->items()[2].get());
			}

			columnControl->wrapItem(rowsControl[0].get());
		};

		fncMoved(this);
	}

	MainEvent::~MainEvent()
	{
	}
}