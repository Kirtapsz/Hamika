#pragma once

#include "ActiveMap.h"
#include "MapDrawer.h"

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
	class MapList:public virtual KIR5::Panel
	{
		friend class MainEvent;
		struct PanelData
		{
			int x;
			int y;
			int w;
			float a;
		};
		PanelData paneldata[5];
		class Panel:public BlueprintPanel
		{
			float fx;
			float fy;
			float fw;
			int ID;
			KIR5::Font font;
			const std::vector<std::shared_ptr<Res::BluePrint>> *bluePrints = nullptr;
			public:
			Panel();
			PanelData source;
			inline void SetMapContainer(const std::vector<std::shared_ptr<Res::BluePrint>> *bluePrints);
			void SetFocus(int ID);
			inline void show();
			int GetFocus() const;
			void Restart();

			void resize(float w);
			void move(float x, float y);
			void position(float x, float y);
			inline float widthf();
			inline float virtual_xf();
			inline float virtual_yf();
			void align(int type);
		};
		KIR5::Shared<Panel> panel[5];
		Panel *panelp[5];
		inline void SetTop();
		//balról jobbra
		void MoveRight();
		//jobbról balra
		void MoveLeft();
		int MoveCount = 0;
		Res::World *world;
		int Focus = 0;
		int mx;
		int lastMC;
		int arrived = 2;
		double speed = 0.5;
		const static double Defspeed;
		bool MouseButtonDown = false;
		bool MouseAxes;
		public:
		void SetFocus(int Focus);
		MapList();
		~MapList();
		static constexpr unsigned __int32 MoveToRetZero = 0x00000000;
		static constexpr unsigned __int32 MoveToRetLast = 0xFFFFFFFF;
		float MoveTo(float dest, float source, float current, float cps);
		inline int GetFocus() const;
	};

	class MainEvent
	{
		private: class WorldButton: public Editor::UIbbutton<Editor::UI_M>
		{
			private: const std::string filename;
			public: WorldButton(const std::string &filename);
		};

		private: struct Menu: KIR5::Shared<RTEngine<Editor::ColoredPanel>>
		{
			struct Worlds: KIR5::Shared<RTpanel<KIR5::Row<KIR5::Panel, WorldButton>, 0, 168, 296, 64, 500, 400>>
			{
				Worlds();
			} worlds;
			struct Settings: KIR5::Shared<RTpanel<Editor::ColoredPanel, 0, 0, 296, 158, 500, 400>>
			{
				Settings();
			} settings;
			struct BluePrints: KIR5::Shared< RTpanel<MapList, 0, 242, 296, 158, 500, 400>>
			{
				BluePrints();
			} bluePrints;
			struct Accounts: KIR5::Shared<RTEngine<RTpanel<Editor::ColoredPanel, 306, 0, 194, 138, 500, 400>>>
			{
				struct List: KIR5::Shared< RTpanel<KIR5::Panel, 0, 0, 160, 138, 194, 138>>
				{
					KIR5::Shared<KIR5::ListPanel<KIR5::Panel, UI::Button<Res::Consolas>>> items;
					KIR5::Shared<KIR5::VerticalScrollBar<>> scrollBar;
					KIR5::Shared<KIR5::RectangleButton<>> scrollBarThumb;
					KIR5::Shared<KIR5::ScrollControl> scrollControl{new KIR5::ScrollControl(items, scrollBar)};

					List();
				} list;
				struct Create: KIR5::Shared< RTpanel<UI::BmpButton, 162, 106, 32, 32, 194, 138>>
				{
					Create();
				} create;

				Accounts();
				~Accounts();

				void refresh();
			} accounts;
			struct HirtoryOfBlueprint: KIR5::Shared< RTpanel<Editor::ColoredPanel, 306, 242, 194, 116, 500, 400>>
			{
				HirtoryOfBlueprint();
			} hirtoryOfBlueprint;
			struct CurrentAccount: KIR5::Shared< RTpanel<Editor::ColoredPanel, 306, 148, 194, 84, 500, 400>>
			{
				CurrentAccount();
			} currentAccount;
			struct CreateAccount: KIR5::Shared< RTEngine<RTpanel<Editor::ColoredPanel, 306, 148, 194, 84, 500, 400>>>
			{
				struct PanelLabel: KIR5::Shared< RTpanel<UI::Label<Res::Consolas>, 0, 0, 178, 16, 194, 84>>
				{
					PanelLabel();
				} panelLabel;
				struct: KIR5::Shared< RTpanel<UI::TextBox<Res::Consolas>, 55, 29, 135, 16, 194, 84>>
				{
				} usernameTextBox;
				struct: KIR5::Shared< RTpanel<UI::TextBox<Res::Consolas>, 55, 48, 135, 16, 194, 84>>
				{
				} passwrodTextBox;
				struct UsernameLabel: KIR5::Shared< RTpanel<UI::Label<Res::Consolas>, 4, 32, 46, 10, 194, 84>>
				{
					UsernameLabel();
				} usernameLabel;
				struct PasswrodLabel: KIR5::Shared< RTpanel<UI::Label<Res::Consolas>, 4, 51, 46, 10, 194, 84>>
				{
					PasswrodLabel();
				} passwrodLabel;
				struct CancelButton: KIR5::Shared< RTpanel<UI::ButtonX<Res::MajorSnafu>, 178, 0, 16, 16, 194, 84>>
				{
					CancelButton();
				} cancelButton;
				struct ApplyButton: KIR5::Shared< RTpanel<UI::Button<Res::Consolas>, 134, 68, 60, 16, 194, 84>>
				{
					ApplyButton();
				} applyButton;

				CreateAccount();
			} createAccount;

			struct PlayButton: KIR5::Shared< RTpanel<Editor::UIbbutton<Editor::UI_M>, 306, 368, 32, 32, 500, 400>>
			{
				PlayButton();
			} playButton;

			struct SkipButton: KIR5::Shared< RTpanel<Editor::UIbbutton<Editor::UI_M>, 348, 368, 32, 32, 500, 400>>
			{
				SkipButton();
			} skipButton;

			inline Menu()
			{
				(*get()) << worlds << settings;
				(*get()) << bluePrints;
				(*get()) << accounts << hirtoryOfBlueprint << currentAccount << createAccount << playButton << skipButton;
			}
		} menu;

		private: std::shared_ptr<KIR5::Panel> parent;

		private: std::shared_ptr<Res::Account> account;
		private: KIR5::Shared<ActiveMap> activeMap;
		private: KIR5::Shared<KIR5::RectangleButton<KIR5::Button<KIR5::TextBox<>>>> replayTextBox;

		public: MainEvent(std::shared_ptr<KIR5::Panel> parent);
		public: ~MainEvent();

		public: void mapFinished(bool victory, unsigned long long ticks);
	};

	extern std::shared_ptr<MainEvent> mainEvent;
	extern int maxBitmapSize;
}