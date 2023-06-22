#include "Objects.h"
#include "Murphy.h"

#include "OriginalObjects.h"
#include "EditorObjects.h"
#include <KIR/KIR4_console.h>

namespace Objects
{
	using namespace Object;
	struct Container
	{
		const char *name;
		Brick::PRINT printFnc;
		Brick::INITIALIZER initializerFnc;
		Brick::DRAWNER drawnerFnc;
		Brick::CREATER createrFnc;
		Brick::TIMER timerFnc;
		Brick::UPDATE updaterFnc;
		SIMPLE_DRAWNER simpleDrawnerFnc;
	} objects[] = {
		{Space_000::name, Space_000::Print, Space_000::Initializer, Space_000::Drawner, Space_000::Create, Space_000::Timer, Space_000::Update, Space_000::simpleDraw},
		{Zonk_001::name, Zonk_001::Print, Zonk_001::Initializer, Zonk_001::Drawner, Zonk_001::Create, Zonk_001::Timer, Zonk_001::Update, Zonk_001::simpleDraw},
		{BaseX_002::name, BaseX_002::Print, BaseX_002::Initializer, BaseX_002::Drawner, BaseX_002::Create, BaseX_002::Timer, BaseX_002::Update, BaseX_002::simpleDraw},
		{HardwareBlueLed_003::name, HardwareBlueLed_003::Print, HardwareBlueLed_003::Initializer, HardwareBlueLed_003::Drawner, HardwareBlueLed_003::Create, HardwareBlueLed_003::Timer, HardwareBlueLed_003::Update, HardwareBlueLed_003::simpleDraw},
		{HardwareBrick1_004::name, HardwareBrick1_004::Print, HardwareBrick1_004::Initializer, HardwareBrick1_004::Drawner, HardwareBrick1_004::Create, HardwareBrick1_004::Timer, HardwareBrick1_004::Update, HardwareBrick1_004::simpleDraw},
		{HardwareBlueResistance_005::name, HardwareBlueResistance_005::Print, HardwareBlueResistance_005::Initializer, HardwareBlueResistance_005::Drawner, HardwareBlueResistance_005::Create, HardwareBlueResistance_005::Timer, HardwareBlueResistance_005::Update, HardwareBlueResistance_005::simpleDraw},
		{HardwareGreenLed_006::name, HardwareGreenLed_006::Print, HardwareGreenLed_006::Initializer, HardwareGreenLed_006::Drawner, HardwareGreenLed_006::Create, HardwareGreenLed_006::Timer, HardwareGreenLed_006::Update, HardwareGreenLed_006::simpleDraw},
		{HardwareRedLed_007::name, HardwareRedLed_007::Print, HardwareRedLed_007::Initializer, HardwareRedLed_007::Drawner, HardwareRedLed_007::Create, HardwareRedLed_007::Timer, HardwareRedLed_007::Update, HardwareRedLed_007::simpleDraw},
		{HardwareResource_008::name, HardwareResource_008::Print, HardwareResource_008::Initializer, HardwareResource_008::Drawner, HardwareResource_008::Create, HardwareResource_008::Timer, HardwareResource_008::Update, HardwareResource_008::simpleDraw},
		{HardwareResistance_009::name, HardwareResistance_009::Print, HardwareResistance_009::Initializer, HardwareResistance_009::Drawner, HardwareResistance_009::Create, HardwareResistance_009::Timer, HardwareResistance_009::Update, HardwareResistance_009::simpleDraw},
		{HardwareStripLine_010::name, HardwareStripLine_010::Print, HardwareStripLine_010::Initializer, HardwareStripLine_010::Drawner, HardwareStripLine_010::Create, HardwareStripLine_010::Timer, HardwareStripLine_010::Update, HardwareStripLine_010::simpleDraw},
		{HardwareYellowResistance_011::name, HardwareYellowResistance_011::Print, HardwareYellowResistance_011::Initializer, HardwareYellowResistance_011::Drawner, HardwareYellowResistance_011::Create, HardwareYellowResistance_011::Timer, HardwareYellowResistance_011::Update, HardwareYellowResistance_011::simpleDraw},
		{HardwareRedCapacitor_012::name, HardwareRedCapacitor_012::Print, HardwareRedCapacitor_012::Initializer, HardwareRedCapacitor_012::Drawner, HardwareRedCapacitor_012::Create, HardwareRedCapacitor_012::Timer, HardwareRedCapacitor_012::Update, HardwareRedCapacitor_012::simpleDraw},
		{Bug_013::name, Bug_013::Print, Bug_013::Initializer, Bug_013::Drawner, Bug_013::Create, Bug_013::Timer, Bug_013::Update, Bug_013::simpleDraw},
		{Infotron_014::name, Infotron_014::Print, Infotron_014::Initializer, Infotron_014::Drawner, Infotron_014::Create, Infotron_014::Timer, Infotron_014::Update, Infotron_014::simpleDraw},
		{Exit_015::name, Exit_015::Print, Exit_015::Initializer, Exit_015::Drawner, Exit_015::Create, Exit_015::Timer, Exit_015::Update, Exit_015::simpleDraw},
		{CrossPort_016::name, CrossPort_016::Print, CrossPort_016::Initializer, CrossPort_016::Drawner, CrossPort_016::Create, CrossPort_016::Timer, CrossPort_016::Update, CrossPort_016::simpleDraw},
		{DualPortHorizontal_017::name, DualPortHorizontal_017::Print, DualPortHorizontal_017::Initializer, DualPortHorizontal_017::Drawner, DualPortHorizontal_017::Create, DualPortHorizontal_017::Timer, DualPortHorizontal_017::Update, DualPortHorizontal_017::simpleDraw},
		{DualPortVertical_018::name, DualPortVertical_018::Print, DualPortVertical_018::Initializer, DualPortVertical_018::Drawner, DualPortVertical_018::Create, DualPortVertical_018::Timer, DualPortVertical_018::Update, DualPortVertical_018::simpleDraw},
		{OneWayPortDown_019::name, OneWayPortDown_019::Print, OneWayPortDown_019::Initializer, OneWayPortDown_019::Drawner, OneWayPortDown_019::Create, OneWayPortDown_019::Timer, OneWayPortDown_019::Update, OneWayPortDown_019::simpleDraw},
		{OneWayPortLeft_020::name, OneWayPortLeft_020::Print, OneWayPortLeft_020::Initializer, OneWayPortLeft_020::Drawner, OneWayPortLeft_020::Create, OneWayPortLeft_020::Timer, OneWayPortLeft_020::Update, OneWayPortLeft_020::simpleDraw},
		{OneWayPortRight_021::name, OneWayPortRight_021::Print, OneWayPortRight_021::Initializer, OneWayPortRight_021::Drawner, OneWayPortRight_021::Create, OneWayPortRight_021::Timer, OneWayPortRight_021::Update, OneWayPortRight_021::simpleDraw},
		{OneWayPortUp_022::name, OneWayPortUp_022::Print, OneWayPortUp_022::Initializer, OneWayPortUp_022::Drawner, OneWayPortUp_022::Create, OneWayPortUp_022::Timer, OneWayPortUp_022::Update, OneWayPortUp_022::simpleDraw},
		{RAMChipsMini_023::name, RAMChipsMini_023::Print, RAMChipsMini_023::Initializer, RAMChipsMini_023::Drawner, RAMChipsMini_023::Create, RAMChipsMini_023::Timer, RAMChipsMini_023::Update, RAMChipsMini_023::simpleDraw},
		{RAMChipsLeft_024::name, RAMChipsLeft_024::Print, RAMChipsLeft_024::Initializer, RAMChipsLeft_024::Drawner, RAMChipsLeft_024::Create, RAMChipsLeft_024::Timer, RAMChipsLeft_024::Update, RAMChipsLeft_024::simpleDraw},
		{RAMChipsRight_025::name, RAMChipsRight_025::Print, RAMChipsRight_025::Initializer, RAMChipsRight_025::Drawner, RAMChipsRight_025::Create, RAMChipsRight_025::Timer, RAMChipsRight_025::Update, RAMChipsRight_025::simpleDraw},
		{Electrons_026::name, Electrons_026::Print, Electrons_026::Initializer, Electrons_026::Drawner, Electrons_026::Create, Electrons_026::Timer, Electrons_026::Update, Electrons_026::simpleDraw},
		{SnikSnakMove_027::name, SnikSnakMove_027::Print, SnikSnakMove_027::Initializer, SnikSnakMove_027::Drawner, SnikSnakMove_027::Create, SnikSnakMove_027::Timer, SnikSnakMove_027::Update, SnikSnakMove_027::simpleDraw},
		{Terminal_028::name, Terminal_028::Print, Terminal_028::Initializer, Terminal_028::Drawner, Terminal_028::Create, Terminal_028::Timer, Terminal_028::Update, Terminal_028::simpleDraw},
		{Utility1_029::name, Utility1_029::Print, Utility1_029::Initializer, Utility1_029::Drawner, Utility1_029::Create, Utility1_029::Timer, Utility1_029::Update, Utility1_029::simpleDraw},
		{Utility2_030::name, Utility2_030::Print, Utility2_030::Initializer, Utility2_030::Drawner, Utility2_030::Create, Utility2_030::Timer, Utility2_030::Update, Utility2_030::simpleDraw},
		{Utility3_031::name, Utility3_031::Print, Utility3_031::Initializer, Utility3_031::Drawner, Utility3_031::Create, Utility3_031::Timer, Utility3_031::Update, Utility3_031::simpleDraw},
		{ExplosionEffect_032::name, ExplosionEffect_032::Print, ExplosionEffect_032::Initializer, ExplosionEffect_032::Drawner, ExplosionEffect_032::Create, ExplosionEffect_032::Timer, ExplosionEffect_032::Update, ExplosionEffect_032::simpleDraw},
		{Explosion_033::name, Explosion_033::Print, Explosion_033::Initializer, Explosion_033::Drawner, Explosion_033::Create, Explosion_033::Timer, Explosion_033::Update, Explosion_033::simpleDraw},
		{Unknown::name, Unknown::Print, Unknown::Initializer, Unknown::Drawner, Unknown::Create, Unknown::Timer, Unknown::Update, Unknown::simpleDraw},
		{RAMChipsTop_035::name, RAMChipsTop_035::Print, RAMChipsTop_035::Initializer, RAMChipsTop_035::Drawner, RAMChipsTop_035::Create, RAMChipsTop_035::Timer, RAMChipsTop_035::Update, RAMChipsTop_035::simpleDraw},
		{RAMChipsBottom_036::name, RAMChipsBottom_036::Print, RAMChipsBottom_036::Initializer, RAMChipsBottom_036::Drawner, RAMChipsBottom_036::Create, RAMChipsBottom_036::Timer, RAMChipsBottom_036::Update, RAMChipsBottom_036::simpleDraw},
		{HardwareSwitch_037::name, HardwareSwitch_037::Print, HardwareSwitch_037::Initializer, HardwareSwitch_037::Drawner, HardwareSwitch_037::Create, HardwareSwitch_037::Timer, HardwareSwitch_037::Update, HardwareSwitch_037::simpleDraw},
		{OneWayPortDownGrav_038::name, OneWayPortDownGrav_038::Print, OneWayPortDownGrav_038::Initializer, OneWayPortDownGrav_038::Drawner, OneWayPortDown_019::Create, OneWayPortDown_019::Timer, OneWayPortDown_019::Update, OneWayPortDownGrav_038::simpleDraw},
		{OneWayPortLeftGrav_039::name, OneWayPortLeftGrav_039::Print, OneWayPortLeftGrav_039::Initializer, OneWayPortLeftGrav_039::Drawner, OneWayPortLeft_020::Create, OneWayPortLeft_020::Timer, OneWayPortLeft_020::Update, OneWayPortLeftGrav_039::simpleDraw},
		{OneWayPortRightGrav_040::name, OneWayPortRightGrav_040::Print, OneWayPortRightGrav_040::Initializer, OneWayPortRightGrav_040::Drawner, OneWayPortRight_021::Create, OneWayPortRight_021::Timer, OneWayPortRight_021::Update, OneWayPortRightGrav_040::simpleDraw},
		{OneWayPortUpGrav_041::name, OneWayPortUpGrav_041::Print, OneWayPortUpGrav_041::Initializer, OneWayPortUpGrav_041::Drawner, OneWayPortUp_022::Create, OneWayPortUp_022::Timer, OneWayPortUp_022::Update, OneWayPortUpGrav_041::simpleDraw},
		{Murphy::name, Murphy::Print, Murphy::Initializer, Murphy::Drawner, Murphy::Create, Murphy::Timer, Murphy::Update, Murphy::simpleDraw},
		{Murphy::name, Murphy::Print, Murphy::InitializerCrawlTail, Murphy::DrawnerCrawlTail, Murphy::CreateCrawlTail, Murphy::TimerCrawlTail, Murphy::UpdateCrawlTail, Murphy::simpleDrawCrawlTail},
	};


