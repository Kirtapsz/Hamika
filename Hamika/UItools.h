#pragma once

#include <KIR/AL/KIR5_panel_tools.h>
#include <KIR/AL/KIR5_color.h>
#include <KIR\AL\KIR5_label.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>

#include "Font.h"


namespace UI
{
	class RPanel: public virtual KIR5::Panel
	{
		friend class REngine;
		template<typename T, int X, int Y, int W, int H, int RW, int RH>
		friend class RTpanel;
		private: int _x = 0; int _y = 0; int _w = 0; int _h = 0; float _rw = 0; float _rh = 0;
	};

	template<typename T, int X, int Y, int W, int H, int RW, int RH>
	class RTpanel: public virtual T, public virtual RPanel
	{
		public: inline RTpanel()
		{
			_x = X;
			_y = Y;
			_w = W;
			_h = H;
			_rw = RW;
			_rh = RH;
		}
	};

	class REngine: public virtual KIR5::Panel
	{
		public: inline REngine()
		{
			fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				for (auto &it : getChildrens())
				{
					RPanel *r_panel = dynamic_cast<RPanel *>(it.get());
					if (r_panel)
					{
						r_panel->move(
							(int)(r_panel->_x / r_panel->_rw * width()),
							(int)(r_panel->_y / r_panel->_rh * height()),
							(int)(r_panel->_w / r_panel->_rw * width()),
							(int)(r_panel->_h / r_panel->_rh * height())
						);
					}
				}
			});
		}
	};
	template<typename T>
	class RTEngine: public virtual T, public virtual REngine
	{
	};



	class Line: public KIR5::ColoredPanel
	{
		public: inline Line()
		{
			show();
			setBackgroundColor(KIR5::Color(160, 90, 30));
		}
	};
	template<Res::Font &FONT>
	class TextBox: public KIR5::TextBox<KIR5::RectangleButton<KIR5::Button<>>>
	{
		public: inline TextBox()
		{
			show();
			setTextColor(KIR5::Color(230, 120, 40));
			setColor(KIR5::Color(30, 30, 30));
			setTextAlignment(KIR5::LEFT | KIR5::VCENTER | KIR5::IGNORE_DESCENT);

			fncDraw.push_back(FNC_DRAW([&](FNC_DRAW_PARAMS)->FNC_DRAW_RET
			{
				al_draw_line(x_, y_ + h_ - 1, x_ + w_, y_ + h_ - 1, KIR5::Color(230, 120, 40), 1.f);
			}));

			fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				this->setTextFont(FONT[height() - 2]);
			});
		}
	};
	template<Res::Font &FONT>
	class ButtonX: public KIR5::TextButton<>
	{
		public: inline ButtonX()
		{
			show();
			setTextColor(KIR5::Color(255, 0, 0));
			setTextAlignment(KIR5::CENTER);
			setText("X");

			fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				this->setTextFont(FONT[height() - 2]);
			});
		}
	};
	template<Res::Font &FONT>
	class Button: public KIR5::TextButton<KIR5::FramedRectangleButton<>>
	{
		public: inline Button()
		{
			show();
			setTextColor(KIR5::Color(152, 152, 152));
			setTextAlignment(KIR5::CENTER | KIR5::IGNORE_DESCENT);
			setColor(KIR5::Color(30, 30, 30));
			setColorFrame(KIR5::Color(50, 50, 50));
			setFrameSize(2);

			fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				this->setTextFont(FONT[height() - 2]);
			});
		}
	};
	class BmpButton: public KIR5::ColoredBitmapButton<KIR5::RectangleButton<KIR5::Button<>>>
	{
		public: inline BmpButton()
		{
			show();
			setColor(KIR5::Color(30, 30, 30));
		}
	};
	template<Res::Font &FONT>
	class Label: public KIR5::Label<>
	{
		public: inline Label()
		{
			show();
			setTextColor(KIR5::Color(50, 50, 50));
			setTextAlignment(KIR5::LEFT | KIR5::VCENTER | KIR5::IGNORE_DESCENT);

			fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				this->setTextFont(FONT[height() - 2]);
			});
		}
	};
	class Background: public KIR5::ColoredPanel
	{
		public: inline Background()
		{
			show();
			setBackgroundColor(KIR5::Color(20, 20, 20));
		}
	};
	class Window: public KIR5::ColoredPanel
	{
		public: inline Window()
		{
			show();
			setBackgroundColor(KIR5::Color(15, 15, 15));
		}
	};
	class Panel: public KIR5::Panel
	{
		public: inline Panel()
		{
			show();
		}
	};
}

