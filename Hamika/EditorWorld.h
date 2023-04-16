#pragma once

#include "Types.h"
#include "World.h"
#include "Objects.h"
#include "Font.h"
#include "BlueprintPanel.h"
#include "UItools.h"

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_list_panel.h>
#include <KIR\KIR4_time.h>
#include <KIR\AL\KIR5_scroll_bar.h>
#include <KIR\AL\KIR5_label.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>
#include <KIR\AL\KIR5_combo_box.h>
#include <KIR/AL/KIR5_panel_control.h>

#include <memory>

namespace UI::Editor
{
	enum
	{
		DELETE_ITEM = 300,
		MOVE_ITEM_UP,
		MOVE_ITEM_DOWN,
		MOVE_ITEM_POSITION,
		REFRESH_ITEM,
	};

	class Worldi: public Panel
	{
		friend class MainEvent;
		friend class SaveWorldDialog;

		private: class Item: public Panel
		{
			friend class Worldi;
			private: int ID_;
			private: KIR5::Shared<TextBox<Res::Consolas, 15>> ID;
			private: KIR5::Shared<Label<Res::Consolas, 15>> name;
			private: KIR5::Shared<BmpButton> moveUp;
			private: KIR5::Shared<BmpButton> moveDown;
			private: KIR5::Shared<BmpButton> deleteMap;
			private: KIR5::Shared<BmpButton> editMap;
			private: KIR5::Shared<BlueprintPanel> blueprintPanel;
			private: std::shared_ptr<Res::BluePrint> bluePrint;

			public: Item(Worldi &mapList, int ID_, const std::shared_ptr<Res::BluePrint> &bluePrint);

			public: void setID(int ID_);
			public: const std::shared_ptr<Res::BluePrint> &getBluePrint() const;
		};

		private: KIR5::Shared<KIR5::Column<Panel>> content;

		private: KIR5::Shared<Button<Res::Consolas, 20>> loadButton;
		private: KIR5::Shared<Button<Res::Consolas, 20>> saveButton;
		private: KIR5::Shared<Button<Res::Consolas, 20>> newMapButton;
		private: KIR5::Shared<Button<Res::Consolas, 20>> newWorldButton;

		private: KIR5::Shared<Label<Res::Consolas, 15>> worltTitle_Label;
		private: KIR5::Shared<TextBox<Res::Consolas, 15>> worltTitle_TextBox;

		private: KIR5::Shared<Panel> listPanel;
		private: KIR5::Shared<KIR5::ListPanel<Panel, Item>> list;
		private: KIR5::Shared<KIR5::VerticalScrollBar<>> scrollBar;
		private: KIR5::Shared<KIR5::RectangleButton<>> scrollBarThumb;
		private: KIR5::Shared<KIR5::ScrollControl> scrollControl = new KIR5::ScrollControl(list, scrollBar);

		public: Worldi();
		public: ~Worldi();
		public: int staticWidth();
		public: void setWorld(std::shared_ptr<Res::World> &world);

	};
}