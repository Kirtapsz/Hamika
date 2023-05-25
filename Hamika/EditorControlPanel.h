#pragma once

#include "Types.h"
#include "World.h"
#include "Objects.h"
#include "Font.h"
#include "BlueprintPanel.h"
#include "EditorSceneBlock.h"
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

namespace UI::Editor
{
	class ControlPanel: public AdjusterPanel<Panel, 647, 270>
	{
		private: static const std::string GLOBAL_GRAVITY_ON;
		private: static const std::string GLOBAL_GRAVITY_OFF;

		private: std::shared_ptr<Res::BluePrint> bluePrint;
		private: std::shared_ptr<Matrix<SceneBlock<EditorObjectBase>>> map;

		private: class ResizeButtonClass:public BmpButton
		{
			friend class ControlPanel;
			private: bool isSelected = false;
		};

		private: class BlockIDPanel: public virtual Panel
		{
			friend class ControlPanel;
			private: KIR5::Font font;
			private: Type::ID id = 0;
			private: int counter = -1;
			private: int multiplier = 1;
			public: BlockIDPanel();
		};

		private: KIR5::Shared<AdjusterPanel<Window, 108, 94>> info_Panel;

		private: KIR5::Shared<AdjustablePanel<Label<Res::Consolas>, 229, 0, 67, 19>> titleOfBluePrint_Label;
		private: KIR5::Shared<AdjustablePanel<TextBox<Res::Consolas>, 298, 0, 238, 19 >> titleOfBluePrint_TextBox;

		private: KIR5::Shared<AdjustablePanel<Label<Res::Consolas>, 229, 22, 67, 19 >> sizeOfCamera_Label;
		private: KIR5::Shared<AdjustablePanel<TextBox<Res::Consolas>, 298, 22, 238, 19 >> sizeOfCamera_TextBox;

		private: KIR5::Shared<AdjustablePanel<Label<Res::Consolas>, 229, 44, 67, 19 >> murphySpeed_Label;
		private: KIR5::Shared<AdjustablePanel<TextBox<Res::Consolas>, 298, 44, 238, 19 >> murphySpeed_TextBox;

		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas, 20>, 0, 200, 85, 70>>  saveBluePrint_Button;

		private: KIR5::Shared<AdjustPanel<Window, 480, 72, 60, 74>> rotation_Panel;
		private: KIR5::Shared<AdjustablePanel<Label<Res::Consolas>, 3, 3, 64, 14>> rotation_Label;
		private: std::array<std::shared_ptr<ResizeButtonClass>, 4> rotation_Navigation_Buttons{{
			KIR5::Shared<AdjustablePanel<ResizeButtonClass, 22, 17, 16, 16>>(),
			KIR5::Shared<AdjustablePanel<ResizeButtonClass, 41, 36, 16, 16>>(),
			KIR5::Shared<AdjustablePanel<ResizeButtonClass, 22, 55, 16, 16>>(),
			KIR5::Shared<AdjustablePanel<ResizeButtonClass, 3, 36, 16, 16>>()
		}};
		private: KIR5::Shared<AdjustablePanel<BmpButton, 22, 36, 16, 16>> rotation_Apply_Button;

		private: KIR5::Shared<AdjustPanel<Window, 539, 0, 108, 145>> bluePrintResize_Panel;
		private: KIR5::Shared<AdjustablePanel<Label<Res::Consolas>, 3, 3, 106, 18>> bluePrintResize_Label;
		private: KIR5::Shared<AdjustablePanel<TextBox<Res::Consolas>, 3, 24, 81, 18>> bluePrintResize_Size_TextBox;
		private: KIR5::Shared<AdjustablePanel<BmpButton, 87, 24, 18, 18>> bluePrintResize_Apply_Button;
		private: std::array<std::array<std::shared_ptr<ResizeButtonClass>, 3>, 3> bluePrintResize_Navigation_Buttons{{
			{
				KIR5::Shared<AdjustablePanel<ResizeButtonClass, 3, 45, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<ResizeButtonClass, 3, 78, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<ResizeButtonClass, 3, 111, 32, 32>>(),
			},
			{
				KIR5::Shared<AdjustablePanel<ResizeButtonClass, 38, 45, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<ResizeButtonClass, 38, 78, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<ResizeButtonClass, 38, 111, 32, 32>>(),
			},
			{
				KIR5::Shared<AdjustablePanel<ResizeButtonClass, 73, 45, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<ResizeButtonClass, 73, 78, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<ResizeButtonClass, 73, 111, 32, 32>>(),
			},
		}};

