#pragma once

#include <KIR\AL\KIR5_event_engine.h>
#include <KIR\AL\KIR5_display.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>
#include <KIR/AL/KIR5_panel_control.h>

#include "EditorScene.h"
#include "EditorWorld.h"
#include "EditorMiniMap.h"
#include "EditorControlPanel.h"
#include "EditorSaveWorldDialog.h"
#include "UItools.h"

namespace Editor::UI
{
	using namespace Scene;
	class MainEvent
	{
		friend class Edit;
		friend class Worldi;
		friend class Menu;
		friend class MiniMap;
		friend class ControlPanel;
		friend class MapLoadPanel;
		friend class MapSavePanel;
		friend class SaveWorldDialog;

		private: std::shared_ptr<KIR5::Panel> parent;

		private: KIR5::Shared<KIR5::Column<>> columnControl;
		private: KIR5::Shared<KIR5::Row<>> rowsControl[3];

		private: KIR5::Shared<Edit> scene;
		private: KIR5::Shared<Worldi> worldi;
		private: KIR5::Shared<MiniMap> miniMap;
		public: KIR5::Shared<ControlPanel> controlPanel;
		private: KIR5::Shared<SaveWorldDialog> saveWorldDialog;

		public: MainEvent(std::shared_ptr<KIR5::Panel> parent);
		public: static std::shared_ptr<MainEvent> s_object;
		public: static void initialize(std::shared_ptr<KIR5::Panel> _parent);
		public: static void shutdown();
		public: ~MainEvent();
	};
}