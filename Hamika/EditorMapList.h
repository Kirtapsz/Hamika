#pragma once

#include "Types.h"
#include "BluePrints.h"
#include "Objects.h"
#include "Font.h"
#include "BlueprintPanel.h"

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_list_panel.h>
#include <KIR\KIR4_time.h>
#include <KIR\AL\KIR5_scroll_bar.h>
#include <KIR\AL\KIR5_label.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>

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

	class MapList: public KIR5::Panel
	{
		friend class MainEvent;
		private: KIR5::Bitmap arrowUpImg;
		private: KIR5::Bitmap arrowDownImg;
		private: KIR5::Bitmap editImg;
		private: KIR5::Bitmap deleteImg;
		private: class Item: public KIR5::Panel
		{
			friend class MapList;
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

			public: Item(MapList &mapList, int ID_, std::shared_ptr<BluePrint> &bluePrint);

			public: void setID(int ID_);
		};

		private: KIR5::EVENT<KIR5::Panel> savePanelBackground;
		private: KIR5::EVENT<KIR5::ColoredPanel> savePanel;
		private: KIR5::EVENT<KIR5::Label<KIR5::Panel>> saveDirectoryLabel;
		private: KIR5::EVENT<KIR5::TextBox<KIR5::Panel>> saveDirectory;
		private: KIR5::EVENT<KIR5::Label<KIR5::Panel>> saveWordNameDirectoryLabel;
		private: KIR5::EVENT<KIR5::TextBox<KIR5::Panel>> saveWordNameDirectory;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> saveOriginal;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> saveCancel;

		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> loadButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> saveButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> newMapButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> newWorldButton;

		private: KIR5::EVENT<KIR5::ListPanel<KIR5::Panel, Item>> list;
		private: KIR5::EVENT<KIR5::VerticalScrollBar<>> scrollBar;
		private: KIR5::EVENT<KIR5::RectangleButton<>> scrollBarThumb;
		private: KIR5::EVENT<KIR5::ScrollControl> scrollControl = new KIR5::ScrollControl(list, scrollBar);
		private: KIR5::Event::FncSwap<FNC_MOVED_TYPE> FncMoved;

		public: MapList();
		public: ~MapList();
		public: int staticWidth();
		public: void setWorld(std::vector<std::shared_ptr<BluePrint>> &world);

	};
}