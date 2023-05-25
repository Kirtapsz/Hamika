#pragma once

#include "SceneConfiguration.h"
#include "SceneDrawer.h"

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
#include "World.h"

namespace UI
{
	class MapList:public virtual Panel
	{
		private: enum Status
		{
			Completed,
			Available,
			Unavailable
		};

		private: struct S_BlueprintPanel: public virtual AdjusterPanel<KIR5::Button<BlueprintPanel>, 178, 95>
		{
			std::int32_t index_ = -1;
			std::float_t fra = 0.f;
			std::float_t alpha = 255.f;
			bool setColor = true;
			KIR5::Shared<AdjustablePanel<BmpoButton, 89, 32, 30, 30>> button;
		};

		private: std::float_t currentTransition_ = 0.f;
		private: std::int32_t targetTransition_ = 0;
		private: std::int32_t middleIndex_ = 0;
		private: std::shared_ptr<Res::World> world_;
		private: std::shared_ptr<Res::Account> account_;
		private: std::vector<Status> worldStatus_;
		private: static constexpr std::int32_t bluePrintPanelSize = 7;
		private: static constexpr std::int32_t bluePrintPanelWingsize = bluePrintPanelSize / 2;
		private: std::array<KIR5::Shared<S_BlueprintPanel>, bluePrintPanelSize> bluePrintPanel;

		private: std::int32_t mouseHoldIndex = 0;
		private: std::int32_t mouseHoldX = 0;
		private: bool mouseOnHold = false;

		public: std::shared_ptr<Res::BluePrint> getBluePrint() const;
		private: std::shared_ptr<Res::BluePrint> getByIndex(std::int32_t _index) const;
		private: Status getStatus(std::int32_t index_) const;
		private: std::int32_t getNoOfBlueprints() const;
		public: void setWorld(const std::shared_ptr<Res::World> &_world);
		public: const std::shared_ptr<Res::World> &getWorld() const;
		public: void refresh();
		public: void setAccount(const std::shared_ptr<Res::Account> &_account);
		public: std::int32_t getIndex() const;
		public: void setIndex(std::int32_t _index);

			  // panels commands
		private: void move();
		private: void sort();
		private: void status();
		private: void reset();

		public: MapList();

	};
}