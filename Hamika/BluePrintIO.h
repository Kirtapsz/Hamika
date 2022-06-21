#pragma once

#include "BluePrint.h"

class BluePrintIO
{
	public: virtual std::shared_ptr<BluePrint> read(std::vector<unsigned __int8> &data, size_t &bytesRead) = 0;
	public: virtual std::vector<unsigned __int8> write(const std::shared_ptr<BluePrint> &bluePrint, size_t &bytesWrite) = 0;
};

class OriginalLevelBluePrintIO
{
	static constexpr Type::ID transition[]
	{
		ObjectID::Space,
		ObjectID::Zonk,
		ObjectID::BaseX,
		ObjectID::Murphy,
		ObjectID::Infotron,
		ObjectID::RAMChipsMini,
		ObjectID::HardwareBrick1,
		ObjectID::Exit,
		ObjectID::Utility1,
		ObjectID::OneWayPortRight,
		ObjectID::OneWayPortDown,
		ObjectID::OneWayPortLeft,
		ObjectID::OneWayPortUp,
		ObjectID::OneWayPortRightGrav,
		ObjectID::OneWayPortDownGrav,
		ObjectID::OneWayPortLeftGrav,
		ObjectID::OneWayPortUpGrav,
		ObjectID::SnikSnakMove,
		ObjectID::Utility3,
		ObjectID::Terminal,
		ObjectID::Utility2,
		ObjectID::DualPortVertical,
		ObjectID::DualPortHorizontal,
		ObjectID::CrossPort,
		ObjectID::Electrons,
		ObjectID::Bug,
		ObjectID::RAMChipsLeft,
		ObjectID::RAMChipsRight,
		ObjectID::HardwareSwitch,
		ObjectID::HardwareGreenLed,
		ObjectID::HardwareBlueLed,
		ObjectID::HardwareRedLed,
		ObjectID::HardwareStripLine,
		ObjectID::RAMChipsTop,
		ObjectID::RAMChipsBottom,
		ObjectID::HardwareResource,
		ObjectID::HardwareRedCapacitor,
		ObjectID::HardwareResistance,
		ObjectID::HardwareBlueResistance,
		ObjectID::HardwareYellowResistance,
	};

	private: unsigned __int8 getOriginalID(Type::ID id);

	public: virtual std::shared_ptr<BluePrint> read(std::vector<unsigned __int8> &data, size_t &bytesRead);
	public: virtual std::vector<unsigned __int8> write(const std::shared_ptr<BluePrint> &bluePrint, size_t &bytesWrite);
};
