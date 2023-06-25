#include "BlueprintPanel.h"

#include <KIR/AL/KIR5_blender.h>

namespace UI
{
	void BlueprintPanel::bluePrint(const std::shared_ptr<Res::BluePrint> &_bluePrint_)
	{
		_bluePrint = _bluePrint_;
		_redrawn = true;
	}
	void BlueprintPanel::color(const KIR5::Color &color_)
	{
		_color = color_;
	}
	void BlueprintPanel::redraw()
	{
		_redrawn = true;
	}

	BlueprintPanel::BlueprintPanel()
	{
		fncDraw.push_back(FNC_DRAW([&](FNC_DRAW_PARAMS) -> FNC_DRAW_RET
		{
			if (_redrawn && _bluePrint)
			{
				_redrawn = false;

				int smax = (std::max)(((Type::Size)(_bluePrint->blocks)).width, ((Type::Size)(_bluePrint->blocks)).height);
				_drawSize = {400 / smax, 400 / smax};

				_disp_bmp = al_create_bitmap(_drawSize.width * ((Type::Size)(_bluePrint->blocks)).width, _drawSize.height * ((Type::Size)(_bluePrint->blocks)).height);

				KIR5::BitmapTarget target(_disp_bmp);

				al_clear_to_color(KIR5::Color::_transparent);

				_bluePrint->blocks.foreach([&](const Type::Coord &coord, Res::BluePrint::Block &object)
				{
					DrawObject(object.id, coord.x * _drawSize.width, coord.y * _drawSize.height, _drawSize.width, _drawSize.height);
				});
			}
			{
				float smin = (std::min)(w_ / float(_disp_bmp.width()), h_ / float(_disp_bmp.height()));

				float w = _disp_bmp.width() * smin;
				float h = _disp_bmp.height() * smin;

				float x = x_ + (w_ - w) / 2.f;
				float y = y_ + (h_ - h) / 2.f;

				_disp_bmp.drawTintedScaled((int)x, (int)y, (int)w, (int)h, 0, 0, _disp_bmp.width(), _disp_bmp.height(), _color.getAlphaColored());
			}
		}));
	}
}
