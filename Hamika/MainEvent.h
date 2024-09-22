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

namespace UI
{
	class MainEvent
	{
		friend class MapList;

		private: class WorldButton: public BmpButton
		{
			private: std::shared_ptr<Res::World> world_;
			public: WorldButton(const std::shared_ptr<Res::World> &_world);
		};

		private: struct Menu: public KIR5::Shared<AdjusterPanel<Panel, 500, 252>>
		{
			KIR5::Bitmap background;

			struct Worlds: KIR5::Shared<AdjustablePanel<KIR5::Row<Panel, WorldButton>, 0, 0, 296, 44>>
			{
				Worlds();
			} worlds;

			KIR5::Shared<AdjustablePanel<Label<Res::Consolas>, 0, 74, 248, 18>> worldTitle;

			struct OpenSettings: KIR5::Shared<AdjustablePanel<BmpoButton, 268, 56, 28, 28>>
			{
				OpenSettings();
			} openSettings;
			KIR5::Shared<AdjustablePanel<MapList, 0, 94, 336, 158>> bluePrints;
			struct BlueprintInfo: KIR5::Shared<AdjustPanel<Panel, 346, 94, 154, 130>>
			{
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas>, 5, 6, 22, 10>> ID;
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas>, 31, 6, 118, 10>> title;

				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas, 14>, 5, 25, 62, 10>> personalRecordLabel;
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas, 14>, 71, 25, 58, 10>> personalRecord;


				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas>, 57, 40, 90, 10>> hallOfFlameLabel;
				KIR5::Shared<AdjustablePanel<UI::Line<>, 5, 52, 144, 1>> hallOfFlameLine;

				struct HallOfFlameRow: AdjusterPanel<Panel, 144, 10>
				{
					std::string _replayUuid;
					KIR5::Shared<AdjustablePanel<UI::Button<Res::Consolas, 12>, 0, 0, 22, 10>> rank;
					KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas, 12>, 23, 0, 56, 10>> username;
					KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas, 12>, 80, 0, 64, 10>> time;
				};
				std::array<std::shared_ptr<AdjustablePanel<HallOfFlameRow>>, 5> hallOfFlameRows{
					KIR5::Shared<AdjustablePanel<HallOfFlameRow>>{5,58, 144, 10},
					KIR5::Shared<AdjustablePanel<HallOfFlameRow>>{5,72, 144, 10},
					KIR5::Shared<AdjustablePanel<HallOfFlameRow>>{5,86, 144, 10},
					KIR5::Shared<AdjustablePanel<HallOfFlameRow>>{5,100, 144, 10},
					KIR5::Shared<AdjustablePanel<HallOfFlameRow>>{5,114, 144, 10},
				};

				BlueprintInfo();
				void refresh();
			} blueprintInfo;
			struct Accounts: KIR5::Shared<AdjustPanel<Panel, 306, 0, 194, 84>>
			{
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas>, 5, 5, 168, 16>> panelLabel;
				KIR5::Shared<AdjustablePanel<UI::ButtonX<Res::MajorSnafu>, 173, 5, 16, 16>> cancelButton;

				struct List: KIR5::Shared<AdjustablePanel<Panel, 39, 21, 120, 58>>
				{
					KIR5::Shared<KIR5::ListPanel<Panel, UI::Button<Res::Consolas>>> items;
					KIR5::Shared<KIR5::VerticalScrollBar<>> scrollBar;
					KIR5::Shared<KIR5::RectangleButton<>> scrollBarThumb;
					KIR5::Shared<KIR5::ScrollControl> scrollControl{new KIR5::ScrollControl(items, scrollBar)};

					List();
				} list;
				struct Create: KIR5::Shared<AdjustablePanel<UI::BmpButton, 15, 47, 18, 18>>
				{
					Create();
				} create;

				Accounts();
				void refresh();
			} accounts;
			struct Settings: KIR5::Shared<AdjustPanel<Panel, 306, 0, 194, 84>>
			{
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas>, 5, 5, 168, 16>> panelLabel;
				KIR5::Shared<AdjustablePanel<UI::ButtonX<Res::MajorSnafu>, 173, 5, 16, 16>> cancelButton;

				Settings();
			} settings;
			struct CurrentAccount: KIR5::Shared<AdjustPanel<Panel, 306, 0, 194, 84>>
			{
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas>, 5, 5, 168, 16>> panelLabel;
				KIR5::Shared<AdjustablePanel<UI::ButtonX<Res::MajorSnafu>, 173, 5, 16, 16>> cancelButton;

				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas, 12>, 9, 32, 56, 10>> usernameLabel;
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas, 12>, 9, 46, 56, 10>> totalTimePlayedLabel;
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas, 12>, 9, 60, 56, 10>> numberOfCompletedMapsLabel;

				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas, 12>, 75, 32, 95, 10>> username;
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas, 12>, 75, 46, 95, 10>> totalTimePlayed;
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas, 12>, 75, 60, 95, 10>> numberOfCompletedMaps;

				CurrentAccount();
				void refresh();
			} currentAccount;
			struct CreateAccount: KIR5::Shared<AdjustPanel<Panel, 306, 0, 194, 84>>
			{
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas>, 5, 5, 168, 16>> panelLabel;
				KIR5::Shared<AdjustablePanel<UI::TextBox<Res::Consolas>, 59, 29, 121, 16>> usernameTextBox;
				KIR5::Shared<AdjustablePanel<UI::TextBox<Res::Consolas>, 59, 48, 121, 16>> passwrodTextBox;
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas>, 8, 32, 46, 10>> usernameLabel;
				KIR5::Shared<AdjustablePanel<UI::Label<Res::Consolas>, 8, 51, 46, 10>> passwrodLabel;
				KIR5::Shared<AdjustablePanel<UI::ButtonX<Res::MajorSnafu>, 173, 5, 16, 16>> cancelButton;
				KIR5::Shared<AdjustablePanel<UI::Button<Res::Consolas>, 175, 65, 14, 14>> applyButton;

				CreateAccount();
			} createAccount;

			inline Menu()
			{
				background = Res::uielements[Res::UIElements::MenuBackground];
				(*get()) << worlds << settings
					<< bluePrints << worldTitle << openSettings
					<< accounts << blueprintInfo << currentAccount << createAccount;

				get()->fncDraw.push_back([&](FNC_DRAW_PARAMS)
				{
					background.drawScaled(x_, y_, w_, h_);
				});
			}
		} menu;

		private: std::shared_ptr<KIR5::Panel> parent;
		private: std::shared_ptr<Res::Account> account_;
		private: std::shared_ptr<Scene::Base::Standard::Type> scene_;

		private: MainEvent(const std::shared_ptr<KIR5::Panel> &_parent);
		public: static std::shared_ptr<MainEvent> s_object;
		public: static void initialize(std::shared_ptr<KIR5::Panel> _parent);
		public: static void shutdown();
		public: ~MainEvent();

		private: void playGame(const std::shared_ptr<Res::BluePrint> &bluePrint_);
		private: void devGame(const std::shared_ptr<Res::BluePrint> &bluePrint_);
		private: void replayGame(const std::string &replayUuid);
		private: void finishGame(bool updateUI = true);

		private: void defaultDynamicPanel();
		private: void hideDynamicPanel();
		private: void setAccount(const std::shared_ptr<Res::Account> &_account);
	};
}