	void RunInitializer()
	{
		Object::Brick::Initialize();
		Editor::Object::Front::Initializer();
		Editor::Object::Back::Initializer();
		for (int i = 0; i < sizeof(objects) / sizeof(Container); i++)
			if (objects[i].initializerFnc)
				objects[i].initializerFnc();
	}
}



__int32 GetObjectNumber()
{
	return sizeof(Objects::objects) / sizeof(Objects::Container);
}



void DrawObject(int id, int x, int y, int w, int h)
{
	if (id < GetObjectNumber() && id >= 0)
	{
		Objects::objects[id].simpleDrawnerFnc(x, y, w, h);
	}
	else
	{
		static_cast<KIR5::SubBitmap &>(Res::bitmapBox).drawScaled(x, y, w, h);
	}
}

const char *GetObjectName(int id)
{
	if (id < GetObjectNumber() && id >= 0)
		return Objects::objects[id].name;
	return "";
}
void ObjectCreate(Object::Brick *object, Type::ID id, Type::Coord coord, Object::Brick::CREATER creater)
{
	object->__init__(id, coord);

	if (id >= 0 && id < sizeof(Objects::objects) / sizeof(Objects::Container))
	{
		object->isExists = true;
		object->name = Objects::objects[id].name;
		object->timerFnc = Objects::objects[id].timerFnc;
		object->updaterFnc = Objects::objects[id].updaterFnc;
		object->drawnerFnc = Objects::objects[id].drawnerFnc;
		object->printFnc = Objects::objects[id].printFnc;
		creater = Objects::objects[id].createrFnc;
	}

	if (creater)
	{
		Object::Brick::Stack stack(object);
		creater(&stack);
	}
	else
	{
		clog << "creater was nullptr at ID: " << id << KIR4::eol;
	}
}



namespace Editor
{
	void ObjectCreate(Object::Brick *object, Type::ID id, Type::Coord coord, Object::Brick::CREATER creater)
	{
		object->id = id;
		object->coord = coord;
		memset(object->specific, 0, sizeof(object->specific));

		object->isExists = true;
		if (id >= 0 && id < sizeof(Objects::objects) / sizeof(Objects::Container))
		{
			object->name = Objects::objects[id].name;
			object->drawnerFnc = Object::Back::Drawner;
		}
		else
		{
			object->name = "Unknown";
			object->drawnerFnc = nullptr;
		}
	}
}