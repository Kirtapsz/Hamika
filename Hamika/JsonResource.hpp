#ifndef RES_JSON_HANDLER_HPP
#define RES_JSON_HANDLER_HPP

namespace Res::Json
{
	template<typename C, typename T>
	inline void ReadFromJson_(const ::Json &json, T &t)
	{
		t = json.get<T>();
	}
	template<typename C, typename T>
	inline const T &WriteToJson_(const T &t)
	{
		return t;
	}

	template<typename C>
	inline void ReadFromJson_(const ::Json &json, ::Json &t)
	{
		t = json;
	}
	template<typename C>
	inline const ::Json &WriteToJson_(const ::Json &t)
	{
		return t;
	}

	template<typename C, std::size_t COUNT>
	inline void ReadFromJson_(const ::Json &json, TerminatedStringRecord<COUNT> &t)
	{
		t = json.get<std::string>();
		if (t.length() > COUNT)
		{
			t = t.substr(0, COUNT);
		}
	}
	template<typename C, std::size_t COUNT>
	inline std::string WriteToJson_(const TerminatedStringRecord<COUNT> &t)
	{
		std::string ret;
		if (t.length() > COUNT)
		{
			ret = t.substr(0, COUNT);
		}
		else
		{
			ret = t;
		}
		return t;
	}

	template<typename C, std::size_t COUNT>
	inline void ReadFromJson_(const ::Json &json, FixedStringRecord<COUNT> &t)
	{
		t = json.get<std::string>();
		if (t.length() > COUNT)
		{
			t = t.substr(0, COUNT);
		}
	}
	template<typename C, std::size_t COUNT>
	inline std::string WriteToJson_(const FixedStringRecord<COUNT> &t)
	{
		std::string ret;
		if (t.length() > COUNT)
		{
			ret = t.substr(0, COUNT);
		}
		else
		{
			ret = t;
		}
		return t;
	}


	template<typename C, typename T, std::size_t COUNT>
	inline void ReadFromJson_(const ::Json &json, FixedVectorRecord<T, COUNT> &t)
	{
		std::size_t idx = 0;
		for (auto &it : t)
		{
			ReadFromJson(json[idx++], it);
		}
	}
	template<typename C, typename T, std::size_t COUNT>
	inline ::Json WriteToJson_(const FixedVectorRecord<T, COUNT> &t)
	{
		::Json ret = ::Json::array();
		for (auto &it : t)
		{
			ret.push_back(WriteToJson(it));
		}
		return ret;
	}

	template<typename C, typename T, std::size_t COUNT>
	inline void ReadFromJson_(const ::Json &json, HeapVectorRecord<T, COUNT> &t)
	{
		std::size_t idx = 0;
		for (auto &it : t)
		{
			ReadFromJson(json[idx++], it);
		}
	}
	template<typename C, typename T, std::size_t COUNT>
	inline ::Json WriteToJson_(const HeapVectorRecord<T, COUNT> &t)
	{
		::Json ret = ::Json::array();
		for (auto &it : t)
		{
			ret.push_back(WriteToJson(it));
		}
		return ret;
	}

	template <typename C, class BASE, typename... Args> struct TypeList
	{
		static constexpr BASE *ReadFromJson__(const ::Json &json)
		{
			std::int8_t type = json["ObjectType"].get<std::int8_t>();
			return ReadFromJson__<Args...>(json, type);
		}

		template <typename T>
		static constexpr BASE *ReadFromJson__(const ::Json &json, std::int8_t &type)
		{
			if (type == 0)
			{
				T *t = new T();
				ReadFromJson(json, *t);
				return t;
			}
			else
			{
				--type;
			}
			return nullptr;
		}

		template <typename T, typename N, typename... Args>
		static constexpr BASE *ReadFromJson__(const ::Json &json, std::int8_t &type)
		{
			BASE *base = ReadFromJson__<T>(json, type);
			if (base == nullptr)
			{
				base = ReadFromJson__<N, Args...>(json, type);
			}
			return base;
		}


		static constexpr ::Json WriteToJson__(BASE *base)
		{
			::Json ret;
			std::int8_t type = 0;
			WriteToJson__<Args...>(ret, type, base);
			return ret;
		}

		template <typename T>
		static constexpr void *WriteToJson__(::Json &json, std::int8_t &type, BASE *base)
		{
			T *t = dynamic_cast<T *>(base);
			if (t != nullptr)
			{
				json = WriteToJson(*t);
				json["ObjectType"] = type;
			}
			else
			{
				++type;
			}
			return t;
		}

		template <typename T, typename N, typename... Args>
		static constexpr void WriteToJson__(::Json &json, std::int8_t &type, BASE *base)
		{
			if (WriteToJson__<T>(json, type, base) == nullptr)
			{
				WriteToJson__<N, Args...>(json, type, base);
			}
		}
	};

