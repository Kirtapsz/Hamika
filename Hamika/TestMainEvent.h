#pragma once

#include "SceneConfiguration.h"
#include "SceneDrawer.h"
#include "MapList.h"

#include <KIR\AL\KIR5_event_engine.h>
#include <KIR/AL/KIR5_text_box.h>
#include <KIR/AL/KIR5_button.h>
#include <KIR/AL/KIR5_panel_control.h>
#include <KIR\AL\KIR5_check_box.h>

#include "Account.h"
#include "Font.h"
#include "Bitmap.h"
#include "UItools.h"
#include "BlueprintPanel.h"

namespace UI::Multitest
{
	class MainEvent
	{
		friend class MapList;

		private: std::int8_t speed;
		private: const std::list<std::string> replayPaths;
		private: std::list<std::string>::const_iterator replayIterator;
		private: std::shared_ptr<KIR5::Panel> parent;
		private: std::shared_ptr<Scene::InteractiveMultitest> scene_;

		private: MainEvent(const std::shared_ptr<KIR5::Panel> &_parent, const std::list<std::string> &replayPathsh, std::int8_t speed);
		public: static std::shared_ptr<MainEvent> s_object;
		public: static void initialize(std::shared_ptr<KIR5::Panel> _parent, const std::list<std::string> &replayPaths, std::int8_t speed);
		public: static void shutdown();
		public: ~MainEvent();

		private: void testGame(const std::string &replayPath);
		private: void finishGame();
	};
}