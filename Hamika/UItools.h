#pragma once

#include <KIR/AL/KIR5_panel_tools.h>
#include <KIR/AL/KIR5_color.h>
#include <KIR\AL\KIR5_label.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>

#include "Font.h"

namespace UI
{
	template<typename T = KIR5::Panel, int X = 0, int Y = 0, int W = 0, int H = 0>
	class FlexiblePanel: public virtual T, public KIR5::FlexiblePanel::Base
	{
		public: FlexiblePanel(int x_, int y_, int w_, int h_):
			KIR5::FlexiblePanel::Base(std::make_shared<KIR5::FlexiblePanel::LinkedSize>(x_, y_, w_, h_))
		{

		}

		public: FlexiblePanel():
			KIR5::FlexiblePanel::Base(std::make_shared<KIR5::FlexiblePanel::LinkedSize>(X, Y, W, H))
		{

		}
	};

	template<typename T, int W, int H>
	class FlexiblePanelManager: public virtual T, public virtual KIR5::FlexiblePanel::Manager<W, H>
	{
	};

	template<typename T, int X, int Y, int W, int H>
	class FlexiblePanelAndManager: public virtual FlexiblePanel<T, X, Y, W, H>, public virtual FlexiblePanelManager<T, W, H>
	{
		public: FlexiblePanelAndManager(int x_, int y_):
			FlexiblePanel(x_, y_, W, H)
		{

		}

		public: FlexiblePanelAndManager()
		{

		}
	};

	class Panel: public virtual /*KIR5::ColoredPanel*/ KIR5::Panel
	{
		public: inline Panel()
		{
			show();
			//setBackgroundColor(KIR5::Color(rand() % 255, rand() % 255, rand() % 255));
		}
	};

	template<Res::Font &FONT, std::size_t D = 10>
	class TextBox: public virtual KIR5::TextBox<KIR5::RectangleButton<KIR5::Button<>>>
	{
		public: inline TextBox()
		{
			show();
			setTextColor(KIR5::Color(230, 120, 40));
			setColor(KIR5::Color(30, 30, 30));
			setTextAlignment(KIR5::LEFT | KIR5::VCENTER | KIR5::IGNORE_DESCENT);

			fncDraw.push_back(FNC_DRAW([&](FNC_DRAW_PARAMS) -> FNC_DRAW_RET
			{
				al_draw_line(x_, y_ + h_ - 1, x_ + w_, y_ + h_ - 1, KIR5::Color(230, 120, 40), 1.f);
			}));

			fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				setTextFont(FONT[height() / (D / 10.f) / 1.1f]);
			});
		}
	};
	template<Res::Font &FONT, std::size_t D = 10>
	class ButtonX: public virtual KIR5::TextButton<KIR5::Button<Panel>>
	{
		public: inline ButtonX()
		{
			show();
			setTextColor(KIR5::Color(255, 0, 0));
			setTextAlignment(KIR5::CENTER);
			setText("X");

			fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				setTextFont(FONT[height() / (D / 10.f) - 2]);
			});
		}
	};
	template<Res::Font &FONT, std::size_t D = 10>
	class Button: public virtual KIR5::TextButton<KIR5::RectangleButton<KIR5::Button<Panel>>>
	{
		public: inline Button()
		{
			show();
			setTextColor(KIR5::Color(10, 10, 10));
			setTextAlignment(KIR5::CENTER | KIR5::IGNORE_DESCENT);
			setColor(KIR5::Color(40, 40, 40));

			fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				setTextFont(FONT[height() / (D / 10.f) - 2]);
			});
		}
	};
	class BmpButton: public virtual KIR5::ColoredBitmapButton<KIR5::RectangleButton<KIR5::Button<Panel>>>
	{
		public: inline BmpButton()
		{
			show();
			setColor(KIR5::Color(30, 30, 30));
		}
	};
	class BmpoButton: public virtual KIR5::ColoredBitmapButton<KIR5::Button<Panel>>
	{
		public: inline BmpoButton()
		{
			show();
		}
	};
	template<Res::Font &FONT, std::size_t D = 10>
	class Label: public virtual KIR5::Label<Panel>
	{
		public: inline Label()
		{
			show();
			setTextColor(KIR5::Color(50, 50, 50));
			setTextAlignment(KIR5::LEFT | KIR5::VCENTER | KIR5::IGNORE_DESCENT);

			fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				setTextFont(FONT[height() / (D / 10.f) - 2]);
			});
		}
	};
	class Background: public virtual KIR5::ColoredPanel
	{
		public: inline Background()
		{
			show();
			setBackgroundColor(KIR5::Color(20, 20, 20));
		}
	};
	class Window: public virtual KIR5::ColoredPanel
	{
		public: inline Window()
		{
			show();
			setBackgroundColor(KIR5::Color(15, 15, 15));

			fncDraw.push_back(FNC_DRAW([&](FNC_DRAW_PARAMS) -> FNC_DRAW_RET
			{
				al_draw_rectangle(x_ + 1, y_ + 1, x_ + w_, y_ + h_, KIR5::Color(116, 109, 161), 1.f);
			}));
		}
	};

	template <int W = 0, int H = 0>
	class Line: public virtual KIR5::ColoredPanel
	{
		public: inline Line()
		{
			show();
			setBackgroundColor(KIR5::Color(160, 90, 30));
			resize(W, H);
		}
	};
}
