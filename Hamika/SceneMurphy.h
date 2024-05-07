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

namespace UI::Scene::Module::Murphy
{
	class Data
	{
		protected: std::shared_ptr<Res::Account> _account;
		protected: Object::Brick *murphy = nullptr;
		protected: bool victory = false;

		protected: virtual void signGame() = 0;
	};

	template <typename DATA>
	class Func: public virtual Object::Brick::Interface, public virtual DATA, public virtual Validator::Interface
	{
		protected: void initialize(const std::shared_ptr<Res::Account> &account_)
		{
			_account = account_;

			std::vector<Type::Coord> spawns;
			map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
			{
				if (block.grid & GridFlags::SpawnPoint)
				{
					spawns.push_back(coord);
				}
			});

			Type::Coord spawn = spawns[rand() % spawns.size()];
			ObjectCreate(reach(map)[spawn].object, ObjectID::Murphy, spawn);
			murphy = reach(map)[spawn].object;
			Object::Entity::Murphy::SetController(murphy, keyboardController.get());
		}
		public: virtual Json printResult() const
		{
			Json json;
			json["victory"] = victory;
			return json;
		}
		public: void terminateScene()
		{
			murphyDead(murphy);
		}

		public: virtual void murphyMoved(Object::Brick *object)
		{
			if (murphy)
			{
				murphy = object;
			}
		}
		public: virtual void murphyDead(Object::Brick *omurphy)
		{
			if (murphy)
			{
				signGame();
				murphy = nullptr;
			}
		}
		public: virtual void murphyVictory()
		{
			if (murphy)
			{
				victory = true;
				signGame();
				murphy = nullptr;
			}
		}
	};
}

#pragma warning(default:4250)