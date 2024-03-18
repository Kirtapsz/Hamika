#ifndef RES_JSON_HANDLER_HPP
#define RES_JSON_HANDLER_HPP

namespace Res::Json
{
	template <typename C, KIR5::StreamRecords::RecordType RT>
	struct JsonIO;

	struct JsonIO_
	{
		public:
		template<typename T>
		inline static void readJ(const ::Json &json, T &t)
		{
			if constexpr (KIR5::StreamTypes::IsFundamental<T>::value)
			{
				t = json.get<T>();
			}
			else
			{
				JsonIO<T, T::type>::readJ(json, t);
			}
		}

		public:
		template<typename T>
		inline static auto writeJ(const T &t)
		{
			if constexpr (KIR5::StreamTypes::IsFundamental<T>::value)
			{
				return t;
			}
			else
			{
				return JsonIO<T, T::type>::writeJ(t);
			}
		}
	};

	template <typename C>
	struct JsonIO<C, KIR5::StreamRecords::record_t::Record_t>
	{
		template<class... T>
		inline static void readJ(const ::Json &json, KIR5::StreamRecords::Record<T...> &t)
		{
			std::apply([&json](auto&&... args)
			{
				std::size_t index = 0;
				((JsonIO_::readJ(json[C::keys[index++]], args)), ...);
			}, static_cast<std::tuple<T...> &>(t)
				);
		}
		template<class... T>
		inline static ::Json writeJ(const KIR5::StreamRecords::Record<T...> &t)
		{
			::Json ret = ::Json::object();
			std::apply([&ret](auto&&... args)
			{
				std::size_t index = 0;
				((ret[C::keys[index++]] = JsonIO_::writeJ(args)), ...);
			}, static_cast<const std::tuple<T...> &>(t)
				);
			return ret;
		}
	};


	template <typename C>
	struct JsonIO<C, KIR5::StreamRecords::record_t::Vector_t>
	{
		template<typename T_SIZE, class T>
		inline static void readJ(const ::Json &json, KIR5::StreamRecords::Vector<T_SIZE, T> &t)
		{
			t.resize(static_cast<std::size_t>(json.size()));

			std::size_t idx = 0;
			for (auto &it : t)
			{
				JsonIO_::readJ(json[idx++], it);
			}
		}
		template<typename T_SIZE, class T>
		inline static ::Json writeJ(const KIR5::StreamRecords::Vector<T_SIZE, T> &t)
		{
			::Json ret = ::Json::array();
			for (auto &it : t)
			{
				ret.push_back(JsonIO_::writeJ(it));
			}
			return ret;
		}
	};

	template <typename C>
	struct JsonIO<C, KIR5::StreamRecords::record_t::TermString_t>
	{
		template<typename... Ts>
		inline static void readJ(const ::Json &json, KIR5::StreamRecords::TermString<Ts...> &t)
		{
			t = json.get<std::string>();
			//if (t.length() > COUNT)
			//{
			//	t = t.substr(0, COUNT);
			//}
		}
		template<typename... Ts>
		inline static ::Json writeJ(const KIR5::StreamRecords::TermString<Ts...> &t)
		{
			//std::string ret;
			//if (t.length() > COUNT)
			//{
			//	ret = t.substr(0, COUNT);
			//}
			//else
			//{
			//	ret = t;
			//}
			return t;
		}
	};

	template <typename C>
	struct JsonIO<C, KIR5::StreamRecords::record_t::FixString_t>
	{
		template<std::size_t COUNT, typename... Ts>
		inline static void readJ(const ::Json &json, KIR5::StreamRecords::FixString<COUNT, Ts...> &t)
		{
			t = json.get<std::string>();
			if (t.length() > COUNT)
			{
				t = t.substr(0, COUNT);
			}
		}
		template<std::size_t COUNT, typename... Ts>
		inline static ::Json writeJ(const KIR5::StreamRecords::FixString<COUNT, Ts...> &t)
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
	};

