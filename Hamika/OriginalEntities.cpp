#include "OriginalEntities.h"
#include "Space.h"
#include "Bedrock.h"

#include <KIR/KIR4_console.h>

namespace Object::Entity
{
	//Space 000
	namespace Space_000
	{
		const char *name = "000 - Space";

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Space::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Space"] = Space::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Space::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Space::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Space::Drawner(OBJECT_DRAWNER_CALL);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{

		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}


	//Zonk 001
	namespace Zonk_001
	{
		const char *name = "001 - Zonk";

		Res::Slides ZonkMoveHorizontal;

		constexpr float moveSpeed = CPS / 13.99f;
		constexpr float rollSpeed = moveSpeed;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			ZonkMoveHorizontal.initialize(Res::tiles[Res::Tiles::_001_ZonkMoveHorizontal], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			entity_data->draw_number_ = 0;

			_brick.SetMoveSpeed({rollSpeed,moveSpeed});
			_brick.SetFlags(Brick::CanBeExploded | Brick::RollOff | Brick::CanPushLeft | Brick::CanPushRight);
			_brick.enablePhysics();

			FallAndRoll::Create(_brick, entity_data->fall_and_roll_);
			entity_data->fall_and_roll_.setHeavy(true);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			Json json;

			json["draw_number"] = entity_data->draw_number_;

			json["\\FallAndRoll"] = FallAndRoll::Print(_brick, entity_data->fall_and_roll_);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			FallAndRoll::Timer(_brick, entity_data->fall_and_roll_);
			if (_brick.IsMoveLeft())
			{
				DRAW_NUMBER_DESC(_brick.move.x(), 1.f, entity_data->draw_number_, &_brick, ZonkMoveHorizontal);
			}
			else if (_brick.IsMoveRight())
			{
				DRAW_NUMBER_ASC(_brick.move.x(), -1.f, entity_data->draw_number_, &_brick, ZonkMoveHorizontal);
			}
			else
			{
				if (entity_data->draw_number_ != 0)
				{
					entity_data->draw_number_ = 0;
					_brick.requests.draw = true;
				}
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			FallAndRoll::Update(_brick, entity_data->fall_and_roll_, updateType);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			ZonkMoveHorizontal[entity_data->draw_number_].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			ZonkMoveHorizontal[0].drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}


	//BaseX 002
	namespace BaseX_002
	{
		const char *name = "002 - BaseX";

		KIR5::SubBitmap BaseX;
		Res::Slides BaseLineDisappear;

		constexpr float disappearTime = 0.23f;

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			BaseX = Res::tiles[Res::Tiles::_002_BaseX];
			BaseLineDisappear.initialize(Res::tiles[Res::Tiles::_002_BaseX_square_disappear], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			entity_data->disappearTimer = 0;
			entity_data->draw_number_ = 0;

			_brick.SetFlags(Brick::CanBeExploded | Brick::MurphyStepOn | Brick::MurphyCanSuck | Brick::GiveGravityDelay);

			_brick.events.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			Json json;

			json["disappearTimer"] = entity_data->disappearTimer;
			json["draw_number"] = entity_data->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (ACTION_TIMER(entity_data->disappearTimer,
							 disappearTime,
							 &_brick,
							 [&entity_data, &_brick]()->bool
			{
				return entity_data->disappearTimer == ACTION_TIMER_START;
			},
							 [&entity_data, &_brick]()->bool
			{
				entity_data->draw_number_ = 0;
				_brick.events.timer = true;
				_brick.events.update = false;
				_brick.requests.remove = false;
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				DRAW_NUMBER_ASC(entity_data->disappearTimer,
				disappearTime,
				entity_data->draw_number_,
				&_brick, BaseLineDisappear);
			return true;
			},
				[&entity_data, &_brick]()->bool
			{
				_brick.events.clear();
				_brick.requests.remove = true;
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
  			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (_brick.requests.remove)
			{
				entity_data->disappearTimer = ACTION_TIMER_START;
				Timer(_brick);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (entity_data->disappearTimer > 0)
			{
				BaseLineDisappear[entity_data->draw_number_].drawScaled(x, y, w, h);
			}
			else
			{
				BaseX.drawScaled(x, y, w, h);
			}
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			BaseX.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareBlueLed 003
	namespace HardwareBlueLed_003
	{
		const char *name = "003 - HardwareBlueLed";

		KIR5::SubBitmap HardwareBlueLed;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareBlueLed = Res::tiles[Res::Tiles::_003_HardwareBlueLed];
		}

		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareBlueLed.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareBlueLed.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareBrick1 004
	namespace HardwareBrick1_004
	{
		const char *name = "004 - HardwareBrick1";

		KIR5::SubBitmap HardwareBrick1;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareBrick1 = Res::tiles[Res::Tiles::_004_HardwareBrick1];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareBrick1.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareBrick1.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareBlueResistance 005
	namespace HardwareBlueResistance_005
	{
		const char *name = "005 - HardwareBlueResistance";

		KIR5::SubBitmap HardwareBlueResistance;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareBlueResistance = Res::tiles[Res::Tiles::_005_HardwareBlueResistance];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareBlueResistance.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareBlueResistance.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareGreenLed 006
	namespace HardwareGreenLed_006
	{
		const char *name = "006 - HardwareGreenLed";

		KIR5::SubBitmap HardwareGreenLed;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareGreenLed = Res::tiles[Res::Tiles::_006_HardwareGreenLed];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareGreenLed.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareGreenLed.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareRedLed 007
	namespace HardwareRedLed_007
	{
		const char *name = "007 - HardwareRedLed";

		KIR5::SubBitmap HardwareRedLed;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareRedLed = Res::tiles[Res::Tiles::_007_HardwareRedLed];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareRedLed.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareRedLed.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareResource 008
	namespace HardwareResource_008
	{
		const char *name = "008 - HardwareResource";

		KIR5::SubBitmap HardwareResource;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareResource = Res::tiles[Res::Tiles::_008_HardwareResource];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareResource.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareResource.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareResistance 009
	namespace HardwareResistance_009
	{
		const char *name = "009 - HardwareResistance";

		KIR5::SubBitmap HardwareResistance;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareResistance = Res::tiles[Res::Tiles::_009_HardwareResistance];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareResistance.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareResistance.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareStripLine 010
	namespace HardwareStripLine_010
	{
		const char *name = "010 - HardwareStripLine";

		KIR5::SubBitmap HardwareStripLine;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareStripLine = Res::tiles[Res::Tiles::_010_HardwareStripLine];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareStripLine.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareStripLine.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareYellowResistance 011
	namespace HardwareYellowResistance_011
	{
		const char *name = "011 - HardwareYellowResistance";

		KIR5::SubBitmap HardwareYellowResistance;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareYellowResistance = Res::tiles[Res::Tiles::_011_HardwareYellowResistance];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareYellowResistance.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareYellowResistance.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareRedCapacitor 012
	namespace HardwareRedCapacitor_012
	{
		const char *name = "012 - HardwareRedCapacitor";

		KIR5::SubBitmap HardwareRedCapacitor;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareRedCapacitor = Res::tiles[Res::Tiles::_012_HardwareRedCapacitor];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareRedCapacitor.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareRedCapacitor.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//Bug 013
	namespace Bug_013
	{
		const char *name = "013 - Bug";

		Res::Slides Bug;

		constexpr float electricTime = 0.7f;      // duration in sec
		constexpr float electricDelayTime = 3.0f; // duration in sec
		constexpr float disappearTime = 0.23f;    // duration in sec
		constexpr float triggerChance = 1 / 6.f;  // chance / sec

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Bug.initialize(Res::tiles[Res::Tiles::_013_Bug], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			entity_data->electricDelayTimer = 0;
			entity_data->electricTimer = ACTION_TIMER_START;
			entity_data->disappearTimer = 0;
			entity_data->draw_number_ = 0;
			_brick.SetFlags(Brick::CanBeExploded | Brick::MurphyStepOn | Brick::MurphyCanSuck | Brick::GiveGravityDelay | Brick::MurphyDies);

			_brick.events.timer = true;
			_brick.events.update = true;

			_brick.requests.timer = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Json json;

			json["electricDelayTimer"] = entity_data->electricDelayTimer;
			json["electricTimer"] = entity_data->electricTimer;
			json["disappearTimer"] = entity_data->disappearTimer;
			json["draw_number"] = entity_data->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			_brick.requests.timer = true;

			if (ACTION_TIMER(entity_data->disappearTimer,
							 disappearTime,
							 &_brick,
							 [&entity_data, &_brick]()->bool
			{
				return entity_data->disappearTimer == ACTION_TIMER_START;
			},
							 [&entity_data, &_brick]()->bool
			{
				entity_data->draw_number_ = 0;
				_brick.events.timer = true;
				_brick.events.update = false;
				_brick.requests.remove = false;
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				DRAW_NUMBER_ASC(entity_data->disappearTimer,
				disappearTime,
				entity_data->draw_number_,
				&_brick, BaseX_002::BaseLineDisappear);
			return true;
			},
				[&entity_data, &_brick]()->bool
			{
				_brick.events.clear();
				_brick.requests.remove = true;
				return true;
			}))
			{
				return;
			}


			if (ACTION_TIMER(entity_data->electricDelayTimer,
							 electricDelayTime,
							 &_brick,
							 [&entity_data, &_brick]()->bool
			{
				return entity_data->electricDelayTimer == ACTION_TIMER_START;
			},
							 [&entity_data, &_brick]()->bool
			{
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				return true;
			}))
			{
				return;
			}

			if (ACTION_TIMER(entity_data->electricTimer,
							 electricTime,
							 &_brick,
							 [&entity_data, &_brick]()->bool
			{
				return _brick.scene->rollTrigger(&_brick, 1300, triggerChance) || entity_data->electricTimer == ACTION_TIMER_START;
			},
							 [&entity_data, &_brick]()->bool
			{
				entity_data->draw_number_ = 0;

				_brick.AddFlags(Brick::Flags::MurphyDies);
				_brick.requests.draw = true;
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				DRAW_NUMBER_ASC(entity_data->electricTimer,
				electricTime,
				entity_data->draw_number_,
				&_brick, Bug);
			return true;
			},
				[&entity_data, &_brick]()->bool
			{
				entity_data->electricDelayTimer = ACTION_TIMER_START;
				_brick.RemoveFlags(Brick::Flags::MurphyDies);
				_brick.requests.draw = true;
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (_brick.requests.remove)
			{
				entity_data->disappearTimer = ACTION_TIMER_START;
				Timer(_brick);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (entity_data->disappearTimer > 0)
			{
				BaseX_002::BaseLineDisappear[entity_data->draw_number_].drawScaled(x, y, w, h);
			}
			else if (entity_data->electricTimer > 0)
			{
				Bug[entity_data->draw_number_].drawScaled(x, y, w, h);
			}
			else
			{
				BaseX_002::BaseX.drawScaled(x, y, w, h);
			}
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Bug[Bug.getDrawNumber(0.5f)].drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//Infotron 014
	namespace Infotron_014
	{
		const char *name = "014 - Infotron";

		Res::Slides InfotronDisappear;
		Res::Slides InfotronMoveHorizontal;

		constexpr float moveSpeed = CPS / 13.99f;
		constexpr float rollSpeed = moveSpeed;
		constexpr float disappearTime = 0.23f;

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			InfotronDisappear.initialize(Res::tiles[Res::Tiles::_014_InfotronDisappear], Res::tiles[Res::BitmapPool::Fallback]);
			InfotronMoveHorizontal.initialize(Res::tiles[Res::Tiles::_014_InfotronMoveHorizontal], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			entity_data->disappearTimer = 0;
			entity_data->draw_number_ = 0;
			_brick.SetMoveSpeed({rollSpeed,moveSpeed});
			_brick.SetFlags(Brick::CanBeExploded | Brick::RollOff | Brick::MurphyStepOn | Brick::MurphyCanSuck | Brick::Give1Score);

			FallAndRoll::Create(_brick, entity_data->fall_and_roll_);
			entity_data->fall_and_roll_.setHeavy(true);

			_brick.events.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Json json;

			json["disappearTimer"] = entity_data->disappearTimer;
			json["draw_number"] = entity_data->draw_number_;
			json["\\FallAndRoll"] = FallAndRoll::Print(_brick, entity_data->fall_and_roll_);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (ACTION_TIMER(entity_data->disappearTimer,
							 disappearTime,
							 &_brick,
							 [&entity_data, &_brick]()->bool
			{
				return entity_data->disappearTimer == ACTION_TIMER_START;
			},
							 [&entity_data, &_brick]()->bool
			{
				entity_data->draw_number_ = 0;
				_brick.events.timer = true;
				_brick.events.update = false;
				_brick.requests.remove = false;
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				DRAW_NUMBER_ASC(entity_data->disappearTimer,
				disappearTime,
				entity_data->draw_number_,
				&_brick, InfotronDisappear);
			return true;
			},
				[&entity_data, &_brick]()->bool
			{
				_brick.events.clear();
				_brick.requests.remove = true;
				return true;
			}))
			{
				return;
			}

			FallAndRoll::Timer(_brick, entity_data->fall_and_roll_);
			if (_brick.IsMoveLeft())
			{
				DRAW_NUMBER_DESC(_brick.move.x(), 1.f, entity_data->draw_number_, &_brick, InfotronMoveHorizontal);
			}
			else if (_brick.IsMoveRight())
			{
				DRAW_NUMBER_ASC(_brick.move.x(), -1.f, entity_data->draw_number_, &_brick, InfotronMoveHorizontal);
			}
			else
			{
				if (entity_data->draw_number_ != 0)
				{
					entity_data->draw_number_ = 0;
					_brick.requests.draw = true;
				}
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (_brick.requests.remove)
			{
				entity_data->disappearTimer = ACTION_TIMER_START;
				Timer(_brick);
			}

			if (!_brick.scene->IamRemain(_brick))
			{
				FallAndRoll::Update(_brick, entity_data->fall_and_roll_, updateType);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (entity_data->disappearTimer > 0)
			{
				InfotronDisappear[entity_data->draw_number_].drawScaled(x, y, w, h);
			}
			else
			{
				InfotronMoveHorizontal[entity_data->draw_number_].drawScaled(x, y, w, h);
			}
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			InfotronMoveHorizontal[0].drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//Exit 015
	namespace Exit_015
	{
		const char *name = "015 - Exit";

		Res::Slides Exit;

		constexpr float animateTime = 0.5f;

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Exit.initialize(Res::tiles[Res::Tiles::_015_Exit], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			DRAW_NUMBER_ASC_INIT(entity_data->draw_number_, &_brick, Exit);

			entity_data->draw_number_ = 0;
			entity_data->animateTimer = 0.f;

			_brick.events.update = true;
			_brick.requests.update = true;

			_brick.SetFlags(Brick::CanBeExploded);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Json json;

			json["draw_number"] = entity_data->draw_number_;
			json["animateTimer"] = entity_data->animateTimer;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (ACTION_TIMER(entity_data->animateTimer,
							 animateTime,
							 &_brick,
							 [&entity_data, &_brick]()->bool
			{
				return entity_data->animateTimer == ACTION_TIMER_START;
			},
							 [&entity_data, &_brick]()->bool
			{
				entity_data->draw_number_ = 0;
				_brick.events.timer = true;
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				DRAW_NUMBER_ASC(entity_data->animateTimer,
				animateTime,
				entity_data->draw_number_,
				&_brick, Exit);
			return true;
			},
				[&entity_data, &_brick]()->bool
			{
				_brick.events.clear();
				_brick.requests.clear();
				_brick.requests.draw = true;
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			entity_data->animateTimer = _brick.scene->getScoreToCollect() > 0 ? 0.f : ACTION_TIMER_START;
			_brick.events.update = false;

			Timer(_brick);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Exit[entity_data->draw_number_].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Exit[Exit.getCount() - 1].drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}

		void Open(Brick &_brick)
		{
			Type::Coord coord{};
			for (coord.x() = 0; coord.x() < _brick.scene->MapSize().width(); coord.x()++)
			{
				for (coord.y() = 0; coord.y() < _brick.scene->MapSize().height(); coord.y()++)
				{
					Brick *brick = _brick.scene->GetObjectU(coord);
					if (brick->id == ObjectID::Exit)
					{
						EntityData *entity_data = (EntityData *)(brick->specific);
						entity_data->animateTimer = ACTION_TIMER_START;
						Timer(*brick);
					}
				}
			}
		}
	}

	//CrossPort 016
	namespace CrossPort_016
	{
		const char *name = "016 - CrossPort";

		KIR5::SubBitmap CrossPort;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			CrossPort = Res::tiles[Res::Tiles::_016_CrossPort];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::Passage);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			CrossPort.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			CrossPort.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//DualPortHorizontal 017
	namespace DualPortHorizontal_017
	{
		const char *name = "017 - DualPortHorizontal";

		KIR5::SubBitmap DualPortHorizontal;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			DualPortHorizontal = Res::tiles[Res::Tiles::_017_DualPortHorizontal];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::PassageHorizontal);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			DualPortHorizontal.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			DualPortHorizontal.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//DualPortVertical 018
	namespace DualPortVertical_018
	{
		const char *name = "018 - DualPortVertical";

		KIR5::SubBitmap DualPortVertical;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			DualPortVertical = Res::tiles[Res::Tiles::_018_DualPortVertical];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::PassageVertical);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			DualPortVertical.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			DualPortVertical.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//OneWayPortDown 019
	namespace OneWayPortDown_019
	{
		const char *name = "019 - OneWayPortDown";

		KIR5::SubBitmap OneWayPortDown;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			OneWayPortDown = Res::tiles[Res::Tiles::_019_OneWayPortDown];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::PassageFromTop);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			OneWayPortDown.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			OneWayPortDown.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//OneWayPortLeft 020
	namespace OneWayPortLeft_020
	{
		const char *name = "020 - OneWayPortLeft";

		KIR5::SubBitmap OneWayPortLeft;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			OneWayPortLeft = Res::tiles[Res::Tiles::_020_OneWayPortLeft];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::PassageFromRight);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			OneWayPortLeft.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			OneWayPortLeft.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//OneWayPortRight 021
	namespace OneWayPortRight_021
	{
		const char *name = "021 - OneWayPortRight";

		KIR5::SubBitmap OneWayPortRight;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			OneWayPortRight = Res::tiles[Res::Tiles::_021_OneWayPortRight];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::PassageFromLeft);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			OneWayPortRight.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			OneWayPortRight.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//OneWayPortUp 022
	namespace OneWayPortUp_022
	{
		const char *name = "022 - OneWayPortUp";

		KIR5::SubBitmap OneWayPortUp;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			OneWayPortUp = Res::tiles[Res::Tiles::_022_OneWayPortUp];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::PassageFromBottom);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			OneWayPortUp.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			OneWayPortUp.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//RAMChipsMini 023
	namespace RAMChipsMini_023
	{
		const char *name = "023 - RAMChipsMini";

		KIR5::SubBitmap RAMChipsMini;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			RAMChipsMini = Res::tiles[Res::Tiles::_023_RAMChipsMini];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::RollOff);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			RAMChipsMini.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			RAMChipsMini.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//RAMChipsLeft 024
	namespace RAMChipsLeft_024
	{
		const char *name = "024 - RAMChipsLeft";

		KIR5::SubBitmap RAMChipsLeft;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			RAMChipsLeft = Res::tiles[Res::Tiles::_024_RAMChipsLeft];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::RollOff);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			RAMChipsLeft.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			RAMChipsLeft.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//RAMChipsRight 025
	namespace RAMChipsRight_025
	{
		const char *name = "025 - RAMChipsRight";

		KIR5::SubBitmap RAMChipsRight;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			RAMChipsRight = Res::tiles[Res::Tiles::_025_RAMChipsRight];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::RollOff);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			RAMChipsRight.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			RAMChipsRight.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//Electrons 026
	namespace Electrons_026
	{
		const char *name = "026 - Electrons";

		constexpr float moveSpeed = CPS / 13.99f;
		constexpr float rotateSpeed = Type::Rotations::_180 / (1 / moveSpeed);

		Res::Slides Electrons;

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Electrons.initialize(Res::tiles[Res::Tiles::_026_Electrons], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			_brick.SetRotationSpeed(rotateSpeed);
			_brick.SetMoveSpeed({moveSpeed,moveSpeed});
			_brick.SetTranslationID(ObjectID::Infotron);
			_brick.SetFlags(Brick::CanBeExploded | Brick::ExplosionType3);

			Animator::Create(_brick, entity_data->animator_);
			entity_data->animator_.SetNumberOfFrames(Electrons.getCount());
			entity_data->animator_.SetAnimationTime(0.8f);

			MoveLeftWay::Create(_brick, entity_data->move_left_way_);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Json json;

			json["\\Animator"] = Animator::Print(_brick, entity_data->animator_);
			json["\\MoveLeftWay"] = MoveLeftWay::Print(_brick, entity_data->move_left_way_);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Animator::Timer(_brick, entity_data->animator_);
			MoveLeftWay::Timer(_brick, entity_data->move_left_way_);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Animator::Update(_brick, entity_data->animator_, updateType);
			MoveLeftWay::Update(_brick, entity_data->move_left_way_, updateType);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Electrons[entity_data->animator_.GetDrawNumber()].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Electrons[0].drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//SnikSnakMove 027
	namespace SnikSnakMove_027
	{
		const char *name = "027 - SnikSnakMove";

		std::array<Res::Slides, 4>SnikSnakMove;
		Res::Slides SnikSnakRotate;

		constexpr float moveSpeed = CPS / 13.99f;
		constexpr float rotateSpeed = Type::Rotations::_180 / (1 / moveSpeed);

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			SnikSnakMove[Type::Directions::up].initialize(Res::tiles[Res::Tiles::_027_SnikSnakMoveUp], Res::tiles[Res::BitmapPool::Fallback]);
			SnikSnakMove[Type::Directions::right].initialize(Res::tiles[Res::Tiles::_027_SnikSnakMoveRight], Res::tiles[Res::BitmapPool::Fallback]);
			SnikSnakMove[Type::Directions::down].initialize(Res::tiles[Res::Tiles::_027_SnikSnakMoveDown], Res::tiles[Res::BitmapPool::Fallback]);
			SnikSnakMove[Type::Directions::left].initialize(Res::tiles[Res::Tiles::_027_SnikSnakMoveLeft], Res::tiles[Res::BitmapPool::Fallback]);
			SnikSnakRotate.initialize(Res::tiles[Res::Tiles::_027_SnikSnakRotate], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			entity_data->draw_number_ = 0;

			_brick.SetRotationSpeed(rotateSpeed);
			_brick.SetMoveSpeed({moveSpeed,moveSpeed});
			_brick.SetFlags(Brick::CanBeExploded | Brick::ExplosionType3);
			_brick.SetTranslationID(ObjectID::Space);

			MoveLeftWay::Create(_brick, entity_data->move_left_way_);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Json json;

			json["draw_number"] = entity_data->draw_number_;
			json["\\MoveLeftWay"] = MoveLeftWay::Print(_brick, entity_data->move_left_way_);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			MoveLeftWay::Timer(_brick, entity_data->move_left_way_);

			if (_brick.isActionMove())
			{
				DRAW_NUMBER_ASC(_brick.GetAbsMove(), 1.f, entity_data->draw_number_, &_brick, SnikSnakMove[_brick.getMoveDirection()]);
			}
			else
			{
				DRAW_NUMBER_DESC(Brick::GetRealRotation(
					_brick.rotation + (Type::Rotations::_360 / (float)SnikSnakRotate.getCount()) / 2.f),
					Type::Rotations::_360, entity_data->draw_number_, &_brick, SnikSnakRotate);
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			MoveLeftWay::Update(_brick, entity_data->move_left_way_, updateType);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (_brick.isActionMove())
			{
				SnikSnakMove[_brick.getMoveDirection()][entity_data->draw_number_].drawScaled(x, y, w, h);
			}
			else
			{
				SnikSnakRotate[entity_data->draw_number_].drawScaled(x, y, w, h);
			}
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			SnikSnakRotate[0].drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//Terminal 028
	namespace Terminal_028
	{
		const char *name = "028 - Terminal";

		Res::Slides Terminal;

		constexpr float activatedAnimateTime = 0.7f;
		constexpr float animateTime = 3.f;

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Terminal.initialize(Res::tiles[Res::Tiles::_028_Terminal], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Animator::Create(_brick, entity_data->animator_);
			entity_data->animator_.SetNumberOfFrames(Terminal.getCount());
			entity_data->animator_.SetAnimationTime(animateTime);

			_brick.SetFlags(Brick::ButtonPush | Brick::CanBeExploded);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Json json;

			json["\\Animator"] = Animator::Print(_brick, entity_data->animator_);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Animator::Timer(_brick, entity_data->animator_);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Animator::Update(_brick, entity_data->animator_, updateType);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Terminal[entity_data->animator_.GetDrawNumber()].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Terminal[0].drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}

		void speedUpTerminalSpeed(Brick &_brick)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			entity_data->animator_.SetAnimationTime(activatedAnimateTime);
		}

		void Pushed(Brick &_brick)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (entity_data->animator_.time != activatedAnimateTime)
			{
				Type::Coord coord;
				for (coord.x() = 0; coord.x() < _brick.scene->MapSize().width(); coord.x()++)
				{
					for (coord.y() = 0; coord.y() < _brick.scene->MapSize().height(); coord.y()++)
					{
						if (_brick.scene->GetObjectU(coord))
						{
							if (_brick.scene->GetObjectU(coord)->id == ObjectID::Utility3)
							{
								_brick.scene->blowup(*_brick.scene->GetObjectU(coord));
							}
							else if (_brick.scene->GetObjectU(coord)->id == _brick.id)
							{
								speedUpTerminalSpeed(*_brick.scene->GetObjectU(coord));
							}
						}
					}
				}
			}
		}
	}

	//Utility1 029
	namespace Utility1_029
	{
		const char *name = "029 - Utility1";

		KIR5::SubBitmap Utility1;

		constexpr float moveSpeed = CPS / 13.99f;

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Utility1 = Res::tiles[Res::Tiles::_029_Utility1];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			entity_data->active = false;
			_brick.SetFlags(Brick::CanBeExploded | Brick::ExplosionType3 | Brick::CanPushLeft | Brick::CanPushRight);
			_brick.SetMoveSpeed({moveSpeed,moveSpeed});
			_brick.SetTranslationID(ObjectID::Space);

			Fall::Create(_brick, entity_data->fall_and_roll_);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			Json json;

			json["active"] = entity_data->active;
			json["\\Fall"] = Fall::Print(_brick, entity_data->fall_and_roll_);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			Fall::Timer(_brick, entity_data->fall_and_roll_);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			Fall::Update(_brick, entity_data->fall_and_roll_, updateType);

			if (_brick.isActionMove())
			{
				entity_data->active = true;
			}
			else if (entity_data->active)
			{
				_brick.scene->ObjectArrived(_brick.GetCoord());
				_brick.scene->blowup(_brick);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			Utility1.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Utility1.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//Utility2 030
	namespace Utility2_030
	{
		const char *name = "030 - Utility2";

		Res::Slides Utility2;
		KIR5::SubBitmap Utility2Activated;

		constexpr float activateTime = 1.8f;
		constexpr float disappearTime = 0.23f;

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Utility2.initialize(Res::tiles[Res::Tiles::_030_Utility2], Res::tiles[Res::BitmapPool::Fallback]);
			Utility2Activated = Res::tiles[Res::Tiles::_030_Utility2Activated];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			entity_data->activateTimer = 0.f;
			entity_data->disappearTimer = 0.f;
			entity_data->draw_number_ = 0;
			_brick.SetFlags(Brick::CanBeExploded | Brick::MurphyCanSuck | Brick::Give1Unity | Brick::MurphyStepOn);

			_brick.events.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);
			Json json;

			json["activateTimer"] = entity_data->activateTimer;
			json["disappearTimer"] = entity_data->disappearTimer;
			json["draw_number"] = entity_data->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (ACTION_TIMER(entity_data->disappearTimer,
							 disappearTime,
							 &_brick,
							 [&entity_data, &_brick]()->bool
			{
				return entity_data->disappearTimer == ACTION_TIMER_START;
			},
							 [&entity_data, &_brick]()->bool
			{
				entity_data->draw_number_ = 0;
				_brick.events.timer = true;
				_brick.events.update = false;
				_brick.requests.remove = false;
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				DRAW_NUMBER_ASC(entity_data->disappearTimer,
				disappearTime,
				entity_data->draw_number_,
				&_brick, Utility2);
			return true;
			},
				[&entity_data, &_brick]()->bool
			{
				_brick.events.clear();
				_brick.requests.remove = true;
				return true;
			}))
			{
				return;
			}

			if (ACTION_TIMER(entity_data->activateTimer,
							 activateTime,
							 &_brick,
							 [&entity_data, &_brick]()->bool
			{
				return entity_data->activateTimer == ACTION_TIMER_START;
			},
							 [&entity_data, &_brick]()->bool
			{
				entity_data->draw_number_ = 0;
				_brick.SetFlags(Brick::CanBeExploded | Brick::ExplosionType3);
				_brick.events.timer = true;
				_brick.events.update = false;
				_brick.requests.remove = false;
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				DRAW_NUMBER_T draw_number = (DRAW_NUMBER_T(entity_data->activateTimer * 10)) % 2;
				if (entity_data->draw_number_ != draw_number)
				{
					entity_data->draw_number_ = draw_number;
					_brick.requests.draw = true;
				}
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				_brick.events.clear();
				_brick.scene->blowup(_brick);
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (_brick.requests.remove)
			{
				entity_data->disappearTimer = ACTION_TIMER_START;
				Timer(_brick);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (entity_data->activateTimer > 0 && entity_data->draw_number_ == 1)
			{
				Utility2Activated.drawScaled(x, y, w, h);
			}
			else
			{
				Utility2[entity_data->draw_number_].drawScaled(x, y, w, h);
			}
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Utility2[0].drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}

		void Activate(Brick &_brick)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			entity_data->activateTimer = ACTION_TIMER_START;
			Timer(_brick);
		}
	}

	//Utility3 031
	namespace Utility3_031
	{
		const char *name = "031 - Utility3";

		KIR5::SubBitmap Utility3;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Utility3 = Res::tiles[Res::Tiles::_031_Utility3];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::ExplosionType3 | Brick::CanPush);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Utility3.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Utility3.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//ExplosionEffect 032
	namespace ExplosionEffect_032
	{
		const char *name = "032 - ExplosionEffect";

		Res::Slides Blasting;

		constexpr float explosionTime = 0.745f;

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Blasting.initialize(Res::tiles[Res::Tiles::_032_Explosion], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			entity_data->explosionTimer = explosionTime;
			entity_data->draw_number_ = 0;

			_brick.events.timer = false;
			_brick.events.update = true;
			_brick.events.topDraw = true;

			_brick.requests.finalize = true;

			_brick.SetFlags(Brick::StepOn | Brick::MurphyStepOn | Brick::CanBeExploded | Brick::RollOff | Brick::MurphyDies | Brick::MurphyStepOn);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Json json;

			json["explosionTimer"] = entity_data->explosionTimer;
			json["draw_number"] = entity_data->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			if (ACTION_TIMER(entity_data->explosionTimer,
							 explosionTime,
							 &_brick,
							 [&entity_data, &_brick]()->bool
			{
				return false;
			},
							 [&entity_data, &_brick]()->bool
			{
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				DRAW_NUMBER_ASC(entity_data->explosionTimer,
				explosionTime,
				entity_data->draw_number_,
				&_brick, ExplosionEffect_032::Blasting);
			return true;
			},
				[&entity_data, &_brick]()->bool
			{
				_brick.requests.remove = true;
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Blasting[entity_data->draw_number_].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Blasting[0].drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			_brick.events.timer = true;
			_brick.requests.timer = true;
			_brick.events.update = false;
		}
	}

	//Explosion 033
	namespace Explosion_033
	{
		const char *name = "033 - Explosion";

		constexpr float blastingTime = ExplosionEffect_032::explosionTime / 2.f;

		typedef EntityData Specific;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			entity_data->blastingTimer = blastingTime;
			entity_data->draw_number_ = 0;

			_brick.scene->GetObject(_brick.GetCoord()).events.clear();
			_brick.scene->GetObject(_brick.GetCoord()).requests.clear();
			_brick.scene->GetObject(_brick.GetCoord()).RemoveFlags(Brick::Flags::CanBeKilled);

			_brick.events.timer = false;
			_brick.events.update = true;
			_brick.events.topDraw = true;

			_brick.requests.finalize = true;

			_brick.SetFlags(Brick::CanBeExploded | Brick::RollOff | Brick::MurphyDies | Brick::MurphyStepOn);
		}
		void ReCreate(OBJECT_CREATER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			entity_data->draw_number_ = 0;

			if (entity_data->blastingTimer == 0)
			{
				entity_data->blastingTimer = blastingTime;

				_brick.scene->GetObject(_brick.GetCoord()).events.clear();
				_brick.scene->GetObject(_brick.GetCoord()).requests.clear();
				_brick.scene->GetObject(_brick.GetCoord()).RemoveFlags(Brick::Flags::CanBeKilled);
			}

			_brick.events.update = true;
			_brick.requests.finalize = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			Json json;

			json["explosionTimer"] = entity_data->explosionTimer;
			json["blastingTimer"] = entity_data->blastingTimer;
			json["draw_number"] = entity_data->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			ACTION_TIMER(entity_data->blastingTimer,
						 blastingTime,
						 &_brick,
						 [&entity_data, &_brick]()->bool
			{
				return false;
			},
						 [&entity_data, &_brick]()->bool
			{
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				Brick &brick = _brick.scene->GetObject(_brick.GetCoord());
				if (brick.GetFlags() & Brick::ExplosionType)
				{
					_brick.scene->blowup(brick);
				}
				else
				{
					_brick.scene->ObjectVirtualArrived(_brick.GetCoord());
					_brick.scene->ObjectPut(_brick.GetCoord(), _brick.GetObjectIDremain());
				}
				return true;
			});

			ACTION_TIMER(entity_data->explosionTimer,
						 ExplosionEffect_032::explosionTime,
						 &_brick,
						 [&entity_data, &_brick]()->bool
			{
				return false;
			},
						 [&entity_data, &_brick]()->bool
			{
				return true;
			},
				[&entity_data, &_brick]()->bool
			{
				DRAW_NUMBER_ASC(entity_data->explosionTimer,
				ExplosionEffect_032::explosionTime,
				entity_data->draw_number_,
				&_brick, ExplosionEffect_032::Blasting);
			return true;
			},
				[&entity_data, &_brick]()->bool
			{
				_brick.requests.remove = true;
				return true;
			});
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			ExplosionEffect_032::Blasting[entity_data->draw_number_].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			ExplosionEffect_032::Blasting[0].drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{
			EntityData *entity_data = (EntityData *)(_brick.specific);

			_brick.events.timer = true;
			_brick.requests.timer = true;
			_brick.events.update = false;

			entity_data->explosionTimer = ExplosionEffect_032::explosionTime;
		}
	}

	//RAMChipsTop 035
	namespace RAMChipsTop_035
	{
		const char *name = "035 - RAMChipsTop";

		KIR5::SubBitmap RAMChipsTop;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			RAMChipsTop = Res::tiles[Res::Tiles::_035_RAMChipsTop];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::RollOff);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			RAMChipsTop.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			RAMChipsTop.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//RAMChipsBottom 036
	namespace RAMChipsBottom_036
	{
		const char *name = "036 - RAMChipsBottom";

		KIR5::SubBitmap RAMChipsBottom;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			RAMChipsBottom = Res::tiles[Res::Tiles::_036_RAMChipsBottom];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::CanBeExploded | Brick::RollOff);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			RAMChipsBottom.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			RAMChipsBottom.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//HardwareSwitch 037
	namespace HardwareSwitch_037
	{
		const char *name = "037 - HardwareSwitch";

		KIR5::SubBitmap HardwareSwitch;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			HardwareSwitch = Res::tiles[Res::Tiles::_037_HardwareSwitch];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Bedrock::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Bedrock"] = Bedrock::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Bedrock::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Bedrock::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			HardwareSwitch.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			HardwareSwitch.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//OneWayPortDownGrav 038
	namespace OneWayPortDownGrav_038
	{
		const char *name = "038 - OneWayPortDownGrav";

		KIR5::SubBitmap OneWayPortDown;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			OneWayPortDown = Res::tiles[Res::Tiles::_038_OneWayPortDownGrav];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::SwapsGravity | Brick::CanBeExploded | Brick::PassageFromTop);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			OneWayPortDown.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			OneWayPortDown.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//OneWayPortLeftGrav 039
	namespace OneWayPortLeftGrav_039
	{
		const char *name = "039 - OneWayPortLeftGrav";

		KIR5::SubBitmap OneWayPortLeft;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			OneWayPortLeft = Res::tiles[Res::Tiles::_039_OneWayPortLeftGrav];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::SwapsGravity | Brick::CanBeExploded | Brick::PassageFromRight);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			OneWayPortLeft.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			OneWayPortLeft.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//OneWayPortRightGrav 040
	namespace OneWayPortRightGrav_040
	{
		const char *name = "040 - OneWayPortRightGrav";

		KIR5::SubBitmap OneWayPortRight;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			OneWayPortRight = Res::tiles[Res::Tiles::_040_OneWayPortRightGrav];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::SwapsGravity | Brick::CanBeExploded | Brick::PassageFromLeft);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			OneWayPortRight.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			OneWayPortRight.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}

	//OneWayPortUpGrav 041
	namespace OneWayPortUpGrav_041
	{
		const char *name = "041 - OneWayPortUpGrav";

		KIR5::SubBitmap OneWayPortUp;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			OneWayPortUp = Res::tiles[Res::Tiles::_041_OneWayPortUpGrav];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			_brick.SetFlags(Brick::SwapsGravity | Brick::CanBeExploded | Brick::PassageFromBottom);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			return json;
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			OneWayPortUp.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			OneWayPortUp.drawScaled(x, y, w, h);
		}
		void Finalize(OBJECT_FINALIZE_PARAM)
		{

		}
	}
}
