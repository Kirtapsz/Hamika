#include "Font.h"

namespace Res
{
	KIR5::Font Font::get(std::size_t &key) const
	{
		KIR5::Font font;
		font.load(path.c_str(), key);
		return font;
	}
	void Font::vadlidate(std::size_t &key) const
	{
		if (key == 0)
		{
			key = 1;
		}
	}

	Font MajorSnafu("Hamika\\font\\MajorSnafu.ttf");
	Font CalibriBold("Hamika\\font\\CalibriBold.ttf");
	Font TimesNewRoman("Hamika\\font\\TimesNewRoman.ttf");
	Font Consolas("Hamika\\font\\Consolas.ttf");

};