	template <typename C>
	struct JsonIO<C, KIR5::StreamRecords::record_t::Array_t>
	{
		template<std::size_t COUNT, class T>
		inline static void readJ(const ::Json &json, KIR5::StreamRecords::Array<COUNT, T> &t)
		{
			std::size_t idx = 0;
			for (auto &it : t)
			{
				JsonIO_::readJ(json[idx++], it);
			}
		}
		template<std::size_t COUNT, class T>
		inline static ::Json writeJ(const KIR5::StreamRecords::Array<COUNT, T> &t)
		{
			::Json ret = ::Json::array();
			for (auto &it : t)
			{
				ret.push_back(JsonIO_::writeJ(it));
			}
			return ret;
		}
	};

	template <typename C>
	struct JsonIO<C, KIR5::StreamRecords::record_t::FixVector_t>
	{
		template<std::size_t COUNT, class T>
		inline static void readJ(const ::Json &json, KIR5::StreamRecords::FixVector<COUNT, T> &t)
		{
			std::size_t idx = 0;
			for (auto &it : t)
			{
				JsonIO_::readJ(json[idx++], it);
			}
		}
		template<std::size_t COUNT, class T>
		inline static ::Json writeJ(const KIR5::StreamRecords::FixVector<COUNT, T> &t)
		{
			::Json ret = ::Json::array();
			for (auto &it : t)
			{
				ret.push_back(JsonIO_::writeJ(it));
			}
			return ret;
		}
	};

	template <typename C>
	struct JsonIO<C, KIR5::StreamRecords::record_t::SharedDerived_t>
	{
		template <typename T_BASE>
		struct PartIO
		{
			template <typename ...T>
			static constexpr T_BASE *readJ_(const ::Json &json)
			{
				std::int8_t type = json["ObjectType"].get<std::int8_t>();
				return readJ_<T...>(json, type);
			}

			template <typename T>
			static constexpr T_BASE *readJ_(const ::Json &json, std::int8_t &type)
			{
				if (type == 0)
				{
					T *t = new T();
					JsonIO_::readJ(json, *t);
					return t;
				}
				else
				{
					--type;
				}
				return nullptr;
			}

			template <typename T, typename N, typename... Args>
			static constexpr T_BASE *readJ_(const ::Json &json, std::int8_t &type)
			{
				T_BASE *base = readJ_<T>(json, type);
				if (base == nullptr)
				{
					base = readJ_<N, Args...>(json, type);
				}
				return base;
			}


			template <typename ...T>
			static constexpr ::Json writeJ_(T_BASE *base)
			{
				::Json ret;
				std::int8_t type = 0;
				writeJ_<T...>(ret, type, base);
				return ret;
			}

			template <typename T>
			static constexpr void *writeJ_(::Json &json, std::int8_t &type, T_BASE *base)
			{
				T *t = dynamic_cast<T *>(base);
				if (t != nullptr)
				{
					json = JsonIO_::writeJ(*t);
					json["ObjectType"] = type;
				}
				else
				{
					++type;
				}
				return t;
			}

			template <typename T, typename N, typename... Args>
			static constexpr void writeJ_(::Json &json, std::int8_t &type, T_BASE *base)
			{
				if (writeJ_<T>(json, type, base) == nullptr)
				{
					writeJ_<N, Args...>(json, type, base);
				}
			}
		};

		template<class T_BASE, class... T>
		inline static void readJ(const ::Json &json, KIR5::StreamRecords::SharedDerived<T_BASE, T...> &t)
		{
			std::int8_t type = json["ObjectType"].get<std::int8_t>();
			t = C(PartIO<T_BASE>::readJ_<T...>(json, type));
		}

		template<class T_BASE, class... T>
		inline static ::Json writeJ(const KIR5::StreamRecords::SharedDerived<T_BASE, T...> &t)
		{
			return PartIO<T_BASE>::writeJ_<T...>(t.get());
		}
	};

