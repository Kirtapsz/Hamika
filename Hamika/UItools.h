#pragma once

#include <KIR/AL/KIR5_panel_tools.h>
#include <KIR/AL/KIR5_color.h>
#include <KIR\AL\KIR5_label.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>

#include "Font.h"

namespace Editor
{
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
	class UIline: public KIR5::ColoredPanel
	{
		public: inline UIline()
		{
			show();
			setBackgroundColor(KIR5::Color(160, 90, 30));
			resize(UI_SIZE::lineSize, UI_SIZE::lineSize);
		}
	};
	template<typename UI_SIZE>
	class UItextBox: public KIR5::RectangleButton<KIR5::Button<KIR5::TextBox<>>>
	{
		private: KIR5::Event::FncRSwap<FNC_DRAW_TYPE> FncDraw;

		public: inline UItextBox()
		{
			show();
			resize(UI_SIZE::dimension, UI_SIZE::dimension);
			setTextFont(Font::Consolas[UI_SIZE::textSize]);
			setTextColor(KIR5::Color(230, 120, 40));
			setColor(KIR5::Color(30, 30, 30));
			setTextAlignment(KIR5::LEFT | KIR5::VCENTER);

			FncDraw.lock(fncDraw);
			FncDraw.set([&](FNC_DRAW_PARAMS)
			{
				al_draw_line(x_, y_ + h_ - 1, x_ + w_, y_ + h_ - 1, KIR5::Color(230, 120, 40), 1.f);
			});
		}
	};
	template<typename UI_SIZE>
	class UIbutton: public KIR5::FramedRectangleButton<KIR5::TextButton<>>
	{
		public: inline UIbutton()
		{
			show();
			resize(UI_SIZE::dimension, UI_SIZE::dimension);
			setTextFont(Font::Consolas[UI_SIZE::textSize]);
			setTextColor(KIR5::Color(152, 152, 152));
			setTextAlignment(KIR5::CENTER);
			setColor(KIR5::Color(30, 30, 30));
			setColorFrame(KIR5::Color(50, 50, 50));
			setFrameSize(2);
		}
	};
	template<typename UI_SIZE>
	class UIbbutton: public KIR5::RectangleButton<KIR5::ColoredBitmapButton<KIR5::Button<>>>
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
			setTextFont(Font::Consolas[UI_SIZE::textSize]);
			setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
		}
	};

	template<typename UI_SIZE>
	class UIbackground: public KIR5::ColoredPanel
	{
		public: inline UIbackground()
		{
			show();
			setBackgroundColor(KIR5::Color(20, 20, 20));
			resize(UI_SIZE::dimension, UI_SIZE::dimension);
		}
	};

	template<typename UI_SIZE>
	class UIwindow: public KIR5::ColoredPanel
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