	template<typename C, class BASE, class I, class... T>
	inline void ReadFromJson_(const ::Json &json, SwitchVectorRecord<BASE, I, T...> &t)
	{
		t.resize(static_cast<std::size_t>(json.size()));

		std::size_t idx = 0;
		for (auto &it : t)
		{
			it = std::shared_ptr<BASE>(TypeList<C, BASE, T...>::ReadFromJson__(json[idx++]));
		}
	}
	template<typename C, class BASE, class I, class... T>
	inline ::Json WriteToJson_(const SwitchVectorRecord<BASE, I, T...> &t)
	{
		::Json ret = ::Json::array();
		for (auto &it : t)
		{
			ret.push_back(TypeList<C, BASE, T...>::WriteToJson__(it.get()));
		}
		return ret;
	}

	template<typename C, typename I, typename T>
	inline void ReadFromJson_(const ::Json &json, VectorRecord<I, T> &t)
	{
		t.resize(static_cast<std::size_t>(json.size()));

		std::size_t idx = 0;
		for (auto &it : t)
		{
			ReadFromJson(json[idx++], it);
		}
	}
	template<typename C, typename I, typename T>
	inline ::Json WriteToJson_(const VectorRecord<I, T> &t)
	{
		::Json ret = ::Json::array();
		for (auto &it : t)
		{
			ret.push_back(WriteToJson(it));
		}
		return ret;
	}

	template<typename C, typename I, typename T>
	inline void ReadFromJson_(const ::Json &json, MatrixRecord<I, T> &t)
	{
		I width = static_cast<I>(json.size());
		I height = static_cast<I>(json[0].size());
		t.resize({
			static_cast<std::size_t>(width),
			static_cast<std::size_t>(height)
				 });

		std::size_t w = 0;
		for (auto &c : t)
		{
			std::size_t h = 0;
			for (auto &it : c)
			{
				ReadFromJson(json[w][h++], it);
			}
			++w;
		}
	}
	template<typename C, typename I, typename T>
	inline ::Json WriteToJson_(const MatrixRecord<I, T> &t)
	{
		::Json ret = ::Json::array();
		for (auto &c : t)
		{
			::Json a = ::Json::array();
			for (auto &it : c)
			{
				a.push_back(WriteToJson(it));
			}
			ret.push_back(a);
		}
		return ret;
	}

	template<typename C, class T, std::size_t WIDTH, std::size_t HEIGHT>
	inline void ReadFromJson_(const ::Json &json, FixedMatrixRecord<T, WIDTH, HEIGHT> &t)
	{
		std::size_t w = 0;
		for (auto &c : t)
		{
			std::size_t h = 0;
			for (auto &it : c)
			{
				ReadFromJson(json[w][h++], it);
			}
			++w;
		}
	}
	template<typename C, class T, std::size_t WIDTH, std::size_t HEIGHT>
	inline ::Json WriteToJson_(const FixedMatrixRecord<T, WIDTH, HEIGHT> &t)
	{
		::Json ret = ::Json::array();
		for (auto &c : t)
		{
			::Json a = ::Json::array();
			for (auto &it : c)
			{
				a.push_back(WriteToJson(it));
			}
			ret.push_back(a);
		}
		return ret;
	}

	template<typename C, class T>
	inline void ReadFromJson_(const ::Json &json, EofVectorRecord<T> &t)
	{
		t.resize(static_cast<std::size_t>(json.size()));

		std::size_t idx = 0;
		for (auto &it : t)
		{
			ReadFromJson(json[idx++], it);
		}
	}
	template<typename C, class T>
	inline ::Json WriteToJson_(const EofVectorRecord<T> &t)
	{
		::Json ret = ::Json::array();
		for (auto &it : t)
		{
			ret.push_back(WriteToJson(it));
		}
		return ret;
	}

