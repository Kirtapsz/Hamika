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

	class Worldi: public KIR5::Panel
	{
		friend class MainEvent;
		friend class SaveWorldDialog;

		private: class Item: public KIR5::Panel
		{
			friend class Worldi;
			private: int ID_;
			private: KIR5::Shared<KIR5::TextBox<KIR5::Panel>> ID;
			private: KIR5::Shared<KIR5::Label<KIR5::Panel>> name;
			private: KIR5::Shared<KIR5::ColoredBitmapButton<KIR5::Button<>>> moveUp;
			private: KIR5::Shared<KIR5::ColoredBitmapButton<KIR5::Button<>>> moveDown;
			private: KIR5::Shared<KIR5::ColoredBitmapButton<KIR5::Button<>>> deleteMap;
			private: KIR5::Shared<KIR5::ColoredBitmapButton<KIR5::Button<>>> editMap;
			private: KIR5::Shared<BlueprintPanel> blueprintPanel;
			private: std::shared_ptr<Res::BluePrint> bluePrint;

			public: Item(Worldi &mapList, int ID_, const std::shared_ptr<Res::BluePrint> &bluePrint);

			public: void setID(int ID_);
			public: const std::shared_ptr<Res::BluePrint> &getBluePrint() const;
		};

		private: KIR5::Shared<KIR5::Column<KIR5::Panel>> content;

		private: KIR5::Shared<KIR5::TextButton<KIR5::RectangleButton<>>> loadButton;
		private: KIR5::Shared<KIR5::TextButton<KIR5::RectangleButton<>>> saveButton;
		private: KIR5::Shared<KIR5::TextButton<KIR5::RectangleButton<>>> newMapButton;
		private: KIR5::Shared<KIR5::TextButton<KIR5::RectangleButton<>>> newWorldButton;

		private: KIR5::Shared<UIlabel<UI_S>> worltTitle_Label;
		private: KIR5::Shared<UItextBox<UI_S>> worltTitle_TextBox;

		private: KIR5::Shared<KIR5::Panel> listPanel;
		private: KIR5::Shared<KIR5::ListPanel<KIR5::Panel, Item>> list;
		private: KIR5::Shared<KIR5::VerticalScrollBar<>> scrollBar;
		private: KIR5::Shared<KIR5::RectangleButton<>> scrollBarThumb;
		private: KIR5::Shared<KIR5::ScrollControl> scrollControl = new KIR5::ScrollControl(list, scrollBar);

		public: Worldi();
		public: ~Worldi();
		public: int staticWidth();
		public: void setWorld(std::shared_ptr<Res::World> &world);

	};
}