	template <typename C>
	struct JsonIO<C, record_t::MatrixRecord_t>
	{
		template<typename T_SIZE, class T>
		inline static void readJ(const ::Json &json, MatrixRecord<T_SIZE, T> &t)
		{
			T_SIZE width = static_cast<T_SIZE>(json.size());
			T_SIZE height = static_cast<T_SIZE>(json[0].size());
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
					JsonIO_::readJ(json[w][h++], it);
				}
				++w;
			}
		}
		template<typename T_SIZE, class T>
		inline static ::Json writeJ(const MatrixRecord<T_SIZE, T> &t)
		{
			::Json ret = ::Json::array();
			for (auto &c : t)
			{
				::Json a = ::Json::array();
				for (auto &it : c)
				{
					a.push_back(JsonIO_::writeJ(it));
				}
				ret.push_back(a);
			}
			return ret;
		}
	};

	template <typename C>
	struct JsonIO<C, KIR5::StreamRecords::record_t::FixMatrix_t>
	{
		template<std::size_t WIDTH, std::size_t HEIGHT, class T>
		inline static void readJ(const ::Json &json, KIR5::StreamRecords::FixMatrix<WIDTH, HEIGHT, T> &t)
		{
			std::size_t w = 0;
			for (auto &c : t)
			{
				std::size_t h = 0;
				for (auto &it : c)
				{
					JsonIO_::readJ(json[w][h++], it);
				}
				++w;
			}
		}
		template<std::size_t WIDTH, std::size_t HEIGHT, class T>
		inline static ::Json writeJ(const KIR5::StreamRecords::FixMatrix<WIDTH, HEIGHT, T> &t)
		{
			::Json ret = ::Json::array();
			for (auto &c : t)
			{
				::Json a = ::Json::array();
				for (auto &it : c)
				{
					a.push_back(JsonIO_::writeJ(it));
				}
				ret.push_back(a);
			}
			return ret;
		}
	};

	template <typename C>
	struct JsonIO<C, KIR5::StreamRecords::record_t::EofVector_t>
	{
		template<class T>
		inline static void readJ(const ::Json &json, KIR5::StreamRecords::EofVector<T> &t)
		{
			t.resize(static_cast<std::size_t>(json.size()));

			std::size_t idx = 0;
			for (auto &it : t)
			{
				JsonIO_::readJ(json[idx++], it);
			}
		}
		template<class T>
		inline static ::Json writeJ(const KIR5::StreamRecords::EofVector<T> &t)
		{
			::Json ret = ::Json::array();
			for (auto &it : t)
			{
				ret.push_back(JsonIO_::writeJ(it));
			}
			return ret;
		}
	};

	template <typename C>
	struct JsonIO<C, record_t::HashRecord_t>
	{
		template<class... T>
		inline static void readJ(const ::Json &json, HashRecord<T...> &t)
		{
			std::apply([&json](auto&&... args)
			{
				std::size_t index = 0;
				((JsonIO_::readJ(json[C::keys[index++]], args)), ...);
			}, static_cast<std::tuple<T...> &>(t)
				);
			t.hash = json["ObjectHash"];
		}
		template<class... T>
		inline static ::Json writeJ(const HashRecord<T...> &t)
		{
			::Json ret = ::Json::object();
			ret["ObjectHash"] = t.hash;
			std::apply([&ret](auto&&... args)
			{
				std::size_t index = 0;
				((ret[C::keys[index++]] = JsonIO_::writeJ(args)), ...);
			}, static_cast<const std::tuple<T...> &>(t)
				);
			return ret;
		}
	};

	template <typename C>
	struct JsonIO<C, record_t::JsonRecord_t>
	{
		inline static void readJ(const ::Json &json, JsonRecord &t)
		{
			t = json;
		}
		inline static ::Json writeJ(const JsonRecord &t)
		{
			return ::Json(static_cast<const ::Json &>(t));
		}
	};

	template<typename T, typename H>
	void LoadResource(T &resource, ::Json &json, H &handler)
	{
		H::RECORD_T record;
		JsonIO_::readJ(json, record);
		static_cast<T &>(resource) = record;
	}
	template<typename T, typename H>
	void SaveResource(T &resource, ::Json &json, H &handler)
	{
		json = JsonIO_::writeJ(static_cast<const H::RECORD_T &>(resource));
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