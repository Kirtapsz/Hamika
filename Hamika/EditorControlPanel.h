#pragma once

#include "Types.h"
#include "BluePrints.h"
#include "Objects.h"
#include "Font.h"
#include "BlueprintPanel.h"
#include "EditorActiveBlock.h"

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_list_panel.h>
#include <KIR\KIR4_time.h>
#include <KIR\AL\KIR5_scroll_bar.h>
#include <KIR\AL\KIR5_label.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>
#include <KIR/AL/KIR5_panel_control.h>

#include <memory>


//class BaseTextBox: public KIR5::RectangleButton<KIR5::Button<KIR5::TextBox<>>>
//	class BaseTextButton: public KIR5::RectangleButton<KIR5::TextButton<>>
//	class BaseButton: public KIR5::RectangleButton<>
//
//
//	ColoredBitmapButton

namespace Editor
{
	class ControlPanel: public KIR5::Panel
	{
		private: std::shared_ptr<BluePrint> bluePrint;
		private: std::shared_ptr<Array2D<ActiveBlock<EditorObjectBase>>> map;

		private: std::vector<KIR5::Bitmap> arrowTilted;
		private: std::vector<KIR5::Bitmap> arrowNarrow;
		private: std::vector<KIR5::Bitmap> stick;
		private: KIR5::Bitmap ball;
		private: KIR5::Bitmap execute;


		private: class ResizeButtonClass:public KIR5::RectangleButton<KIR5::ColoredBitmapButton<KIR5::Button<>>>
		{
			friend class ControlPanel;
			private: bool isSelected = false;
		};
		private: KIR5::EVENT<KIR5::ColoredPanel> mapResizeFrame;
		private: KIR5::EVENT<KIR5::ColoredPanel> mapResizeBackground;
		private: KIR5::EVENT<KIR5::Column<KIR5::Panel>> mapResizeContainer;
		private: KIR5::EVENT<KIR5::Label<>> mapResizeLabel;
		private: std::vector<std::vector<KIR5::EVENT<ResizeButtonClass>>> resizeButtons;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::Button<KIR5::TextBox<>>>> mapResizeTextBox;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::ColoredBitmapButton<KIR5::Button<>>>> mapResizeExecute;


		private: KIR5::EVENT<KIR5::ColoredPanel> rotationFrame;
		private: KIR5::EVENT<KIR5::ColoredPanel> rotationBackground;
		private: KIR5::EVENT<KIR5::Column<KIR5::Panel>> rotationContainer;
		private: KIR5::EVENT<KIR5::Label<>> rotationLabel;
		private: std::vector<KIR5::EVENT<ResizeButtonClass>> rotationButtons;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::ColoredBitmapButton<KIR5::Button<>>>> rotationExecute;

		private: class BlockIDPanel: public KIR5::Panel
		{
			friend class ControlPanel;
			private: Type::ID id = 0;
			private: int counter = -1;
			private: int multiplier = 1;
			public: BlockIDPanel();
		};

		private: KIR5::EVENT<KIR5::ColoredPanel> blockPickerFrame;
		private: KIR5::EVENT<KIR5::ColoredPanel> blockPickerBackground;
		private: KIR5::EVENT<KIR5::Column<KIR5::Panel>> blockPickerContainer;

		private: KIR5::EVENT<BlockIDPanel> objectPanel;
		private: KIR5::Event::FncSwap<FNC_LOSS_FOCUS_TYPE> objectIDTextBoxLossFocus;
		private: KIR5::Event::FncSwap<FNC_GET_FOCUS_TYPE> objectIDTextBoxGetFocus;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::Button<KIR5::TextBox<>>>> objectIDTextBox;

		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::Button<KIR5::TextBox<>>>> randomFillTextBox;
		private: KIR5::EVENT<KIR5::Label<>> randomFillLabel;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> randomFillButton;

		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::ColoredBitmapButton<KIR5::Button<>>>> fillButton;

		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> fillFrameButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> fillContentButton;

		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> fillSelectAllButton;


		private: KIR5::EVENT<KIR5::FramedRectangleButton<KIR5::Button<KIR5::TextBox<>>>> nameOfTheMapTextBox;

		private: KIR5::EVENT<KIR5::FramedRectangleButton<KIR5::Button<KIR5::TextButton<>>>>  saveButton;

		private: KIR5::EVENT<KIR5::ColoredPanel> infotronFrame;
		private: KIR5::EVENT<KIR5::ColoredPanel> infotronBackground;
		private: KIR5::EVENT<KIR5::Column<KIR5::Panel>> infotronContainer;
		private: KIR5::EVENT<KIR5::Label<>> infotronLabel;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::Button<KIR5::TextBox<>>>> infotronToCollectTextBox;
		private: KIR5::EVENT<KIR5::Label<>> infotronToCollectLabel;
		private: std::vector<KIR5::EVENT<BlockIDPanel>> infotronPickers;


		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> selectAllButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> gravityTurnOnOffButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> initExplodeButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> selectFriendlyButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> spawnPointButton;

		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> ramRepairButton;
		private: KIR5::EVENT<KIR5::RectangleButton<KIR5::TextButton<>>> randomizeRamsButton;


		private: std::vector<KIR5::EVENT<BlockIDPanel>> prevBlockPickers;

		public: void pickID(Type::ID id);
		public: void setOperationMode();
		public: void updateBlocks();
		public: void SetMap(std::shared_ptr<BluePrint> &bluePrint, std::shared_ptr<Array2D<ActiveBlock<EditorObjectBase>>> &map);

		public: ControlPanel();
		public: ~ControlPanel();

	};
}