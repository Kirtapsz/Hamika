#pragma once

#include "Types.h"
#include "World.h"
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
#include <KIR\AL\KIR5_combo_box.h>

#include <memory>

namespace UI::Editor
{
	class SaveWorldDialog: public KIR5::Panel
	{
		friend class MainEvent;

		private: KIR5::Shared<KIR5::ComboBox<
			KIR5::FramedRectangleButton<KIR5::Button<KIR5::TextButton<>>>,
			KIR5::ListPanel<KIR5::Panel, KIR5::FramedRectangleButton<KIR5::Button<KIR5::TextButton<>>>>
			>> mapTypeSelector;

		private: KIR5::Shared<KIR5::ColoredPanel> centerBox;
		private: KIR5::Shared<KIR5::Label<KIR5::Panel>> directoryLabel;
		private: KIR5::Shared<KIR5::TextBox<KIR5::Panel>> directory;
		private: KIR5::Shared<KIR5::Label<KIR5::Panel>> wordNameDirectoryLabel;
		private: KIR5::Shared<KIR5::TextBox<KIR5::Panel>> wordNameDirectory;
		private: KIR5::Shared<KIR5::RectangleButton<KIR5::TextButton<>>> saveButton;
		private: KIR5::Shared<KIR5::RectangleButton<KIR5::TextButton<>>> cancelButton;

		public: SaveWorldDialog();
		public: ~SaveWorldDialog();

		public: virtual void show();
	};
}