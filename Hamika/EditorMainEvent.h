#pragma once

#include <KIR\AL\KIR5_event_engine.h>
#include <KIR\AL\KIR5_display.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>
#include <KIR/AL/KIR5_panel_control.h>

#include "EditorActiveMap.h"
#include "EditorWorld.h"
#include "EditorMiniMap.h"
#include "EditorControlPanel.h"
#include "EditorSaveWorldDialog.h"
#include "UItools.h"


namespace Editor
{
	class MainEvent:public KIR5::Display
	{
		friend class ActiveMap;
		friend class Worldi;
		friend class Menu;
		friend class MiniMap;
		friend class ControlPanel;
		friend class MapLoadPanel;
		friend class MapSavePanel;
		friend class SaveWorldDialog;

		private: KIR5::EVENT<KIR5::Column<>> columnControl;
		private: KIR5::EVENT<KIR5::Row<>> rowsControl[3];

		private: KIR5::EVENT<ActiveMap> activeMap;
		private: KIR5::EVENT<Worldi> worldi;
		private: KIR5::EVENT<MiniMap> miniMap;
		private: KIR5::EVENT<ControlPanel> controlPanel;
		private: KIR5::EVENT<SaveWorldDialog> saveWorldDialog;

		public: MainEvent();
		public: ~MainEvent();
	};

	extern std::shared_ptr<MainEvent> mainEvent;
}