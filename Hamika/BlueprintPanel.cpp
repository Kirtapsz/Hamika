#include "BlueprintPanel.h"

void BlueprintPanel::setBluePrint(const std::shared_ptr<BluePrint> &bluePrint)
{
	this->bluePrint = bluePrint;
	redrawn = true;
}

BlueprintPanel::BlueprintPanel()
{
	FncDraw.lock(fncDraw);
	FncDraw = [&](FNC_DRAW_PARAMS)
	{
		if (redrawn && bluePrint)
		{
			redrawn = false;
			printf("BlueprintPanel\n");

			int smax = (std::max)(((Type::Size)(bluePrint->blocks)).width, ((Type::Size)(bluePrint->blocks)).height);
			DrawSize = {400 / smax,400 / smax};

			disp_bmp = al_create_bitmap(DrawSize.width * ((Type::Size)(bluePrint->blocks)).width, DrawSize.height * ((Type::Size)(bluePrint->blocks)).height);

			KIR5::BitmapTarget target;

			target.lock(disp_bmp);

			al_clear_to_color(al_map_rgb(0, 0, 0));

			bluePrint->blocks.foreach([&](const Type::Coord &coord, BluePrint::Block &object)
			{
				DrawObject(object.id, coord.x * DrawSize.width, coord.y * DrawSize.height, DrawSize.width, DrawSize.height);
			});

			target.unlock();
		}
		{
			float smin = min(w_ / float(disp_bmp.width()), h_ / float(disp_bmp.height()));

			float w = disp_bmp.width() * smin;
			float h = disp_bmp.height() * smin;

			float x = x_ + (w_ - w) / 2.f;
			float y = y_ + (h_ - h) / 2.f;

			al_draw_filled_rectangle(x_, y_, x_ + w_, y_ + w_, KIR5::Color(0, 0, 0, min(20, alpha)).getAlphaColored());
			disp_bmp.drawTintedScaled((int)x, (int)y, (int)w, (int)h, 0, 0, disp_bmp.width(), disp_bmp.height(), KIR5::Color(255, 255, 255, alpha).getAlphaColored());
		}
	};
}
