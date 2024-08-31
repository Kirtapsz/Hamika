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
#include "SceneBackground.h"

#pragma warning(disable: 4250)

namespace UI::Scene::Module::Art
{
	class Data
	{
		protected: SceneDrawer_T<SceneBlock<Object::Brick>> drawer;
		protected: std::shared_ptr<BackgoundInterface> background;
		protected: KIR5::Shared<StatusBar> statusbar;
		protected: KIR5::Shared<Panel> drawnerBar;
		protected: Type::CameraSize cameraSize;

		protected: inline void setGlobalGravity(bool _global_gravity)
		{
			drawer.setGlobalGravity(_global_gravity);
		}
	};

	template <typename DATA>
	class Func: public virtual Object::SceneInterface, public virtual DATA, public virtual Validator::Interface
	{
		protected: void initialize()
		{
			cameraSize = _bluePrint->cameraSize;

			statusbar->SetMap(_bluePrint);

			drawer.enableLayer(SceneLayer::blocks, true);
			drawer.enableLayer(SceneLayer::erases, false);
			drawer.enableLayer(SceneLayer::refresh, false);
			drawer.enableLayer(SceneLayer::gravity, true);
			drawer.enableLayer(SceneLayer::coord, false);

			drawer.setMap(map);
			drawer.updateConfiguration({drawnerBar->width(), drawnerBar->height()}, cameraSize);
			drawer.setGlobalGravity(global_gravity_);

			std::shared_ptr<SingleSeamlessBackground> single_seamless_background = std::shared_ptr<SingleSeamlessBackground>(new SingleSeamlessBackground());
			single_seamless_background->setBitmap(Res::uielements[Res::UIElements::GameBackground]);
			background = single_seamless_background;
			background->setCameraScale(-drawer.getBlockSize().width() / 4.f);
		}

		protected: virtual void Redrawn(Type::Coord coord)
		{
			drawer.setRedrawOnBlock(coord);
		}
		public: virtual Json printStatusBar() const
		{
			Json json;
			json["unityCount"] = statusbar->getUnityCount();
			json["scoreToReach"] = statusbar->getScoreToReach();
			json["scoreToCollect"] = getScoreToCollect();
			return json;
		}

			  // OBJECT INTERFACE

		public: virtual Type::Pixels getBlockSize() const
		{
			return drawer.getBlockSize();
		}
		public: virtual void addUnity(int collect)
		{
			statusbar->addUnity(collect);
		}
		public: virtual void addScore(int collect)
		{
			statusbar->addScore(collect);
		}
		public: virtual int getUnity() const
		{
			return statusbar->getUnityCount();
		}
		public: virtual int getScoreToCollect() const
		{
			return statusbar->getScoreToCollect();
		}
	};

	namespace Mock
	{
		class Data
		{
			protected: Type::CameraSize cameraSize;
			protected: int unityCount = 0;
			protected: int scoreToReach = 0;
			protected: int scoreCount = 0;

			protected: inline void setGlobalGravity(bool _global_gravity)
			{

			}
		};

		template <typename DATA>
		class Func: public virtual Object::SceneInterface, public virtual DATA, public virtual Validator::Interface
		{
			protected: void initialize()
			{
				cameraSize = _bluePrint->cameraSize;
				scoreToReach = _bluePrint->scoreToUnlock;
			}

			protected: virtual void Redrawn(Type::Coord coord)
			{

			}

			public: virtual Json printStatusBar() const
			{
				Json json;
				json["unityCount"] = unityCount;
				json["scoreToReach"] = scoreToReach;
				json["scoreToCollect"] = getScoreToCollect();
				return json;
			}

				  // OBJECT INTERFACE

			public: virtual Type::Pixels getBlockSize() const
			{
				return {blockSizeInPixel, blockSizeInPixel};
			}
			public: virtual void addUnity(int collect)
			{
				unityCount += collect;
			}
			public: virtual void addScore(int collect)
			{
				scoreCount += collect;
			}
			public: virtual int getUnity() const
			{
				return unityCount;
			}
			public: virtual int getScoreToCollect() const
			{
				return std::max(0, scoreToReach - scoreCount);
			}
		};
	}
}

#pragma warning(default:4250)