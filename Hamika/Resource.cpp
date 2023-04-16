#include "Resource.h"

#include "Bitmap.h"
#include "Account.h"
#include "Font.h"
#include "Worlds.h"

namespace Res
{
	struct _Base
	{
		Base *base;
		std::int32_t mode;
		bool initialized = false;
	};
	std::array<_Base, 9> resources{{
		{&bitmapBox, ALWAYS},
		{&tiles, GAME | EDITOR},
		{&uielements, GAME | EDITOR},

		{&MajorSnafu, GAME | EDITOR},
		{&CalibriBold, GAME | EDITOR},
		{&TimesNewRoman, GAME | EDITOR},
		{&Consolas, GAME | EDITOR},

		{&accounts, GAME | RESET},

		{&worlds, GAME | BLUEPRINTS_GAME},
	}};

	bool initialize(std::uint32_t mode)
	{
		for (auto &it : resources)
		{
			if (mode & it.mode)
			{
				it.initialized = it.base->initialize(mode);
				if (!it.initialized)
				{
					return false;
				}
			}
		}
		return true;
	}
	void shutdown()
	{
		for (auto &it : resources)
		{
			if (it.initialized)
			{
				it.base->shutdown();
			}
		}
	}
}