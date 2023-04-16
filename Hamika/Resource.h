#pragma once

#include "Record.h"

namespace Res
{
	template<typename R>
	struct Handler
	{
		typedef R RECORD_T;
		bool initLoad;
		std::function<bool(const KIR5::DynamicStream &stream)> isType;
		const std::vector<std::uint8_t> fingerprintL;
		const std::vector<std::uint8_t> fingerprintR;
		Hash::Type hashType;
		Crypto::Type cryptoType;
	};

	// Models
	struct Base
	{
		enum Enum
		{
			NONE = 0,

			FILE_MISSING_ALLOWED = 1 << 5,
		};
		const std::string path;
		Enum mode;

		inline Base(const std::string &path, Enum mode = NONE):
			path(KIR5::pathCombine(KIR5::getModuleDirectory(), path)),
			mode(mode)
		{

		}

		virtual bool initialize(std::uint32_t mode)
		{
			return true;
		}
		virtual void shutdown()
		{
		}
	};


	template<typename KEY, typename T>
	struct DynamicSkeleton: public Base
	{
		friend class Swapper;

		protected: std::map<KEY, T> map;

		public: using Base::Base;

		public: virtual void shutdown()
		{
			map.clear();
			Base::shutdown();
		}

		protected: struct Swapper
		{
			private: DynamicSkeleton<KEY, T> &container;
			private: T &t;

			public: inline Swapper(DynamicSkeleton<KEY, T> &container, T &t):
				container(container),
				t(t)
			{

			}

			public: inline ~Swapper()
			{
				container.free();
			}

			public: inline operator const T &() const
			{
				return t;
			}
			public: inline operator T &()
			{
				return t;
			}
		};


		private: inline void free()
		{
			for (auto it = map.begin(); it != map.end(); )
			{
				if ((*it).second.useCount() == 1)
				{
					it = map.erase(it);
					continue;
				}
				++it;
			}
		}

		protected: virtual T get(KEY &key) const = 0;
		protected: virtual void vadlidate(KEY &key) const = 0;

		public: const Swapper operator[](KEY &key)
		{
			auto f = map.find(key);
			if (f == map.end())
			{
				vadlidate(key);
				T t = get(key);
				map[key] = t;
				return Swapper(*this, t);
			}
			else
			{
				return Swapper(*this, f->second);
			}
		}
		public: Swapper operator[](KEY key)
		{
			auto f = map.find(key);
			if (f == map.end())
			{
				vadlidate(key);
				T t = get(key);
				map[key] = t;
				return Swapper(*this, t);
			}
			else
			{
				return Swapper(*this, f->second);
			}
		}
	};

	template<typename KEY, typename T>
	struct StaticSkeleton: public Base
	{
		protected: std::map<KEY, T> map;

		public: using Base::Base;

		public: virtual void shutdown()
		{
			map.clear();
			Base::shutdown();
		}

		protected: virtual T get(KEY &key) = 0;
		protected: virtual void vadlidate(KEY &key) const = 0;

		public: const T &operator[](KEY &key)
		{
			auto f = map.find(key);
			if (f == map.end())
			{
				vadlidate(key);
				T t = get(key);
				map[key] = t;
				return map[key];
			}
			else
			{
				return f->second;
			}
		}
		public: T &operator[](KEY key)
		{
			auto f = map.find(key);
			if (f == map.end())
			{
				vadlidate(key);
				T t = get(key);
				map[key] = t;
				return map[key];
			}
			else
			{
				return f->second;
			}
		}
	};

	enum InitFlags: std::uint32_t
	{
		GAME = 1 << 0,
		EDITOR = 1 << 1,
		BLUEPRINTS_GAME = 1 << 2,
		BLUEPRINTS_TEST = 1 << 3,
		RESET = 1 << 4,

		ALWAYS = GAME | EDITOR | BLUEPRINTS_GAME,
	};
	bool initialize(std::uint32_t editorMode);
	void shutdown();
}

#include "Resource.hpp"