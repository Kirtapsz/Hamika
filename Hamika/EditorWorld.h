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

namespace Editor
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
		private: KIR5::Bitmap arrowUpImg;
		private: KIR5::Bitmap arrowDownImg;
		private: KIR5::Bitmap editImg;
		private: KIR5::Bitmap deleteImg;
		private: class Item: public KIR5::Panel
		{
			friend class Worldi;
			private: int ID_;
			private: KIR5::EVENT<KIR5::TextBox<KIR5::Panel>> ID;
			private: KIR5::EVENT<KIR5::Label<KIR5::Panel>> name;
			private: KIR5::EVENT<KIR5::ColoredBitmapButton<KIR5::Button<>>> moveUp;
			private: KIR5::EVENT<KIR5::ColoredBitmapButton<KIR5::Button<>>> moveDown;
			private: KIR5::EVENT<KIR5::ColoredBitmapButton<KIR5::Button<>>> deleteMap;
			private: KIR5::EVENT<KIR5::ColoredBitmapButton<KIR5::Button<>>> editMap;
			private: KIR5::EVENT<BlueprintPanel> blueprintPanel;
			private: KIR5::Event::FncSwap<FNC_MOVED_TYPE> FncMoved;
			private: std::shared_ptr<BluePrint> bluePrint;
			private: KIR5::Event::FncSwap<FNC_DRAW_TYPE> FncDraw;
			private: KIR5::Event::FncSwap<FNC_DRAW_TYPE> FncDrawBlueprint;

			public: Item(Worldi &mapList, int ID_, const std::shared_ptr<BluePrint> &bluePrint);

			public: void setID(int ID_);
			public: const std::shared_ptr<BluePrint> &getBluePrint() const;
		};

		private: KIR5::EVENT<KIR5::Column<KIR5::Panel>> content;
		private: KIR5::Event::FncSwap<FNC_MOVED_TYPE> FncMoved;

		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> loadButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> saveButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> newMapButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> newWorldButton;

		private: KIR5::Event::FncSwap<FNC_MOVED_TYPE> FncNameRowMoved;
		private: KIR5::EVENT<UIlabel<UI_S>> worltTitle_Label;
		private: KIR5::EVENT<UItextBox<UI_S>> worltTitle_TextBox;

		private: KIR5::EVENT<KIR5::Panel> listPanel;
		private: KIR5::EVENT<KIR5::ListPanel<KIR5::Panel, Item>> list;
		private: KIR5::EVENT<KIR5::VerticalScrollBar<>> scrollBar;
		private: KIR5::EVENT<KIR5::RectangleButton<>> scrollBarThumb;
		private: KIR5::EVENT<KIR5::ScrollControl> scrollControl = new KIR5::ScrollControl(list, scrollBar);

		public: Worldi();
		public: ~Worldi();
		public: int staticWidth();
		public: void setWorld(std::shared_ptr<World> &world);

	};
}