#pragma once

#include "Record.h"
#include "Tools.h"

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
		mutable std::string path;
		Enum mode = NONE;

		inline Base()
		{

		}
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

	template <typename T, std::size_t N, const std::array<const char *, N> &K, const Res::Handler<T> &H>
	struct Container: public Base, public T
	{
		struct type_: public T
		{
			constexpr static std::array<const char *, N> keys{K};
		};

		using Base::Base;

		void operator=(const T &record)
		{
			*this = record;
		}
		operator T() const
		{
			return *this;
		}

		constexpr static std::tuple<Res::Handler<type_>> handlers;
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
		ITEST = 1 << 2,
		TEST = 1 << 3,
	};
	bool initialize(std::uint32_t editorMode);
	void shutdown();

	template <typename T>
	using record_cast = typename type_of_base_templates<T, Record, HashRecord>;
}

#include "BinaryResource.hpp"
#include "JsonResource.hpp"

namespace Res
{
	template<typename T>
	void LoadResource(T &resource)
	{
		if (endingWith(toLower(resource.path), ".json"))
		{
			Json::LoadResource_(resource);
		}
		else
		{
			LoadResource_(resource);
		}
	}
	template<typename T>
	void SaveResource(T &resource)
	{
		if (endingWith(toLower(resource.path), ".json"))
		{
			Json::SaveResource_(resource);
		}
		else
		{
			SaveResource_(resource);
		}
	}

	struct MultitestInput: public Base, public Res::Record<Res::VectorRecord<std::uint16_t, std::string>>
	{
		constexpr static std::array<const char *, 1> keys{"files"};

		using Base::Base;

		void operator=(const MultitestInput &record)
		{
			Res::Record<Res::VectorRecord<std::uint16_t, std::string>>::operator=(record);
		}
		operator const MultitestInput&() const
		{
			return *this;
		}

		const static std::tuple<Res::Handler<MultitestInput>> handlers;
	};
}