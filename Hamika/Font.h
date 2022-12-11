#pragma once

#include "Resource.h"

#include <KIR/AL/KIR5_font.h>

namespace Res
{
	class Font: public DynamicSkeleton<std::size_t, KIR5::Font>
	{
		public: using DynamicSkeleton<std::size_t, KIR5::Font>::DynamicSkeleton;
		public: using DynamicSkeleton<std::size_t, KIR5::Font>::operator[];

		private: virtual KIR5::Font get(std::size_t &key) const;
		private: virtual void vadlidate(std::size_t &key) const;
	};

	extern Font MajorSnafu;
	extern Font CalibriBold;
	extern Font TimesNewRoman;
	extern Font Consolas;
}
