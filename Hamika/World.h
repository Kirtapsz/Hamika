#pragma once

#include "BluePrint.h"

namespace Res
{
	struct World: public Base
	{
		using Base::Base;

		KIR5::Bitmap thumbnail;
		std::string title{"UNKNOWN"};
		std::vector<std::shared_ptr<BluePrint>> bluePrints;

		struct IO_HamSt1: Record<
			FixedStringRecord<128>,
			VectorRecord<std::uint16_t, BluePrint::IO_HamSt1>
		>
		{
			constexpr static std::size_t title = 0;
			constexpr static std::size_t blueprints = 1;
		};
		void operator=(const IO_HamSt1 &record);
		operator IO_HamSt1() const;

		struct IO_HamBC: Record<
			EofVectorRecord<BluePrint::IO_HamBC>
		>
		{
			constexpr static std::size_t blueprints = 0;
		};
		void operator=(const IO_HamBC &record);
		operator IO_HamBC() const;

		struct IO_Orig: Record<
			EofVectorRecord<BluePrint::IO_Orig>
		>
		{
			constexpr static std::size_t blueprints = 0;
		};
		void operator=(const IO_Orig &record);
		operator IO_Orig() const;

		template <typename T>
		struct _Handler: Handler<T>
		{
			const std::string name;
		};


		const static std::tuple<
			World::_Handler<World::IO_HamSt1>,
			World::_Handler<World::IO_Orig>,
			World::_Handler<World::IO_HamBC>
		> handlers;


		public: virtual bool initialize(std::uint32_t mode);
	};
}