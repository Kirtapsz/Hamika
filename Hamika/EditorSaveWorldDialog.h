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
#include <KIR\AL\KIR5_check_box.h>

#include <memory>

namespace UI::Editor
{
	class SaveWorldDialog: public Panel
	{
		friend class MainEvent;

		private: KIR5::Shared<KIR5::ComboBox<
			Button<Res::Consolas, 20>,
			KIR5::ListPanel<Panel, Button<Res::Consolas, 20>>
			>> mapTypeSelector;
		private: KIR5::Shared<
			KIR5::SquaredCheckBox<
			KIR5::CheckBox<>::RoundedBox,
			Label<Res::Consolas, 20>>
			> jsonFormat;


		private: KIR5::Shared<Window> centerBox;
		private: KIR5::Shared<Label<Res::Consolas, 20>> directoryLabel;
		private: KIR5::Shared<TextBox<Res::Consolas, 20>> directory;
		private: KIR5::Shared<Label<Res::Consolas, 20>> wordNameDirectoryLabel;
		private: KIR5::Shared<TextBox<Res::Consolas, 20>> wordNameDirectory;
		private: KIR5::Shared<Button<Res::Consolas, 20>> saveButton;
		private: KIR5::Shared<Button<Res::Consolas, 20>> cancelButton;

		public: SaveWorldDialog();
		public: ~SaveWorldDialog();

		public: virtual void show();
	};
}