#include "Resource.h"

#include "Bitmap.h"
#include "Account.h"
#include "Font.h"
#include "Worlds.h"

namespace Res
{
	const std::tuple<Res::Handler<MultitestInput>> MultitestInput::handlers{{false,nullptr,{},{},Res::Hash::none,Res::Crypto::none}};

	struct Objects: public Base
	{
		virtual bool initialize(std::uint32_t mode)
		{
			::Objects::RunInitializer();
			return true;
		}
		virtual void shutdown()
		{
		}
	} objects;

	struct _Base
	{
		Base *base;
		std::int32_t mode;
		bool initialized = false;
	};
	std::array<_Base, 11> resources{{
		{&bitmapBox, GAME | EDITOR | ITEST},
		{&tiles, GAME | EDITOR | ITEST},
		{&uielements, GAME | EDITOR | ITEST},

		{&MajorSnafu, GAME | EDITOR | ITEST},
		{&CalibriBold, GAME | EDITOR | ITEST},
		{&TimesNewRoman, GAME | EDITOR | ITEST},
		{&Consolas, GAME | EDITOR | ITEST},

		{&accounts, GAME},

		{&worlds, GAME | ITEST | TEST},
		{&testWorlds, ITEST | TEST},

		{&objects, GAME | EDITOR | ITEST}
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