namespace UI::Editor
{
	class ColoredPanel: public KIR5::ColoredPanel
	{
		public: inline ColoredPanel()
		{
			setBackgroundColor(KIR5::Color(rand() % 256, rand() % 256, rand() % 256));
		}
	};

	struct UI_S
	{
		static constexpr int gap = 2;
		static constexpr int lineSize = 1;
		static constexpr int frameSize = 2;
		static constexpr int textSize = 14;
		static constexpr int dimension = 18;
	};

	struct UI_M
	{
		static constexpr int gap = 3;
		static constexpr int lineSize = 2;
		static constexpr int frameSize = 2;
		static constexpr int textSize = 24;
		static constexpr int dimension = 32;
	};

	struct UI_L
	{
		static constexpr int gap = 10;
		static constexpr int lineSize = 2;
		static constexpr int frameSize = 2;
		static constexpr int textSize = 30;
		static constexpr int dimension = 40;
	};

	template<typename UI_SIZE>
	class UIline: public Editor::ColoredPanel
	{
		public: inline UIline()
		{
			show();
			setBackgroundColor(KIR5::Color(160, 90, 30));
			resize(UI_SIZE::lineSize, UI_SIZE::lineSize);
		}
	};
	template<typename UI_SIZE>
	class UItextBox: public KIR5::TextBox<KIR5::RectangleButton<KIR5::Button<>>>
	{
		public: inline UItextBox()
		{
			show();
			resize(UI_SIZE::dimension, UI_SIZE::dimension);
			setTextFont(Res::Consolas[UI_SIZE::textSize]);
			setTextColor(KIR5::Color(230, 120, 40));
			setColor(KIR5::Color(30, 30, 30));
			setTextAlignment(KIR5::LEFT | KIR5::VCENTER);

			fncDraw.push_back(FNC_DRAW([&](FNC_DRAW_PARAMS)->FNC_DRAW_RET
			{
				al_draw_line(x_, y_ + h_ - 1, x_ + w_, y_ + h_ - 1, KIR5::Color(230, 120, 40), 1.f);
			}));
		}
	};
	template<typename UI_SIZE>
	class UIbutton: public KIR5::TextButton<KIR5::FramedRectangleButton<>>
	{
		public: inline UIbutton()
		{
			show();
			resize(UI_SIZE::dimension, UI_SIZE::dimension);
			setTextFont(Res::Consolas[UI_SIZE::textSize]);
			setTextColor(KIR5::Color(152, 152, 152));
			setTextAlignment(KIR5::CENTER);
			setColor(KIR5::Color(30, 30, 30));
			setColorFrame(KIR5::Color(50, 50, 50));
			setFrameSize(2);
		}
	};
	template<typename UI_SIZE>
	class UIbbutton: public KIR5::ColoredBitmapButton<KIR5::RectangleButton<KIR5::Button<>>>
	{
		public: inline UIbbutton()
		{
			resize(UI_SIZE::dimension, UI_SIZE::dimension);
			show();
			setColor(KIR5::Color(30, 30, 30));
		}
	};
	template<typename UI_SIZE>
	class UIlabel: public KIR5::Label<>
	{
		public: inline UIlabel()
		{
			show();
			resize(UI_SIZE::dimension, UI_SIZE::dimension);
			setTextColor(KIR5::Color(50, 50, 50));
			setTextFont(Res::Consolas[UI_SIZE::textSize]);
			setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
		}
	};

	template<typename UI_SIZE>
	class UIbackground: public Editor::ColoredPanel
	{
		public: inline UIbackground()
		{
			show();
			setBackgroundColor(KIR5::Color(20, 20, 20));
			resize(UI_SIZE::dimension, UI_SIZE::dimension);
		}
	};

	template<typename UI_SIZE>
	class UIwindow: public Editor::ColoredPanel
	{
		public: inline UIwindow()
		{
			show();
			setBackgroundColor(KIR5::Color(15, 15, 15));
			resize(UI_SIZE::dimension, UI_SIZE::dimension);
		}
	};

	template<typename UI_SIZE>
	class UIpanel: public KIR5::Panel
	{
		public: inline UIpanel()
		{
			show();
			resize(UI_SIZE::dimension, UI_SIZE::dimension);
		}
	};
}