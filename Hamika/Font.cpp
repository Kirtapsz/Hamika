#include "Font.h"

namespace Font
{
	FontContainer::FontContainer(const char *fontPath):fontPath(fontPath)
	{

	}

	const KIR5::Font &FontContainer::operator[](int size)
	{
		auto f=fonts.find(size);
		if (f == fonts.end())
		{
			KIR5::Font newFont;
			newFont.load(fontPath.c_str(), size);
			fonts[size]=newFont;
			return fonts[size];
		}
		else
		{
			return f->second;
		}
	}

	FontContainer MajorSnafu("Hamika\\font\\MajorSnafu.ttf");
	FontContainer CalibriBold("Hamika\\font\\CalibriBold.ttf");
	FontContainer TimesNewRoman("Hamika\\font\\TimesNewRoman.ttf");
	FontContainer Consolas("Hamika\\font\\Consolas.ttf");
}