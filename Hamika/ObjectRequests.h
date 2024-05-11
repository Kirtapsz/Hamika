#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

namespace Object
{
	namespace Module
	{
		namespace Requests
		{
			struct Data
			{
				struct Requests
				{
					bool timer;
					bool tick;
					bool update;
					bool finalize;
					bool draw;
					bool remove;

					void clear();

				} requests = {0};
			};

			template<typename MODULES_T>
			struct Func
			{
				private: typename MODULES_T::DATA_T &data_;
				private: typename MODULES_T::FUNC_T &func_;
				public: Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func);

				public: void __init__(Type::ID id, Type::Coord coord);
				public: Json print();
			};
		}
	}
}

#include "ObjectRequests.hpp"