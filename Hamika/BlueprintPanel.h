#pragma once

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_bitmap_target.h>

#include <memory>

#include "World.h"
#include "Font.h"
#include "Bitmap.h"

class BlueprintPanel: public KIR5::Panel
{
	public: bool redrawn = false;
	private: std::shared_ptr<Res::BluePrint> bluePrint;
	private: Type::Size DrawSize;
	private: KIR5::Bitmap disp_bmp;
	public: float alpha = 255;

	public: void setBluePrint(const std::shared_ptr<Res::BluePrint> &bluePrint);

	public: BlueprintPanel();
};
