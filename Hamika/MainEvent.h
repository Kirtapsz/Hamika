#pragma once

#include "ActiveMap.h"
#include "MapDrawer.h"

#include <KIR\AL\KIR5_event_engine.h>
#include <KIR/AL/KIR5_text_box.h>
#include <KIR/AL/KIR5_button.h>

#include "Account.h"
#include "Font.h"
#include "Bitmap.h"
#include "BlueprintPanel.h"

class MapList:public KIR5::Panel
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
		private: KIR5::Event::FncSwap<FNC_DRAW_TYPE> FncDraw;

			   float fx;
			   float fy;
			   float fw;
			   int ID;
			   KIR5::Font font;
			   const std::vector<std::shared_ptr<BluePrint>> *bluePrints = nullptr;
		public:
		Panel();
		PanelData source;
		inline void SetMapContainer(const std::vector<std::shared_ptr<BluePrint>> *bluePrints);
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
	KIR5::EVENT<Panel> panel[5];
	Panel *panelp[5];
	inline void SetTop();
	//balr�l jobbra
	void MoveRight();
	//jobbr�l balra
	void MoveLeft();
	int MoveCount = 0;
	World world;
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

class MainEvent:public KIR5::Display
{
	private: std::shared_ptr<Account> account;
	private: KIR5::EVENT<ActiveMap> activeMap;
	private: KIR5::EVENT<MapList> originalMapList;
	private: KIR5::EVENT<KIR5::RectangleButton<KIR5::Button<KIR5::TextBox<>>>> replayTextBox;
	private: Slides cursorBmp;
	private: int cursorID = -1;
	private: float cursorIDf = 0;
	private: std::vector<ALLEGRO_MOUSE_CURSOR *> cursor;
	private: StackTimer cursorAnimator;

	public: MainEvent();
	public: ~MainEvent();

	public: void mapFinished(bool victory, unsigned long long ticks);
};

extern std::shared_ptr<MainEvent> mainEvent;
extern int maxBitmapSize;