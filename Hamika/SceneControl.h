#pragma once

#include "Types.h"
#include "World.h"
#include "Objects.h"

#include <KIR\AL\KIR5_panel.h>
#include <KIR\KIR4_time.h>
#include <KIR\AL\KIR5_display.h>
#include <KIR\AL\KIR5_event_engine.h>

#include <memory>
#include <limits>

#include "Account.h"
#include "StatusBar.h"
#include "SceneDrawer.h"
#include "Object.h"
#include "UItools.h"
#include "SceneBlock.h"
#include "KeyboardController.h"
#include "RngController.h"
#include "Validator.h"

#pragma warning(disable: 4250)

namespace UI::Scene::Module::Contol
{
	class Data
	{
		protected: KIR5::Panel::FNC_MOVED activeMoved;
		protected: KIR5::Event::FNC_KEY_DOWN activeKeyDown;
		protected: KIR5::Event::FNC_KEY_UP activeKeyUp;
		protected: KIR5::Event::FNC_TIMER activeTimer;
		protected: KIR5::Event::FNC_DRAW drawnerBarDraw;

		protected: std::shared_ptr<KeyboardController> keyboardController;
		protected: std::shared_ptr<RngController> rngController;

		protected: virtual void actionRun() = 0;
	};

	template <typename DATA>
	class Func:
		public virtual Panel,
		public virtual LoopControllerInterface,
		public virtual ObjectBase::Interface,
		public virtual DATA
	{
		protected: void initialize()
		{
			show();

			*this << statusbar;
			*this << drawnerBar;
			drawnerBar->show();

			*activeMoved = [&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
			{
				drawnerBar->move(0, 0, width(), height() - statusbar->Height());
				drawer.InitializeDrawOptions({drawnerBar->width(), drawnerBar->height()}, cameraSize);
				statusbar->Align();
			};

			*activeKeyDown = [&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
			{
				keyboardController->keyDown(key_);
				return false;
			};

			*activeKeyUp = [&](FNC_KEY_UP_PARAMS)->FNC_KEY_UP_RET
			{
				keyboardController->keyUp(key_);
				return false;
			};

			*activeTimer = [&](FNC_TIMER_PARAMS)
			{
				loopRoll();
				keyboardController->loop();
				actionRun();
				statusbar->updateLoopCounter(loopCounter);
			};

			*drawnerBarDraw = [&](FNC_DRAW_PARAMS)->FNC_DRAW_RET
			{
				if (murphy)
				{
					drawer.MoveCameraTo({murphy->GetCoord().x + murphy->GetMove().x, murphy->GetCoord().y + murphy->GetMove().y});
				}
				drawer.DrawBlocks(x_, y_);
			};

			drawnerBar->fncDraw.push_back(drawnerBarDraw);
		}
	};

	namespace Mock
	{
		class Data
		{
			protected: KIR5::Event::FNC_TIMER activeTimer;

			protected: std::shared_ptr<KeyboardController> keyboardController;
			protected: std::shared_ptr<RngController> rngController;

			protected: virtual void actionRun() = 0;
		};

		template <typename DATA>
		class Func:
			public virtual LoopControllerInterface,
			public virtual ObjectBase::Interface,
			public virtual DATA
		{
			protected: void initialize()
			{
				*activeTimer = [&](FNC_TIMER_PARAMS)
				{
					loopRoll();
					keyboardController->loop();
					actionRun();
				};
			}
		};
	}
}

#pragma warning(default:4250)