		private: KIR5::Shared<AdjustPanel<Window, 229, 97, 249, 101>> s_objectPanel;

		private: KIR5::Shared<AdjustablePanel<Label<Res::Consolas>, 3, 3, 243, 18>> s_objectLabel;

		private: KIR5::Shared<AdjustablePanel<BlockIDPanel, 3, 24, 32, 32>> s_objectID_Panel;
		private: KIR5::Shared<AdjustablePanel<TextBox<Res::Consolas>, 38, 31, 67, 18>> s_objectID_TextBox;
		private: KIR5::Shared<AdjustablePanel<BmpButton, 108, 31, 18, 18>> s_objectApply_Button;

		private: KIR5::Shared<AdjustablePanel<TextBox<Res::Consolas>, 129, 31, 57, 18>> s_objectRate_TextBox;
		private: KIR5::Shared<AdjustablePanel<Label<Res::Consolas>, 188, 31, 18, 18>> s_objectRandomFill_Label;

		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 3, 59, 120, 18>> s_objectRandomFill_Button;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 3, 80, 120, 18>> s_objectFillFrame_Button;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 126, 59, 120, 18>> s_objectFillContent_Button;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 126, 80, 120, 18>> s_objectselectAllOfThisType_Button;

		private: KIR5::Shared<AdjustPanel<Window, 481, 149, 166, 121>> infotron_Panel;
		private: KIR5::Shared<AdjustablePanel<Label<Res::Consolas>, 3, 3, 158, 18>> infotron_Label;
		private: KIR5::Shared<AdjustablePanel<TextBox<Res::Consolas>, 3, 24, 64, 27>> infotron_ToCollect_TextBox;
		private: KIR5::Shared<AdjustablePanel<Label<Res::Consolas>, 68, 24, 64, 27>> infotron_Capacity_Label;
		private: std::vector<KIR5::Shared<AdjustablePanel<BlockIDPanel>>> infotron_Pickers_Buttons;

		private: KIR5::Shared<AdjustPanel<Window, 0, 0, 226, 197>> special_Panel;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 3, 3, 220, 21>> selectAllButton;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 3, 27, 220, 21>> gravityTurnOnOffButton;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 3, 51, 220, 21>> initExplodeButton;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 3, 75, 220, 21>> selectFriendlyButton;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 3, 99, 220, 21>> ramRepairButton;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 3, 124, 220, 21>> randomizeRamsButton;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 3, 149, 220, 21>> spawnPointButton;
		private: KIR5::Shared<AdjustablePanel<Button<Res::Consolas>, 3, 173, 220, 21>> globalGravityOnOffButton;

		private: KIR5::Shared<AdjustPanel<Window, 88, 200, 390, 70>> prevBlockPickers_Panel;
		private: std::array<KIR5::Shared<AdjustablePanel<BlockIDPanel>>, 24> prevBlockPickers;


		public: void pickID(Type::ID id);
		public: void setOperationMode();
		public: void updateBlocks();
		public: void SetMap(std::shared_ptr<Res::BluePrint> &bluePrint, std::shared_ptr<Matrix<SceneBlock<EditorObjectBase>>> &map);

		public: ControlPanel();
		public: ~ControlPanel();

	};
}