#pragma once

#include <KIR/AL/KIR5_panel_tools.h>
#include <KIR/AL/KIR5_color.h>
#include <KIR\AL\KIR5_label.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>

#include "Font.h"

namespace UI
{
	class AdjustParam
	{
		template<typename T, int W, int H>
		friend class AdjusterPanel;

		private: int _x;
		private: int _y;
		private: int _w;
		private: int _h;

		public: AdjustParam():
			_x(0), _y(0), _w(0), _h(0)
		{

		}
		public: AdjustParam(int x_, int y_, int w_, int h_):
			_x(x_), _y(y_), _w(w_), _h(h_)
		{

		}

		public: void setAdjustParams(int x_, int y_, int w_, int h_)
		{
			_x = x_;
			_y = y_;
			_w = w_;
			_h = h_;
		}
	};

	template<typename T = KIR5::Panel, int X = 0, int Y = 0, int W = 0, int H = 0>
	class AdjustablePanel: public virtual T, public AdjustParam
	{
		public: AdjustablePanel(int x_, int y_, int w_, int h_):
			AdjustParam(x_, y_, w_, h_)
		{

		}

		public: AdjustablePanel():
			AdjustParam(X, Y, W, H)
		{

		}
	};

	template<typename T, int W, int H>
	class AdjusterPanel: public virtual T
	{
		public: static constexpr int ADJUSTER_WIDTH = W;
		public: static constexpr int ADJUSTER_HEIGHT = H;

		public: inline AdjusterPanel()
		{
			resize(W, H);
			fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				float wr = width() / (float)W;
				float hr = height() / (float)H;
				float r = std::min(wr, hr);

				for (auto &it : getChildrens())
				{
					AdjustParam *aParam = dynamic_cast<AdjustParam *>(it.get());
					if (aParam)
					{
						KIR5::Panel *panel = dynamic_cast<KIR5::Panel *>(it.get());
						panel->move(
							(int)(aParam->_x * r),
							(int)(aParam->_y * r),
							(int)(aParam->_w * r),
							(int)(aParam->_h * r)
						);
					}
				}
			});
		}
	};

	template<typename T, int X, int Y, int W, int H>
	class AdjustPanel: public virtual AdjustablePanel<T, X, Y, W, H>, public virtual AdjusterPanel<T, W, H>
	{
		public: AdjustPanel(int x_, int y_):
			AdjustablePanel(x_, y_, W, H)
		{

		}

		public: AdjustPanel()
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
