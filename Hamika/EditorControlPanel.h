#pragma once

#include "Types.h"
#include "World.h"
#include "Objects.h"
#include "Font.h"
#include "BlueprintPanel.h"
#include "EditorActiveBlock.h"
#include "UItools.h"

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_list_panel.h>
#include <KIR\KIR4_time.h>
#include <KIR\AL\KIR5_scroll_bar.h>
#include <KIR\AL\KIR5_label.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>
#include <KIR/AL/KIR5_panel_control.h>
#include <KIR/AL/KIR5_combo_box.h>

#include <memory>

namespace Editor
{
	class ControlPanel: public KIR5::Panel
	{
		private: static const std::string GLOBAL_GRAVITY_ON;
		private: static const std::string GLOBAL_GRAVITY_OFF;

		private: std::shared_ptr<BluePrint> bluePrint;
		private: std::shared_ptr<Array2D<ActiveBlock<EditorObjectBase>>> map;

		private: std::array<KIR5::Bitmap, 4> arrowTilted;
		private: std::array<KIR5::Bitmap, 4> arrowNarrow;
		private: std::array<KIR5::Bitmap, 4> stick;
		private: KIR5::Bitmap ball;
		private: KIR5::Bitmap execute;

		private: class ResizeButtonClass:public UIbbutton<UI_M>
		{
			friend class ControlPanel;
			private: bool isSelected = false;
		};

		private: class BlockIDPanel: public UIpanel<UI_M>
		{
			friend class ControlPanel;
			private: Type::ID id = 0;
			private: int counter = -1;
			private: int multiplier = 1;
			public: BlockIDPanel();
		};

			   // UI ============= ROW 1
		private: KIR5::EVENT<UIlabel<UI_M>> titleOfBluePrint_Label;
		private: KIR5::EVENT<UItextBox<UI_M>> titleOfBluePrint_TextBox;
		private: KIR5::EVENT<UIlabel<UI_M>> sizeOfCamera_Label;
		private: KIR5::EVENT<UItextBox<UI_M>> sizeOfCamera_TextBox;
		private: KIR5::EVENT<UIlabel<UI_M>> murphySpeed_Label;
		private: KIR5::EVENT<UItextBox<UI_M>> murphySpeed_TextBox;
		private: KIR5::EVENT<UIbutton<UI_M>>  saveBluePrint_Button;
			   // UI ============= ROW 2
			   // ---------------- COL 1
		private: KIR5::EVENT<UIlabel<UI_S>> rotation_Label;
		private: std::array<KIR5::EVENT<ResizeButtonClass>, 4> rotation_Navigation_Buttons;
		private: KIR5::EVENT<UIbbutton<UI_M>> rotation_Apply_Button;
			   // ---------------- COL 2
		private: KIR5::EVENT<UIlabel<UI_S>> bluePrintResize_Label;
		private: std::array<std::array<KIR5::EVENT<ResizeButtonClass>, 3>, 3> bluePrintResize_Navigation_Buttons;
		private: KIR5::EVENT<UItextBox<UI_M>> bluePrintResize_Size_TextBox;
		private: KIR5::EVENT<UIbbutton<UI_M>> bluePrintResize_Apply_Button;
			   // ---------------- COL 3
		private: KIR5::Event::FncSwap<FNC_LOSS_FOCUS_TYPE> objectIDTextBoxLossFocus;
		private: KIR5::Event::FncSwap<FNC_GET_FOCUS_TYPE> objectIDTextBoxGetFocus;
		private: KIR5::EVENT<UIlabel<UI_S>> object_Label;
		private: KIR5::EVENT<BlockIDPanel> object_Panel;
		private: KIR5::EVENT<UItextBox<UI_M>> object_ID_TextBox;
		private: KIR5::EVENT<UItextBox<UI_M>> object_Rate_TextBox;
		private: KIR5::EVENT<UIlabel<UI_M>> object_RandomFill_Label;
		private: KIR5::EVENT<UIbutton<UI_M>> object_RandomFill_Button;
		private: KIR5::EVENT<UIbbutton<UI_M>> object_Apply_Button;
		private: KIR5::EVENT<UIbutton<UI_M>> object_FillFrame_Button;
		private: KIR5::EVENT<UIbutton<UI_M>> object_FillContent_Button;
		private: KIR5::EVENT<UIbutton<UI_M>> object_selectAllOfThisType_Button;
			   // ---------------- COL 4
		private: KIR5::EVENT<UIlabel<UI_S>> infotron_Label;
		private: KIR5::EVENT<UItextBox<UI_M>> infotron_ToCollect_TextBox;
		private: KIR5::EVENT<UIlabel<UI_M>> infotron_Capacity_Label;
		private: std::vector<KIR5::EVENT<BlockIDPanel>> infotron_Pickers_Buttons;
			   // ---------------- COL 5
		private: KIR5::EVENT<UIbutton<UI_S>> selectAllButton;
		private: KIR5::EVENT<UIbutton<UI_S>> gravityTurnOnOffButton;
		private: KIR5::EVENT<UIbutton<UI_S>> initExplodeButton;
		private: KIR5::EVENT<UIbutton<UI_S>> selectFriendlyButton;
		private: KIR5::EVENT<UIbutton<UI_S>> ramRepairButton;
		private: KIR5::EVENT<UIbutton<UI_S>> randomizeRamsButton;
		private: KIR5::EVENT<UIbutton<UI_S>> spawnPointButton;
		private: KIR5::EVENT<UIbutton<UI_S>> globalGravityOnOffButton;
			   // UI ============= ROW 3
		private: std::array<KIR5::EVENT<BlockIDPanel>, 24> prevBlockPickers;
			   // UI =============

		public: void pickID(Type::ID id);
		public: void setOperationMode();
		public: void updateBlocks();
		public: void SetMap(std::shared_ptr<BluePrint> &bluePrint, std::shared_ptr<Array2D<ActiveBlock<EditorObjectBase>>> &map);

		public: ControlPanel();
		public: ~ControlPanel();

	};
}