	template<typename C, typename ...T>
	inline void ReadFromJson_(const ::Json &json, HashRecord<T...> &t)
	{
		std::apply([&json](auto&&... args)
		{
			std::size_t index = 0;
			((ReadFromJson(json[C::keys[index++]], args)), ...);
		}, static_cast<std::tuple<T...> &>(t)
			);
		t.hash = json["ObjectHash"];
	}
	template<typename C, typename ...T>
	inline ::Json WriteToJson_(const HashRecord<T...> &t)
	{
		::Json ret = ::Json::object();
		ret["ObjectHash"] = t.hash;
		std::apply([&ret](auto&&... args)
		{
			std::size_t index = 0;
			((ret[C::keys[index++]] = WriteToJson(args)), ...);
		}, static_cast<const std::tuple<T...> &>(t)
			);
		return ret;
	}

	template<typename C, typename ...T>
	inline void ReadFromJson_(const ::Json &json, Record<T...> &t)
	{
		std::apply([&json](auto&&... args)
		{
			std::size_t index = 0;
			((ReadFromJson(json[C::keys[index++]], args)), ...);
		}, static_cast<std::tuple<T...> &>(t)
			);
	}
	template<typename C, typename ...T>
	inline ::Json WriteToJson_(const Record<T...> &t)
	{
		::Json ret = ::Json::object();
		std::apply([&ret](auto&&... args)
		{
			std::size_t index = 0;
			((ret[C::keys[index++]] = WriteToJson(args)), ...);
		}, static_cast<const std::tuple<T...> &>(t)
			);
		return ret;
	}

	template<typename C>
	inline void ReadFromJson(const ::Json &json, C &t)
	{
		ReadFromJson_<C>(json, static_cast<record_cast<C> &>(t));
	}
	template<typename C>
	inline ::Json WriteToJson(const C &t)
	{
		return WriteToJson_<C>(static_cast<const record_cast<C> &>(t));
	}


	template<typename T, typename H>
	void LoadResource(T &resource, ::Json &json, H &handler)
	{
		H::RECORD_T record;
		ReadFromJson(json, record);
		static_cast<T &>(resource) = record;
	}
	template<typename T, typename H>
	void SaveResource(T &resource, ::Json &json, H &handler)
	{
		json = WriteToJson(static_cast<const H::RECORD_T &>(resource));
	}


	template<typename T, typename H>
	void SaveResource(T &resource, ::Json &json, H &handler, bool &done)
	{
		if (!done)
		{
			try
			{
				SaveResource(resource, json, handler);
				done = true;
			}
			catch (const Exception::Missmatch &)
			{

			}
		}
	}


	template<typename T, typename H>
	void LoadResource(T &resource, ::Json &json, H &handler, bool &done)
	{
		if (!done)
		{
			try
			{
				LoadResource(resource, json, handler);
				done = true;
			}
			catch (const Exception::Missmatch &)
			{
			}
		}
	}

	template<typename T>
	void LoadResource_(T &resource)
	{
		std::ifstream iFile(resource.path);
		::Json json = ::Json::parse(iFile);
		if (iFile.bad())
		{
			if ((resource.mode & Base::FILE_MISSING_ALLOWED) != 0)
			{
				throw Exception::FileIO();
			}
			else
			{
				throw Exception::FileIO();
			}
		}

		std::apply([&resource, &json](auto&&... args)
		{
			bool done = false;
			((LoadResource(resource, json, args, done)), ...);
			if (!done)
			{
				throw Exception::MissingHandler();
			}
		}, T::handlers);
	}
	template<typename T>
	void SaveResource_(T &resource)
	{
		::Json json;

		std::apply([&resource, &json](auto&&... args)
		{
			bool done = false;
			((SaveResource(resource, json, args, done)), ...);
			if (!done)
			{
				throw Exception::MissingHandler();
			}
		}, T::handlers);

		std::ofstream oFile(resource.path);
		oFile << std::setw(4) << json << std::endl;
		if (oFile.bad())
		{
			throw Exception::FileIO();
		}
	}
}

#endif