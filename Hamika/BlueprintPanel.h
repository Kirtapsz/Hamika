#pragma once

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_bitmap_target.h>

#include <memory>

#include "World.h"
#include "Font.h"
#include "Bitmap.h"
#include "UItools.h"

namespace UI
{
	class BlueprintPanel: public virtual Panel
	{
		private: bool _redrawn = false;
		private: std::shared_ptr<Res::BluePrint> _bluePrint;
		private: Type::Size _drawSize;
		private: KIR5::Bitmap _disp_bmp;
		private: KIR5::Color _color{255, 255, 255, 255};

		public: BlueprintPanel();
		public: void bluePrint(const std::shared_ptr<Res::BluePrint> &bluePrint_);
		public: void color(const KIR5::Color &color_ = KIR5::Color(255, 255, 255, 255));
		public: void redraw();
	};
}
