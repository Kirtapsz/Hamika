#pragma once

#include "BaseFunctions.h"
#include "Tools.h"

/*
naming:

activateTimer
activateTime

disappearTimer
disappearTime

moveTimer
moveSpeed

rollTimer
rollSpeed

rotateSpeed
rotateTimer

animateTime
animateTimer

drawNumber

*/

namespace Object::Entity
{
	//Space 000
	namespace Space_000
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//Zonk 001
	namespace Zonk_001
	{
		extern const char *name;

		struct EntityData
		{
			FallAndRoll::EntityData fall_and_roll_;
			DrawNumber draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//BaseX 002
	namespace BaseX_002
	{
		extern const char *name;

		struct EntityData
		{
			float disappearTimer;
			DrawNumber draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareBlueLed 003
	namespace HardwareBlueLed_003
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareBrick1 004
	namespace HardwareBrick1_004
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareBlueResistance 005
	namespace HardwareBlueResistance_005
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareGreenLed 006
	namespace HardwareGreenLed_006
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareRedLed 007
	namespace HardwareRedLed_007
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareResource 008
	namespace HardwareResource_008
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareResistance 009
	namespace HardwareResistance_009
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareStripLine 010
	namespace HardwareStripLine_010
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareYellowResistance 011
	namespace HardwareYellowResistance_011
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareRedCapacitor 012
	namespace HardwareRedCapacitor_012
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//Bug 013
	namespace Bug_013
	{
		extern const char *name;

		struct EntityData
		{
			float electricDelayTimer;
			float electricTimer;
			float disappearTimer;
			DrawNumber draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//Infotron 014
	namespace Infotron_014
	{
		extern const char *name;

		struct EntityData
		{
			FallAndRoll::EntityData fall_and_roll_;
			float disappearTimer;
			DrawNumber draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//Exit 015
	namespace Exit_015
	{
		extern const char *name;

		struct EntityData
		{
			DrawNumber draw_number_;
			float animateTimer;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);

		void Open(Brick &_brick);
	}

	//CrossPort 016
	namespace CrossPort_016
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//DualPortHorizontal 017
	namespace DualPortHorizontal_017
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//DualPortVertical 018
	namespace DualPortVertical_018
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//OneWayPortDown 019
	namespace OneWayPortDown_019
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//OneWayPortLeft 020
	namespace OneWayPortLeft_020
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//OneWayPortRight 021
	namespace OneWayPortRight_021
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//OneWayPortUp 022
	namespace OneWayPortUp_022
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//RAMChipsMini 023
	namespace RAMChipsMini_023
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//RAMChipsLeft 024
	namespace RAMChipsLeft_024
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//RAMChipsRight 025
	namespace RAMChipsRight_025
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//Electrons 026
	namespace Electrons_026
	{
		extern const char *name;

		struct EntityData
		{
			Animator::EntityData animator_;
			MoveLeftWay::EntityData move_left_way_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//SnikSnakMove 027
	namespace SnikSnakMove_027
	{
		extern const char *name;

		struct EntityData
		{
			MoveLeftWay::EntityData move_left_way_;
			DrawNumber draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//Terminal 028
	namespace Terminal_028
	{
		extern const char *name;

		struct EntityData
		{
			Animator::EntityData animator_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);

		void Pushed(Brick &_brick);
	}

	//Utility1 029
	namespace Utility1_029
	{
		extern const char *name;

		struct EntityData
		{
			FallAndRoll::EntityData fall_and_roll_;
			bool active;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//Utility2 030
	namespace Utility2_030
	{
		extern const char *name;

		struct EntityData
		{
			float activateTimer;
			float disappearTimer;
			DrawNumber draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);

		void Activate(Brick &_brick);
	}

	//Utility3 031
	namespace Utility3_031
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//ExplosionEffect 032
	namespace ExplosionEffect_032
	{
		extern const char *name;

		struct EntityData
		{
			float explosionTimer;
			DrawNumber draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//Explosion 033
	namespace Explosion_033
	{
		extern const char *name;

		struct EntityData
		{
			float explosionTimer;
			float blastingTimer;
			DrawNumber draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);

		void ReCreate(OBJECT_CREATER_PARAM);
	}

	////ExplosionExpand 034
	//namespace ExplosionExpand_034
	//{
	//	extern const char *name;

	//	void Initializer(OBJECT_INITIALIZER_PARAM);
	//	void Create(OBJECT_CREATER_PARAM);
	//	OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
	//	void Timer(OBJECT_TIMER_PARAM);
	//	void Update(OBJECT_UPDATE_PARAM);
	//	void Drawner(OBJECT_DRAWNER_PARAM);
	//	void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
	void Finalize(OBJECT_FINALIZE_PARAM);
	//}

	//RAMChipsTop 035
	namespace RAMChipsTop_035
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//RAMChipsBottom 036
	namespace RAMChipsBottom_036
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//HardwareSwitch 037
	namespace HardwareSwitch_037
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//OneWayPortDownGrav 038
	namespace OneWayPortDownGrav_038
	{
		extern const char *name;
		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//OneWayPortLeftGrav 039
	namespace OneWayPortLeftGrav_039
	{
		extern const char *name;
		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//OneWayPortRightGrav 040
	namespace OneWayPortRightGrav_040
	{
		extern const char *name;
		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}

	//OneWayPortUpGrav 041
	namespace OneWayPortUpGrav_041
	{
		extern const char *name;
		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
		void Finalize(OBJECT_FINALIZE_PARAM);
	}
}