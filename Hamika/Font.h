#pragma once

#include <KIR\AL\KIR5_font.h>
#include <map>

namespace Font
{
	class FontContainer
	{
		private: std::map<int, KIR5::Font> fonts;
		private: const std::string fontPath;

		public: FontContainer(const char *fontPath);

		public: const KIR5::Font &operator[](int size);
	};

	extern FontContainer MajorSnafu;
	extern FontContainer CalibriBold;
	extern FontContainer TimesNewRoman;
	extern FontContainer Consolas;
}