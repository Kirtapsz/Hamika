#pragma once

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_bitmap_target.h>

#include <memory>

#include "BluePrint.h"
#include "Font.h"
#include "Bitmap.h"

class BlueprintPanel: public KIR5::Panel
{
	public: bool redrawn = false;
	private: std::shared_ptr<BluePrint> bluePrint;
	private: KIR5::Event::FncSwap<FNC_DRAW_TYPE> FncDraw;
	private: Type::Size DrawSize;
	private: KIR5::Bitmap disp_bmp;
	public: float alpha = 255;

	public: void setBluePrint(const std::shared_ptr<BluePrint> &bluePrint);

	public: BlueprintPanel();
};
