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

		const float moveSpeed = 4.401544f;
		const float rollSpeed = 4.401544f;

		struct Specific
		{
			int drawNumber;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			ZonkMoveHorizontal.initialize(Res::tiles[Res::Tiles::_001_ZonkMoveHorizontal], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->drawNumber = 0;

			stack->o->SetMoveSpeed({rollSpeed,moveSpeed});
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::RollOff | ObjectBase::CanPushLeft | ObjectBase::CanPushRight);
			stack->o->EnablePhysics();

			MoveDownHeavy::Create(OBJECT_CREATER_CALL);
			RollDown::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["drawNumber"] = s->drawNumber;

			json["\\MoveDownHeavy"] = MoveDownHeavy::Print(OBJECT_PRINTER_CALL);
			json["\\RollDown"] = RollDown::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);
			MoveDownHeavy::Timer(OBJECT_TIMER_CALL);
			RollDown::Timer(OBJECT_TIMER_CALL);
			if (stack->o->IsMoveLeft())
			{
				DRAW_NUMBER_R(stack->o->move.x, 1.f, s->drawNumber, stack->o, ZonkMoveHorizontal);
			}
			else if (stack->o->IsMoveRight())
			{
				DRAW_NUMBER(stack->o->move.x, -1.f, s->drawNumber, stack->o, ZonkMoveHorizontal);
			}
			else
			{
				if (s->drawNumber != 0)
				{
					s->drawNumber = 0;
					stack->o->requests.draw = true;
				}
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			pops(Specific, s);
			MoveDownHeavy::Update(OBJECT_UPDATE_CALL);
			RollDown::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			pops(Specific, s);
			ZonkMoveHorizontal[s->drawNumber].drawScaled(x, y, w, h);
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

		const float disappearTime = 0.23f;

		struct Specific
		{
			float disappearTimer;
			int drawNumber;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			BaseX = Res::tiles[Res::Tiles::_002_BaseX];
			BaseLineDisappear.initialize(Res::tiles[Res::Tiles::_002_BaseX_square_disappear], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->disappearTimer = 0;
			s->drawNumber = 0;

			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::MurphyStepOn | ObjectBase::MurphyCanSniff | ObjectBase::GiveGravityDelay);

			stack->o->events.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["disappearTimer"] = s->disappearTimer;
			json["drawNumber"] = s->drawNumber;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);

			if (ACTION_TIMER(s->disappearTimer,
							 disappearTime,
							 stack->o,
							 [&stack, &s]()->bool
			{
				return s->disappearTimer == ACTION_TIMER_START;
			},
							 [&stack, &s]()->bool
			{
				s->drawNumber = 0;
				stack->o->events.timer = true;
				stack->o->events.update = false;
				stack->o->requests.remove = false;
				return true;
			},
				[&stack, &s]()->bool
			{
				DRAW_NUMBER(s->disappearTimer,
				disappearTime,
				s->drawNumber,
				stack->o, BaseLineDisappear);
			return true;
			},
				[&stack, &s]()->bool
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
			gets(Specific, s);

			if (stack->o->requests.remove)
			{
				s->disappearTimer = ACTION_TIMER_START;
				Timer(stack);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			pops(Specific, s);

			if (s->disappearTimer > 0)
			{
				BaseLineDisappear[s->drawNumber].drawScaled(x, y, w, h);
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
			int drawNumber;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Bug.initialize(Res::tiles[Res::Tiles::_013_Bug], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->electricDelayTimer = 0;
			s->electricTimer = ACTION_TIMER_START;
			s->disappearTimer = 0;
			s->drawNumber = 0;
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::MurphyStepOn | ObjectBase::MurphyCanSniff | ObjectBase::GiveGravityDelay | ObjectBase::MurphyDies);

			stack->o->events.timer = true;
			stack->o->events.update = true;

			stack->o->requests.timer = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["electricDelayTimer"] = s->electricDelayTimer;
			json["electricTimer"] = s->electricTimer;
			json["disappearTimer"] = s->disappearTimer;
			json["drawNumber"] = s->drawNumber;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);

			stack->o->requests.timer = true;

			if (ACTION_TIMER(s->disappearTimer,
							 disappearTime,
							 stack->o,
							 [&stack, &s]()->bool
			{
				return s->disappearTimer == ACTION_TIMER_START;
			},
							 [&stack, &s]()->bool
			{
				s->drawNumber = 0;
				stack->o->events.timer = true;
				stack->o->events.update = false;
				stack->o->requests.remove = false;
				return true;
			},
				[&stack, &s]()->bool
			{
				DRAW_NUMBER(s->disappearTimer,
				disappearTime,
				s->drawNumber,
				stack->o, BaseX_002::BaseLineDisappear);
			return true;
			},
				[&stack, &s]()->bool
			{
				stack->o->events.clear();
				stack->o->requests.remove = true;
				return true;
			}))
			{
				return;
			}


			if (ACTION_TIMER(s->electricDelayTimer,
							 electricDelayTime,
							 stack->o,
							 [&stack, &s]()->bool
			{
				return s->electricDelayTimer == ACTION_TIMER_START;
			},
							 [&stack, &s]()->bool
			{
				return true;
			},
				[&stack, &s]()->bool
			{
				return true;
			},
				[&stack, &s]()->bool
			{
				return true;
			}))
			{
				return;
			}

			if (ACTION_TIMER(s->electricTimer,
							 electricTime,
							 stack->o,
							 [&stack, &s]()->bool
			{
				return stack->o->scene->rollTrigger(stack->o, 1300, triggerChance) || s->electricTimer == ACTION_TIMER_START;
			},
							 [&stack, &s]()->bool
			{
				s->drawNumber = 0;

				stack->o->AddFlags(ObjectBase::Flags::MurphyDies);
				stack->o->requests.draw = true;
				return true;
			},
				[&stack, &s]()->bool
			{
				DRAW_NUMBER(s->electricTimer,
				electricTime,
				s->drawNumber,
				stack->o, Bug);
			return true;
			},
				[&stack, &s]()->bool
			{
				s->electricDelayTimer = ACTION_TIMER_START;
				stack->o->RemoveFlags(ObjectBase::Flags::MurphyDies);
				stack->o->requests.draw = true;
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			gets(Specific, s);

			if (stack->o->requests.remove)
			{
				s->disappearTimer = ACTION_TIMER_START;
				Timer(stack);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			pops(Specific, s);

			if (s->disappearTimer > 0)
			{
				BaseX_002::BaseLineDisappear[s->drawNumber].drawScaled(x, y, w, h);
			}
			else if (s->electricTimer > 0)
			{
				Bug[s->drawNumber].drawScaled(x, y, w, h);
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

		const float moveSpeed = 4.401544f;
		const float rollSpeed = 4.401544f;
		const float disappearTime = 0.23f;

		struct Specific
		{
			float disappearTimer;
			int drawNumber;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			InfotronDisappear.initialize(Res::tiles[Res::Tiles::_014_InfotronDisappear], Res::tiles[Res::BitmapPool::Fallback]);
			InfotronMoveHorizontal.initialize(Res::tiles[Res::Tiles::_014_InfotronMoveHorizontal], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->disappearTimer = 0;
			s->drawNumber = 0;
			stack->o->SetMoveSpeed({rollSpeed,moveSpeed});
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::RollOff | ObjectBase::MurphyStepOn | ObjectBase::MurphyCanSniff | ObjectBase::Give1Score);

			MoveDownHeavy::Create(OBJECT_CREATER_CALL);
			RollDown::Create(OBJECT_CREATER_CALL);

			stack->o->events.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["disappearTimer"] = s->disappearTimer;
			json["drawNumber"] = s->drawNumber;
			json["\\MoveDownHeavy"] = MoveDownHeavy::Print(OBJECT_PRINTER_CALL);
			json["\\RollDown"] = RollDown::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);

			if (ACTION_TIMER(s->disappearTimer,
							 disappearTime,
							 stack->o,
							 [&stack, &s]()->bool
			{
				return s->disappearTimer == ACTION_TIMER_START;
			},
							 [&stack, &s]()->bool
			{
				s->drawNumber = 0;
				stack->o->events.timer = true;
				stack->o->events.update = false;
				stack->o->requests.remove = false;
				return true;
			},
				[&stack, &s]()->bool
			{
				DRAW_NUMBER(s->disappearTimer,
				disappearTime,
				s->drawNumber,
				stack->o, InfotronDisappear);
			return true;
			},
				[&stack, &s]()->bool
			{
				stack->o->events.clear();
				stack->o->requests.remove = true;
				return true;
			}))
			{
				return;
			}

			MoveDownHeavy::Timer(OBJECT_TIMER_CALL);
			RollDown::Timer(OBJECT_TIMER_CALL);
			if (stack->o->IsMoveLeft())
			{
				DRAW_NUMBER_R(stack->o->move.x, 1.f, s->drawNumber, stack->o, InfotronMoveHorizontal);
			}
			else if (stack->o->IsMoveRight())
			{
				DRAW_NUMBER(stack->o->move.x, -1.f, s->drawNumber, stack->o, InfotronMoveHorizontal);
			}
			else
			{
				if (s->drawNumber != 0)
				{
					s->drawNumber = 0;
					stack->o->requests.draw = true;
				}
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			gets(Specific, s);

			if (stack->o->requests.remove)
			{
				s->disappearTimer = ACTION_TIMER_START;
				Timer(stack);
			}

			if (!stack->o->scene->IamRemain(stack->o))
			{
				pops(Specific, s);
				MoveDownHeavy::Update(OBJECT_UPDATE_CALL);
				RollDown::Update(OBJECT_UPDATE_CALL);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			pops(Specific, s);

			if (s->disappearTimer > 0)
			{
				InfotronDisappear[s->drawNumber].drawScaled(x, y, w, h);
			}
			else
			{
				InfotronMoveHorizontal[s->drawNumber].drawScaled(x, y, w, h);
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
			int drawNumber;
			float animateTimer;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Exit.initialize(Res::tiles[Res::Tiles::_015_Exit], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			gets(Specific, s);

			s->drawNumber = 0;
			s->animateTimer = stack->o->scene->getScoreToCollect() > 0 ? 0.f : ACTION_TIMER_START;

			stack->o->SetFlags(ObjectBase::CanBeExplosion);
			Timer(stack);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["drawNumber"] = s->drawNumber;
			json["animateTimer"] = s->animateTimer;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);

			if (ACTION_TIMER(s->animateTimer,
							 animateTime,
							 stack->o,
							 [&stack, &s]()->bool
			{
				return s->animateTimer == ACTION_TIMER_START;
			},
							 [&stack, &s]()->bool
			{
				s->drawNumber = 0;
				stack->o->events.timer = true;
				return true;
			},
				[&stack, &s]()->bool
			{
				DRAW_NUMBER(s->animateTimer,
				animateTime,
				s->drawNumber,
				stack->o, Exit);
			return true;
			},
				[&stack, &s]()->bool
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
			pops(Specific, s);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			pops(Specific, s);
			Exit[s->drawNumber].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Exit[Exit.getCount() - 1].drawScaled(x, y, w, h);
		}

		void Open(ObjectBase *o)
		{
			Type::Coord coord{};
			for (coord.x = 0; coord.x < o->scene->MapSize().width; coord.x++)
			{
				for (coord.y = 0; coord.y < o->scene->MapSize().height; coord.y++)
				{
					ObjectBase *oe = o->scene->GetObjectU(coord);
					if (oe->id == ObjectID::Exit)
					{
						maks(oe);
						gets(Specific, s);
						s->animateTimer = ACTION_TIMER_START;
						Timer(stack);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::Passage);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::PassageHorizontal);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::PassageVertical);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::PassageFromTop);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::PassageFromRight);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::PassageFromLeft);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::PassageFromBottom);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::RollOff);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::RollOff);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::RollOff);
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

		const float moveSpeed = 4.401544f;
		const float rotateSpeed = 492.156863f;

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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::ExplosionType3);

			{
				gets(Animator::Specific, s);
				Animator::Create(OBJECT_CREATER_CALL);
				s->SetNumberOfFrames(Electrons.getCount());
				s->SetAnimationTime(0.8f);
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
			gets(Animator::Specific, s);
			Electrons[s->GetDrawNumber()].drawScaled(x, y, w, h);
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

		Res::Slides SnikSnakMoveUp;
		Res::Slides SnikSnakMoveRight;
		Res::Slides SnikSnakMoveDown;
		Res::Slides SnikSnakMoveLeft;
		Res::Slides SnikSnakRotate;

		const float moveSpeed = 4.401544f;
		const float rotateSpeed = 492.156863f;

		struct Specific
		{
			int drawNumber;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			SnikSnakMoveUp.initialize(Res::tiles[Res::Tiles::_027_SnikSnakMoveUp], Res::tiles[Res::BitmapPool::Fallback]);
			SnikSnakMoveRight.initialize(Res::tiles[Res::Tiles::_027_SnikSnakMoveRight], Res::tiles[Res::BitmapPool::Fallback]);
			SnikSnakMoveDown.initialize(Res::tiles[Res::Tiles::_027_SnikSnakMoveDown], Res::tiles[Res::BitmapPool::Fallback]);
			SnikSnakMoveLeft.initialize(Res::tiles[Res::Tiles::_027_SnikSnakMoveLeft], Res::tiles[Res::BitmapPool::Fallback]);
			SnikSnakRotate.initialize(Res::tiles[Res::Tiles::_027_SnikSnakRotate], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->drawNumber = 0;

			stack->o->SetRotationSpeed(rotateSpeed);
			stack->o->SetMoveSpeed({moveSpeed,moveSpeed});
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::ExplosionType3);
			stack->o->SetTranslationID(ObjectID::Space);

			MoveLeftWay::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["drawNumber"] = s->drawNumber;
			json["\\MoveLeftWay"] = MoveLeftWay::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);

			MoveLeftWay::Timer(OBJECT_TIMER_CALL);

			if (stack->o->IsMoveUp())
			{
				DRAW_NUMBER(stack->o->GetAbsMove(), 1.f, s->drawNumber, stack->o, SnikSnakMoveUp);
			}
			else if (stack->o->IsMoveRight())
			{
				DRAW_NUMBER(stack->o->GetAbsMove(), 1.f, s->drawNumber, stack->o, SnikSnakMoveRight);
			}
			else if (stack->o->IsMoveDown())
			{
				DRAW_NUMBER(stack->o->GetAbsMove(), 1.f, s->drawNumber, stack->o, SnikSnakMoveDown);
			}
			else if (stack->o->IsMoveLeft())
			{
				DRAW_NUMBER(stack->o->GetAbsMove(), 1.f, s->drawNumber, stack->o, SnikSnakMoveLeft);
			}
			else
			{
				DRAW_NUMBER_R(
					ObjectRotationModule<float>::GetRealRotation(
						stack->o->rotation + (Type::Rotations::_360 / (float)SnikSnakRotate.getCount()) / 2.f),
					Type::Rotations::_360, s->drawNumber, stack->o, SnikSnakRotate);
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			pops(Specific, s);
			MoveLeftWay::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			pops(Specific, s);

			if (stack->o->IsMoving())
			{
				if (stack->o->IsMoveUp())
				{
					SnikSnakMoveUp[s->drawNumber].drawScaled(x, y, w, h);
				}
				else if (stack->o->IsMoveRight())
				{
					SnikSnakMoveRight[s->drawNumber].drawScaled(x, y, w, h);
				}
				else if (stack->o->IsMoveDown())
				{
					SnikSnakMoveDown[s->drawNumber].drawScaled(x, y, w, h);
				}
				else
				{
					SnikSnakMoveLeft[s->drawNumber].drawScaled(x, y, w, h);
				}
			}
			else
			{
				SnikSnakRotate[s->drawNumber].drawScaled(x, y, w, h);
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

		const float activatedAnimateTime = 0.7f;
		const float animateTime = 3.f;

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Terminal.initialize(Res::tiles[Res::Tiles::_028_Terminal], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			gets(Animator::Specific, s);
			Animator::Create(OBJECT_CREATER_CALL);
			s->SetNumberOfFrames(Terminal.getCount());
			s->SetAnimationTime(animateTime);

			stack->o->SetFlags(ObjectBase::ButtonPush);
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
			gets(Animator::Specific, s);
			Terminal[s->GetDrawNumber()].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Terminal[0].drawScaled(x, y, w, h);
		}

		void speedUpTerminalSpeed(ObjectBase *o)
		{
			maks(o);
			gets(Animator::Specific, s);
			s->SetAnimationTime(activatedAnimateTime);
		}

		void Pushed(ObjectBase *o)
		{
			maks(o);
			gets(Animator::Specific, s);
			if (s->time != activatedAnimateTime)
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
								o->scene->GetObjectU(coord)->blowUp(coord);
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

		const float moveSpeed = 4.401544f;

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
			pops(Specific, s);
			s->active = false;
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::ExplosionType3 | ObjectBase::CanPushLeft | ObjectBase::CanPushRight);
			stack->o->SetMoveSpeed({moveSpeed,moveSpeed});
			stack->o->SetTranslationID(ObjectID::Space);

			MoveDown::Create(OBJECT_CREATER_CALL);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["active"] = s->active;
			json["\\MoveDown"] = MoveDown::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);
			MoveDown::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			pops(Specific, s);
			MoveDown::Update(OBJECT_UPDATE_CALL);
			regets(Specific, s);

			if (stack->o->IsMoving())
			{
				s->active = true;
			}
			else if (s->active)
			{
				stack->o->scene->ObjectArrived(stack->o->GetCoord());
				stack->o->blowUp(stack->o->GetCoord());
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			pops(Specific, s);
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

		const float activateTime = 1.8f;
		const float disappearTime = 0.23f;

		struct Specific
		{
			float activateTimer;
			float disappearTimer;
			int drawNumber;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Utility2.initialize(Res::tiles[Res::Tiles::_030_Utility2], Res::tiles[Res::BitmapPool::Fallback]);
			Utility2Activated = Res::tiles[Res::Tiles::_030_Utility2Activated];
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->activateTimer = 0.f;
			s->disappearTimer = 0.f;
			s->drawNumber = 0;
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::MurphyCanSniff | ObjectBase::Give1Unity | ObjectBase::MurphyStepOn);

			stack->o->events.update = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["activateTimer"] = s->activateTimer;
			json["disappearTimer"] = s->disappearTimer;
			json["drawNumber"] = s->drawNumber;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);

			if (ACTION_TIMER(s->disappearTimer,
							 disappearTime,
							 stack->o,
							 [&stack, &s]()->bool
			{
				return s->disappearTimer == ACTION_TIMER_START;
			},
							 [&stack, &s]()->bool
			{
				s->drawNumber = 0;
				stack->o->events.timer = true;
				stack->o->events.update = false;
				stack->o->requests.remove = false;
				return true;
			},
				[&stack, &s]()->bool
			{
				DRAW_NUMBER(s->disappearTimer,
				disappearTime,
				s->drawNumber,
				stack->o, Utility2);
			return true;
			},
				[&stack, &s]()->bool
			{
				stack->o->events.clear();
				stack->o->requests.remove = true;
				return true;
			}))
			{
				return;
			}

			if (ACTION_TIMER(s->activateTimer,
							 activateTime,
							 stack->o,
							 [&stack, &s]()->bool
			{
				return s->activateTimer == ACTION_TIMER_START;
			},
							 [&stack, &s]()->bool
			{
				s->drawNumber = 0;
				stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::ExplosionType3);
				stack->o->events.timer = true;
				stack->o->events.update = false;
				stack->o->requests.remove = false;
				return true;
			},
				[&stack, &s]()->bool
			{
				int drawNumber = (int(s->activateTimer * 10)) % 2;
				if (s->drawNumber != drawNumber)
				{
					s->drawNumber = drawNumber;
					stack->o->requests.draw = true;
				}
				return true;
			},
				[&stack, &s]()->bool
			{
				stack->o->events.clear();
				stack->o->blowUp(stack->o->GetCoord());
				return true;
			}))
			{
				return;
			}
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			gets(Specific, s);

			if (stack->o->requests.remove)
			{
				s->disappearTimer = ACTION_TIMER_START;
				Timer(stack);
			}
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			pops(Specific, s);

			if (s->activateTimer > 0 && s->drawNumber == 1)
			{
				Utility2Activated.drawScaled(x, y, w, h);
			}
			else
			{
				Utility2[s->drawNumber].drawScaled(x, y, w, h);
			}
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Utility2[0].drawScaled(x, y, w, h);
		}

		void Activate(ObjectBase *o)
		{
			maks(o);
			gets(Specific, s);

			s->activateTimer = ACTION_TIMER_START;
			Timer(stack);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::ExplosionType3 | ObjectBase::CanPush);
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

		const float explosionTime = 0.6f;

		struct Specific
		{
			float explosionTimer;
			int drawNumber;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			Blasting.initialize(Res::tiles[Res::Tiles::_032_Explosion], Res::tiles[Res::BitmapPool::Fallback]);
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->explosionTimer = explosionTime;
			s->drawNumber = 0;

			stack->o->events.timer = true;
			stack->o->events.topDraw = true;

			stack->o->requests.timer = true;
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["explosionTimer"] = s->explosionTimer;
			json["drawNumber"] = s->drawNumber;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);
			if (ACTION_TIMER(s->explosionTimer,
							 explosionTime,
							 stack->o,
							 [&stack, &s]()->bool
			{
				return false;
			},
							 [&stack, &s]()->bool
			{
				return true;
			},
				[&stack, &s]()->bool
			{
				DRAW_NUMBER(s->explosionTimer,
				explosionTime,
				s->drawNumber,
				stack->o, ExplosionEffect_032::Blasting);
			return true;
			},
				[&stack, &s]()->bool
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
			pops(Specific, s);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			pops(Specific, s);
			Blasting[s->drawNumber].drawScaled(x, y, w, h);
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

		const float blastingTime = ExplosionEffect_032::explosionTime / 2.f;

		struct Specific
		{
			float explosionTimer;
			int drawNumber;
			bool rmobj;
		};

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			pops(Specific, s);
			s->explosionTimer = ExplosionEffect_032::explosionTime;
			s->drawNumber = 0;
			s->rmobj = false;

			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::RollOff);

			stack->o->events.timer = true;
			stack->o->events.topDraw = true;

			stack->o->requests.timer = true;

			stack->o->scene->GetObject(stack->o->GetCoord())->events.clear();
			stack->o->scene->GetObject(stack->o->GetCoord())->requests.clear();
			stack->o->scene->GetObject(stack->o->GetCoord())->RemoveFlags(ObjectBase::Flags::CanBeKilled);
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			pops(Specific, s);
			Json json;

			json["explosionTimer"] = s->explosionTimer;
			json["drawNumber"] = s->drawNumber;
			json["rmobj"] = s->rmobj;

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			pops(Specific, s);

			if (ACTION_TIMER(s->explosionTimer,
							 ExplosionEffect_032::explosionTime,
							 stack->o,
							 [&stack, &s]()->bool
			{
				return false;
			},
							 [&stack, &s]()->bool
			{
				return true;
			},
				[&stack, &s]()->bool
			{
				if (!s->rmobj && s->explosionTimer <= blastingTime)
				{
					if (stack->o->scene->GetObject(stack->o->GetCoord())->GetFlags() & ObjectBase::ExplosionType1)
					{
						stack->o->scene->BlowUpBlock(stack->o->GetCoord());
					}
					else
					{
						stack->o->scene->ObjectVirtualArrived(stack->o->GetCoord());
						stack->o->scene->ObjectPut(stack->o->GetCoord(), stack->o->GetObjectIDremain());
					}
					s->rmobj = true;
				}

				DRAW_NUMBER(s->explosionTimer,
							ExplosionEffect_032::explosionTime,
							s->drawNumber,
							stack->o, ExplosionEffect_032::Blasting);
				return true;
			},
				[&stack, &s]()->bool
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
			pops(Specific, s);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			pops(Specific, s);
			ExplosionEffect_032::Blasting[s->drawNumber].drawScaled(x, y, w, h);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			ExplosionEffect_032::Blasting[0].drawScaled(x, y, w, h);
		}
	}

	//ExplosionExpand 034
	namespace ExplosionExpand_034
	{
		const char *name = "034 - Explosion";

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
		}
		void Create(OBJECT_CREATER_PARAM)
		{
			Explosion_033::Create(OBJECT_CREATER_CALL);
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::RollOff);

			stack->o->events.timer = true;
			stack->o->events.topDraw = true;

			stack->o->requests.timer = true;

			stack->o->scene->GetObject(stack->o->GetCoord())->events.clear();
			stack->o->scene->GetObject(stack->o->GetCoord())->requests.clear();
		}
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM)
		{
			Json json;

			json["\\Explosion_033"] = Explosion_033::Print(OBJECT_PRINTER_CALL);

			return json;
		}
		void Timer(OBJECT_TIMER_PARAM)
		{
			Explosion_033::Timer(OBJECT_TIMER_CALL);
		}
		void Update(OBJECT_UPDATE_PARAM)
		{
			Explosion_033::Update(OBJECT_UPDATE_CALL);
		}
		void Drawner(OBJECT_DRAWNER_PARAM)
		{
			Explosion_033::Drawner(OBJECT_DRAWNER_CALL);
		}
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM)
		{
			Explosion_033::simpleDraw(OBJECT_SIMPLE_DRAWNER_CALL);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion | ObjectBase::RollOff);
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
			stack->o->SetFlags(ObjectBase::CanBeExplosion);
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