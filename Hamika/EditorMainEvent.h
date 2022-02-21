#pragma once

#include <KIR\AL\KIR5_event_engine.h>
#include <KIR\AL\KIR5_display.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>
#include "EditorActiveMap.h"
#include "EditorMapList.h"
#include "EditorMiniMap.h"
#include "EditorControlPanel.h"


namespace Editor
{
	class MainEvent:public KIR5::Display
	{
		friend class ActiveMap;
		friend class MapList;
		friend class Menu;
		friend class MiniMap;
		friend class ControlPanel;
		friend class MapLoadPanel;
		friend class MapSavePanel;

		private: KIR5::EVENT<ActiveMap> activeMap;
		private: KIR5::EVENT<MapList> mapList;
		private: KIR5::EVENT<MiniMap> miniMap;
		private: KIR5::EVENT<ControlPanel> controlPanel;

		public: MainEvent();
		public: ~MainEvent();
	};

	extern std::shared_ptr<MainEvent> mainEvent;
}