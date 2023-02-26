#include "EditorMainEvent.h"

#include <KIR/sys/KIR5_files.h>

namespace UI::Editor
{
	std::shared_ptr<MainEvent> mainEvent;

	MainEvent::MainEvent(std::shared_ptr<KIR5::Panel> _parent):
		parent(_parent)
	{

		Objects::RunInitializer();

		*parent << columnControl;
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
		rowsControl[0]->pushBack(KIR5::Shared<UIline<UI_M>>());
		rowsControl[0]->pushBack(activeMap);

		rowsControl[1]->pushBack(KIR5::Shared<UIline<UI_M>>());
		rowsControl[1]->pushBack(KIR5::Shared<KIR5::Panel>());
		rowsControl[1]->pushBack(KIR5::Shared<UIline<UI_M>>());

		rowsControl[2]->pushBack(miniMap);
		rowsControl[2]->pushBack(KIR5::Shared<UIline<UI_M>>());
		rowsControl[2]->pushBack(controlPanel);

		*parent << saveWorldDialog;
		saveWorldDialog->hide();

		parent->fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			al_clear_to_color(KIR5::Color(20, 20, 20));
		}));

		parent->fncMoved.push_back([&](FNC_MOVED_PARAMS)
		{
			saveWorldDialog->move(0, 0, parent->width(), parent->height());

			for (auto &it : rowsControl)
			{
				it->items()[0]->width(280);
				it->items()[2]->width(0);
				it->wrapItem(it->items()[2].get());
			}

			columnControl->wrapItem(rowsControl[0].get());
		});
	}

	MainEvent::~MainEvent()
	{
	}
}