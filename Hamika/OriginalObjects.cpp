#include "OriginalObjects.h"
#include "Space.h"
#include "Bedrock.h"
#include "BaseFunctions.h"
#include "Tools.h"

#include <KIR/KIR4_console.h>

namespace Object
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
	}


	//Zonk 001
	namespace Zonk_001
	{
		const char *name = "001 - Zonk";

		Res::Slides ZonkMoveHorizontal;

		constexpr float moveSpeed = CPS / 13.99f;
		constexpr float rollSpeed = moveSpeed;

		struct Specific
		{
			DRAW_NUMBER_T draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			ZonkMoveHorizontal.initialize(Res::tiles[Res::Tiles::_001_ZonkMoveHorizontal], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->draw_number_ = 0;

			stack->o->SetMoveSpeed({rollSpeed,moveSpeed});
			stack->o->SetFlags(Brick::CanBeExploded | Brick::RollOff | Brick::CanPushLeft | Brick::CanPushRight);
			stack->o->enablePhysics();

			FallAndRoll::Specific *fall_and_roll_spec = *stack;
			FallAndRoll::Create(OBJECT_CREATER_CALL);
			FallAndRoll::setHeavy(fall_and_roll_spec, true);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["draw_number"] = spec->draw_number_;

			json["\\FallAndRoll"] = FallAndRoll::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			FallAndRoll::Timer(OBJECT_TIMER_CALL);
			if (stack->o->IsMoveLeft())
			{
				DRAW_NUMBER_DESC(stack->o->move.x, 1.f, spec->draw_number_, stack->o, ZonkMoveHorizontal);
			}
			else if (stack->o->IsMoveRight())
			{
				DRAW_NUMBER_ASC(stack->o->move.x, -1.f, spec->draw_number_, stack->o, ZonkMoveHorizontal);
			}
			else
			{
				if (spec->draw_number_ != 0)
				{
					spec->draw_number_ = 0;
					stack->o->requests.draw = true;
				}
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			FallAndRoll::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			ZonkMoveHorizontal[spec->draw_number_].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			ZonkMoveHorizontal[0].drawScaled(x, y, w, h);
		}
	}


	//BaseX 002
	namespace BaseX_002
	{
		const char *name = "002 - BaseX";

		KIR5::SubBitmap BaseX;
		Res::Slides BaseLineDisappear;

		constexpr float disappearTime = 0.23f;

		struct Specific
		{
			float disappearTimer;
			DRAW_NUMBER_T draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			BaseX = Res::tiles[Res::Tiles::_002_BaseX];
			BaseLineDisappear.initialize(Res::tiles[Res::Tiles::_002_BaseX_square_disappear], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->disappearTimer = 0;
			spec->draw_number_ = 0;

			stack->o->SetFlags(Brick::CanBeExploded | Brick::MurphyStepOn | Brick::MurphyCanSuck | Brick::GiveGravityDelay);

			stack->o->events.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["disappearTimer"] = spec->disappearTimer;
			json["draw_number"] = spec->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			if (ACTION_TIMER(spec->disappearTimer,
							 disappearTime,
							 stack->o,
							 [&stack, &spec]()->bool
			{
				return spec->disappearTimer == ACTION_TIMER_START;
			},
							 [&stack, &spec]()->bool
			{
				spec->draw_number_ = 0;
				stack->o->events.timer = true;
				stack->o->events.update = false;
				stack->o->requests.remove = false;
				return true;
			},
				[&stack, &spec]()->bool
			{
				DRAW_NUMBER_ASC(spec->disappearTimer,
				disappearTime,
				spec->draw_number_,
				stack->o, BaseLineDisappear);
			return true;
			},
				[&stack, &spec]()->bool
			{
				stack->o->events.clear();
				stack->o->requests.remove = true;
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Specific *spec = *stack;

			if (stack->o->requests.remove)
			{
				spec->disappearTimer = ACTION_TIMER_START;
				Timer(stack);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			if (spec->disappearTimer > 0)
			{
				BaseLineDisappear[spec->draw_number_].drawScaled(x, y, w, h);
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

		struct Specific
		{
			float electricDelayTimer;
			float electricTimer;
			float disappearTimer;
			DRAW_NUMBER_T draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Bug.initialize(Res::tiles[Res::Tiles::_013_Bug], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->electricDelayTimer = 0;
			spec->electricTimer = ACTION_TIMER_START;
			spec->disappearTimer = 0;
			spec->draw_number_ = 0;
			stack->o->SetFlags(Brick::CanBeExploded | Brick::MurphyStepOn | Brick::MurphyCanSuck | Brick::GiveGravityDelay | Brick::MurphyDies);

			stack->o->events.timer = true;
			stack->o->events.update = true;

			stack->o->requests.timer = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["electricDelayTimer"] = spec->electricDelayTimer;
			json["electricTimer"] = spec->electricTimer;
			json["disappearTimer"] = spec->disappearTimer;
			json["draw_number"] = spec->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			stack->o->requests.timer = true;

			if (ACTION_TIMER(spec->disappearTimer,
							 disappearTime,
							 stack->o,
							 [&stack, &spec]()->bool
			{
				return spec->disappearTimer == ACTION_TIMER_START;
			},
							 [&stack, &spec]()->bool
			{
				spec->draw_number_ = 0;
				stack->o->events.timer = true;
				stack->o->events.update = false;
				stack->o->requests.remove = false;
				return true;
			},
				[&stack, &spec]()->bool
			{
				DRAW_NUMBER_ASC(spec->disappearTimer,
				disappearTime,
				spec->draw_number_,
				stack->o, BaseX_002::BaseLineDisappear);
			return true;
			},
				[&stack, &spec]()->bool
			{
				stack->o->events.clear();
				stack->o->requests.remove = true;
				return true;
			}))
			{
				return;
			}


			if (ACTION_TIMER(spec->electricDelayTimer,
							 electricDelayTime,
							 stack->o,
							 [&stack, &spec]()->bool
			{
				return spec->electricDelayTimer == ACTION_TIMER_START;
			},
							 [&stack, &spec]()->bool
			{
				return true;
			},
				[&stack, &spec]()->bool
			{
				return true;
			},
				[&stack, &spec]()->bool
			{
				return true;
			}))
			{
				return;
			}

			if (ACTION_TIMER(spec->electricTimer,
							 electricTime,
							 stack->o,
							 [&stack, &spec]()->bool
			{
				return stack->o->scene->rollTrigger(stack->o, 1300, triggerChance) || spec->electricTimer == ACTION_TIMER_START;
			},
							 [&stack, &spec]()->bool
			{
				spec->draw_number_ = 0;

				stack->o->AddFlags(Brick::Flags::MurphyDies);
				stack->o->requests.draw = true;
				return true;
			},
				[&stack, &spec]()->bool
			{
				DRAW_NUMBER_ASC(spec->electricTimer,
				electricTime,
				spec->draw_number_,
				stack->o, Bug);
			return true;
			},
				[&stack, &spec]()->bool
			{
				spec->electricDelayTimer = ACTION_TIMER_START;
				stack->o->RemoveFlags(Brick::Flags::MurphyDies);
				stack->o->requests.draw = true;
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Specific *spec = *stack;

			if (stack->o->requests.remove)
			{
				spec->disappearTimer = ACTION_TIMER_START;
				Timer(stack);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			if (spec->disappearTimer > 0)
			{
				BaseX_002::BaseLineDisappear[spec->draw_number_].drawScaled(x, y, w, h);
			}
			else if (spec->electricTimer > 0)
			{
				Bug[spec->draw_number_].drawScaled(x, y, w, h);
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

		struct Specific
		{
			float disappearTimer;
			DRAW_NUMBER_T draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			InfotronDisappear.initialize(Res::tiles[Res::Tiles::_014_InfotronDisappear], Res::tiles[Res::BitmapPool::Fallback]);
			InfotronMoveHorizontal.initialize(Res::tiles[Res::Tiles::_014_InfotronMoveHorizontal], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->disappearTimer = 0;
			spec->draw_number_ = 0;
			stack->o->SetMoveSpeed({rollSpeed,moveSpeed});
			stack->o->SetFlags(Brick::CanBeExploded | Brick::RollOff | Brick::MurphyStepOn | Brick::MurphyCanSuck | Brick::Give1Score);

			FallAndRoll::Specific *fall_and_roll_spec = *stack;
			FallAndRoll::Create(OBJECT_CREATER_CALL);
			FallAndRoll::setHeavy(fall_and_roll_spec, true);

			stack->o->events.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["disappearTimer"] = spec->disappearTimer;
			json["draw_number"] = spec->draw_number_;
			json["\\FallAndRoll"] = FallAndRoll::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			if (ACTION_TIMER(spec->disappearTimer,
							 disappearTime,
							 stack->o,
							 [&stack, &spec]()->bool
			{
				return spec->disappearTimer == ACTION_TIMER_START;
			},
							 [&stack, &spec]()->bool
			{
				spec->draw_number_ = 0;
				stack->o->events.timer = true;
				stack->o->events.update = false;
				stack->o->requests.remove = false;
				return true;
			},
				[&stack, &spec]()->bool
			{
				DRAW_NUMBER_ASC(spec->disappearTimer,
				disappearTime,
				spec->draw_number_,
				stack->o, InfotronDisappear);
			return true;
			},
				[&stack, &spec]()->bool
			{
				stack->o->events.clear();
				stack->o->requests.remove = true;
				return true;
			}))
			{
				return;
			}

			FallAndRoll::Timer(OBJECT_TIMER_CALL);
			if (stack->o->IsMoveLeft())
			{
				DRAW_NUMBER_DESC(stack->o->move.x, 1.f, spec->draw_number_, stack->o, InfotronMoveHorizontal);
			}
			else if (stack->o->IsMoveRight())
			{
				DRAW_NUMBER_ASC(stack->o->move.x, -1.f, spec->draw_number_, stack->o, InfotronMoveHorizontal);
			}
			else
			{
				if (spec->draw_number_ != 0)
				{
					spec->draw_number_ = 0;
					stack->o->requests.draw = true;
				}
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Specific *spec = *stack;

			if (stack->o->requests.remove)
			{
				spec->disappearTimer = ACTION_TIMER_START;
				Timer(stack);
			}

			if (!stack->o->scene->IamRemain(stack->o))
			{
				Object::Brick::Stack::Handler<Specific> sHandler(stack);
				Specific *spec = sHandler;
				FallAndRoll::Update(OBJECT_UPDATE_CALL);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			if (spec->disappearTimer > 0)
			{
				InfotronDisappear[spec->draw_number_].drawScaled(x, y, w, h);
			}
			else
			{
				InfotronMoveHorizontal[spec->draw_number_].drawScaled(x, y, w, h);
			}
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			InfotronMoveHorizontal[0].drawScaled(x, y, w, h);
		}
	}

	//Exit 015
	namespace Exit_015
	{
		const char *name = "015 - Exit";

		Res::Slides Exit;

		constexpr float animateTime = 0.5f;

		struct Specific
		{
			DRAW_NUMBER_T draw_number_;
			float animateTimer;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Exit.initialize(Res::tiles[Res::Tiles::_015_Exit], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Specific *spec = *stack;

			DRAW_NUMBER_ASC_INIT(spec->draw_number_, stack->o, Exit);

			spec->draw_number_ = 0;
			spec->animateTimer = 0.f;

			stack->o->events.update = true;
			stack->o->requests.update = true;

			stack->o->SetFlags(Brick::CanBeExploded);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Specific *spec = *stack;
			Json json;

			json["draw_number"] = spec->draw_number_;
			json["animateTimer"] = spec->animateTimer;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Specific *spec = *stack;

			if (ACTION_TIMER(spec->animateTimer,
							 animateTime,
							 stack->o,
							 [&stack, &spec]()->bool
			{
				return spec->animateTimer == ACTION_TIMER_START;
			},
							 [&stack, &spec]()->bool
			{
				spec->draw_number_ = 0;
				stack->o->events.timer = true;
				return true;
			},
				[&stack, &spec]()->bool
			{
				DRAW_NUMBER_ASC(spec->animateTimer,
				animateTime,
				spec->draw_number_,
				stack->o, Exit);
			return true;
			},
				[&stack, &spec]()->bool
			{
				stack->o->events.clear();
				stack->o->requests.clear();
				stack->o->requests.draw = true;
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Specific *spec = *stack;

			spec->animateTimer = stack->o->scene->getScoreToCollect() > 0 ? 0.f : ACTION_TIMER_START;
			stack->o->events.update = false;

			Timer(stack);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Specific *spec = *stack;

			Exit[spec->draw_number_].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Exit[Exit.getCount() - 1].drawScaled(x, y, w, h);
		}

		void Open(Brick *o)
		{
			Type::Coord coord{};
			for (coord.x = 0; coord.x < o->scene->MapSize().width; coord.x++)
			{
				for (coord.y = 0; coord.y < o->scene->MapSize().height; coord.y++)
				{
					Brick *oe = o->scene->GetObjectU(coord);
					if (oe->id == ObjectID::Exit)
					{
						Brick::Stack stack(oe);
						Specific *spec = stack;
						spec->animateTimer = ACTION_TIMER_START;
						Timer(&stack);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::Passage);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::PassageHorizontal);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::PassageVertical);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::PassageFromTop);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::PassageFromRight);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::PassageFromLeft);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::PassageFromBottom);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::RollOff);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::RollOff);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::RollOff);
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
	}

	//Electrons 026
	namespace Electrons_026
	{
		const char *name = "026 - Electrons";

		constexpr float moveSpeed = CPS / 13.99f;
		constexpr float rotateSpeed = Type::Rotations::_180 / (1 / moveSpeed);

		Res::Slides Electrons;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Electrons.initialize(Res::tiles[Res::Tiles::_026_Electrons], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			stack->o->SetRotationSpeed(rotateSpeed);
			stack->o->SetMoveSpeed({moveSpeed,moveSpeed});
			stack->o->SetTranslationID(ObjectID::Infotron);
			stack->o->SetFlags(Brick::CanBeExploded | Brick::ExplosionType3);

			{
				Animator::Specific *spec = *stack;
				Animator::Create(OBJECT_CREATER_CALL);
				spec->SetNumberOfFrames(Electrons.getCount());
				spec->SetAnimationTime(0.8f);
			}

			MoveLeftWay::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Animator"] = Animator::Print(OBJECT_PRINTER_CALL);
			json["\\MoveLeftWay"] = MoveLeftWay::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Animator::Timer(OBJECT_TIMER_CALL);
			MoveLeftWay::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Animator::Update(OBJECT_UPDATE_CALL);
			MoveLeftWay::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Animator::Specific *spec = *stack;
			Electrons[spec->GetDrawNumber()].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Electrons[0].drawScaled(x, y, w, h);
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

		struct Specific
		{
			DRAW_NUMBER_T draw_number_;
		};

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
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->draw_number_ = 0;

			stack->o->SetRotationSpeed(rotateSpeed);
			stack->o->SetMoveSpeed({moveSpeed,moveSpeed});
			stack->o->SetFlags(Brick::CanBeExploded | Brick::ExplosionType3);
			stack->o->SetTranslationID(ObjectID::Space);

			MoveLeftWay::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["draw_number"] = spec->draw_number_;
			json["\\MoveLeftWay"] = MoveLeftWay::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			MoveLeftWay::Timer(OBJECT_TIMER_CALL);

			if (stack->o->isActionMove())
			{
				DRAW_NUMBER_ASC(stack->o->GetAbsMove(), 1.f, spec->draw_number_, stack->o, SnikSnakMove[stack->o->getMoveDirection()]);
			}
			else
			{
				DRAW_NUMBER_DESC(Brick::GetRealRotation(
					stack->o->rotation + (Type::Rotations::_360 / (float)SnikSnakRotate.getCount()) / 2.f),
					Type::Rotations::_360, spec->draw_number_, stack->o, SnikSnakRotate);
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			MoveLeftWay::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			if (stack->o->isActionMove())
			{
				SnikSnakMove[stack->o->getMoveDirection()][spec->draw_number_].drawScaled(x, y, w, h);
			}
			else
			{
				SnikSnakRotate[spec->draw_number_].drawScaled(x, y, w, h);
			}
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			SnikSnakRotate[0].drawScaled(x, y, w, h);
		}
	}

	//Terminal 028
	namespace Terminal_028
	{
		const char *name = "028 - Terminal";

		Res::Slides Terminal;

		constexpr float activatedAnimateTime = 0.7f;
		constexpr float animateTime = 3.f;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Terminal.initialize(Res::tiles[Res::Tiles::_028_Terminal], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{

			Animator::Specific *spec = *stack;
			Animator::Create(OBJECT_CREATER_CALL);
			spec->SetNumberOfFrames(Terminal.getCount());
			spec->SetAnimationTime(animateTime);

			stack->o->SetFlags(Brick::ButtonPush | Brick::CanBeExploded);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Animator"] = Animator::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Animator::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Animator::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Animator::Specific *spec = *stack;
			Terminal[spec->GetDrawNumber()].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Terminal[0].drawScaled(x, y, w, h);
		}

		void speedUpTerminalSpeed(Brick *o)
		{
			Brick::Stack stack(o);
			Animator::Specific *spec = stack;
			spec->SetAnimationTime(activatedAnimateTime);
		}

		void Pushed(Brick *o)
		{
			Brick::Stack stack(o);
			Animator::Specific *spec = stack;
			if (spec->time != activatedAnimateTime)
			{
				Type::Coord coord;
				for (coord.x = 0; coord.x < o->scene->MapSize().width; coord.x++)
				{
					for (coord.y = 0; coord.y < o->scene->MapSize().height; coord.y++)
					{
						if (o->scene->GetObjectU(coord))
						{
							if (o->scene->GetObjectU(coord)->id == ObjectID::Utility3)
							{
								o->scene->blowup(o->scene->GetObjectU(coord));
							}
							else if (o->scene->GetObjectU(coord)->id == o->id)
							{
								speedUpTerminalSpeed(o->scene->GetObjectU(coord));
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

		struct Specific
		{
			bool active;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Utility1 = Res::tiles[Res::Tiles::_029_Utility1];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->active = false;
			stack->o->SetFlags(Brick::CanBeExploded | Brick::ExplosionType3 | Brick::CanPushLeft | Brick::CanPushRight);
			stack->o->SetMoveSpeed({moveSpeed,moveSpeed});
			stack->o->SetTranslationID(ObjectID::Space);

			Fall::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["active"] = spec->active;
			json["\\Fall"] = Fall::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Fall::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Fall::Update(OBJECT_UPDATE_CALL);
			spec = sHandler;

			if (stack->o->isActionMove())
			{
				spec->active = true;
			}
			else if (spec->active)
			{
				stack->o->scene->ObjectArrived(stack->o->GetCoord());
				stack->o->scene->blowup(stack->o);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Utility1.drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Utility1.drawScaled(x, y, w, h);
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

		struct Specific
		{
			float activateTimer;
			float disappearTimer;
			DRAW_NUMBER_T draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Utility2.initialize(Res::tiles[Res::Tiles::_030_Utility2], Res::tiles[Res::BitmapPool::Fallback]);
			Utility2Activated = Res::tiles[Res::Tiles::_030_Utility2Activated];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->activateTimer = 0.f;
			spec->disappearTimer = 0.f;
			spec->draw_number_ = 0;
			stack->o->SetFlags(Brick::CanBeExploded | Brick::MurphyCanSuck | Brick::Give1Unity | Brick::MurphyStepOn);

			stack->o->events.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["activateTimer"] = spec->activateTimer;
			json["disappearTimer"] = spec->disappearTimer;
			json["draw_number"] = spec->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			if (ACTION_TIMER(spec->disappearTimer,
							 disappearTime,
							 stack->o,
							 [&stack, &spec]()->bool
			{
				return spec->disappearTimer == ACTION_TIMER_START;
			},
							 [&stack, &spec]()->bool
			{
				spec->draw_number_ = 0;
				stack->o->events.timer = true;
				stack->o->events.update = false;
				stack->o->requests.remove = false;
				return true;
			},
				[&stack, &spec]()->bool
			{
				DRAW_NUMBER_ASC(spec->disappearTimer,
				disappearTime,
				spec->draw_number_,
				stack->o, Utility2);
			return true;
			},
				[&stack, &spec]()->bool
			{
				stack->o->events.clear();
				stack->o->requests.remove = true;
				return true;
			}))
			{
				return;
			}

			if (ACTION_TIMER(spec->activateTimer,
							 activateTime,
							 stack->o,
							 [&stack, &spec]()->bool
			{
				return spec->activateTimer == ACTION_TIMER_START;
			},
							 [&stack, &spec]()->bool
			{
				spec->draw_number_ = 0;
				stack->o->SetFlags(Brick::CanBeExploded | Brick::ExplosionType3);
				stack->o->events.timer = true;
				stack->o->events.update = false;
				stack->o->requests.remove = false;
				return true;
			},
				[&stack, &spec]()->bool
			{
				DRAW_NUMBER_T draw_number = (DRAW_NUMBER_T(spec->activateTimer * 10)) % 2;
				if (spec->draw_number_ != draw_number)
				{
					spec->draw_number_ = draw_number;
					stack->o->requests.draw = true;
				}
				return true;
			},
				[&stack, &spec]()->bool
			{
				stack->o->events.clear();
				stack->o->scene->blowup(stack->o);
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Specific *spec = *stack;

			if (stack->o->requests.remove)
			{
				spec->disappearTimer = ACTION_TIMER_START;
				Timer(stack);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			if (spec->activateTimer > 0 && spec->draw_number_ == 1)
			{
				Utility2Activated.drawScaled(x, y, w, h);
			}
			else
			{
				Utility2[spec->draw_number_].drawScaled(x, y, w, h);
			}
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Utility2[0].drawScaled(x, y, w, h);
		}

		void Activate(Brick *o)
		{
			Object::Brick::Stack stack(o);
			Specific *spec = stack;

			spec->activateTimer = ACTION_TIMER_START;
			Timer(&stack);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::ExplosionType3 | Brick::CanPush);
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
	}

	//ExplosionEffect 032
	namespace ExplosionEffect_032
	{
		const char *name = "032 - ExplosionEffect";

		Res::Slides Blasting;

		constexpr float explosionTime = 0.6f;

		struct Specific
		{
			float explosionTimer;
			DRAW_NUMBER_T draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Blasting.initialize(Res::tiles[Res::Tiles::_032_Explosion], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->explosionTimer = explosionTime;
			spec->draw_number_ = 0;

			stack->o->events.timer = false;
			stack->o->events.update = true;
			stack->o->events.topDraw = true;

			stack->o->requests.update = true;

			stack->o->SetFlags(Brick::StepOn | Brick::MurphyStepOn | Brick::CanBeExploded | Brick::RollOff);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["explosionTimer"] = spec->explosionTimer;
			json["draw_number"] = spec->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			if (ACTION_TIMER(spec->explosionTimer,
							 explosionTime,
							 stack->o,
							 [&stack, &spec]()->bool
			{
				return false;
			},
							 [&stack, &spec]()->bool
			{
				return true;
			},
				[&stack, &spec]()->bool
			{
				DRAW_NUMBER_ASC(spec->explosionTimer,
				explosionTime,
				spec->draw_number_,
				stack->o, ExplosionEffect_032::Blasting);
			return true;
			},
				[&stack, &spec]()->bool
			{
				stack->o->requests.remove = true;
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			stack->o->events.timer = true;
			stack->o->requests.timer = true;
			stack->o->events.update = false;
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Blasting[spec->draw_number_].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Blasting[0].drawScaled(x, y, w, h);
		}
	}

	//Explosion 033
	namespace Explosion_033
	{
		const char *name = "033 - Explosion";

		constexpr float blastingTime = ExplosionEffect_032::explosionTime / 2.f;

		struct Specific
		{
			float explosionTimer;
			float blastingTimer;
			DRAW_NUMBER_T draw_number_;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			spec->explosionTimer = ExplosionEffect_032::explosionTime;
			spec->blastingTimer = blastingTime;
			spec->draw_number_ = 0;

			stack->o->SetFlags(Brick::CanBeExploded | Brick::RollOff);

			stack->o->events.timer = false;
			stack->o->events.update = true;
			stack->o->events.topDraw = true;

			stack->o->requests.update = true;

			stack->o->scene->GetObject(stack->o->GetCoord())->events.clear();
			stack->o->scene->GetObject(stack->o->GetCoord())->requests.clear();
			stack->o->scene->GetObject(stack->o->GetCoord())->RemoveFlags(Brick::Flags::CanBeKilled);
		}
		void ReCreate(OBJECT_CREATER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			spec->explosionTimer = ExplosionEffect_032::explosionTime;
			spec->draw_number_ = 0;

			if (spec->blastingTimer == 0)
			{
				spec->blastingTimer = blastingTime;

				stack->o->scene->GetObject(stack->o->GetCoord())->events.clear();
				stack->o->scene->GetObject(stack->o->GetCoord())->requests.clear();
				stack->o->scene->GetObject(stack->o->GetCoord())->RemoveFlags(Brick::Flags::CanBeKilled);
			}

			stack->o->events.timer = false;
			stack->o->events.update = true;

			stack->o->requests.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			Json json;

			json["explosionTimer"] = spec->explosionTimer;
			json["blastingTimer"] = spec->blastingTimer;
			json["draw_number"] = spec->draw_number_;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			ACTION_TIMER(spec->blastingTimer,
						 blastingTime,
						 stack->o,
						 [&stack, &spec]()->bool
			{
				return false;
			},
						 [&stack, &spec]()->bool
			{
				return true;
			},
				[&stack, &spec]()->bool
			{
				return true;
			},
				[&stack, &spec]()->bool
			{
				Brick *object = stack->o->scene->GetObject(stack->o->GetCoord());
				if (object->GetFlags() & Brick::ExplosionType)
				{
					stack->o->scene->blowup(object);
				}
				else
				{
					stack->o->scene->ObjectVirtualArrived(stack->o->GetCoord());
					stack->o->scene->ObjectPut(stack->o->GetCoord(), stack->o->GetObjectIDremain());
				}
				return true;
			});

			ACTION_TIMER(spec->explosionTimer,
						 ExplosionEffect_032::explosionTime,
						 stack->o,
						 [&stack, &spec]()->bool
			{
				return false;
			},
						 [&stack, &spec]()->bool
			{
				return true;
			},
				[&stack, &spec]()->bool
			{
				DRAW_NUMBER_ASC(spec->explosionTimer,
				ExplosionEffect_032::explosionTime,
				spec->draw_number_,
				stack->o, ExplosionEffect_032::Blasting);
			return true;
			},
				[&stack, &spec]()->bool
			{
				stack->o->requests.remove = true;
				return true;
			});
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;

			stack->o->events.timer = true;
			stack->o->requests.timer = true;
			stack->o->events.update = false;
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Object::Brick::Stack::Handler<Specific> sHandler(stack);
			Specific *spec = sHandler;
			ExplosionEffect_032::Blasting[spec->draw_number_].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			ExplosionEffect_032::Blasting[0].drawScaled(x, y, w, h);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::RollOff);
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
			stack->o->SetFlags(Brick::CanBeExploded | Brick::RollOff);
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
	}
}