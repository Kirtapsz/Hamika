#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

#define FLAG(pos) std::uint64_t(1) << std::uint64_t(pos)

namespace Object
{
	struct Flags
	{
		static constexpr Type::Flags StepOn = FLAG(0); // Anything can step on this object
		static constexpr Type::Flags MurphyStepOn = FLAG(1); // Only murphy can step on this object
		static constexpr Type::Flags CanPushUp = FLAG(2); // This object can be pushed up
		static constexpr Type::Flags CanPushDown = FLAG(3); // This object can be pushed down
		static constexpr Type::Flags CanPushRight = FLAG(4); // This object can be pushed right
		static constexpr Type::Flags CanPushLeft = FLAG(5); // This object can be pushed left
		static constexpr Type::Flags CanPush = CanPushUp | CanPushDown | CanPushRight | CanPushLeft; // This object can be pushed to any direction
		static constexpr Type::Flags MurphyDies = FLAG(6); // Murphy dies if steps on this object
		static constexpr Type::Flags RollOffTop = FLAG(7); // Objects can roll down from this object
		static constexpr Type::Flags RollOffBottom = FLAG(8); // Objects can roll up from this object
		static constexpr Type::Flags RollOff = RollOffTop | RollOffBottom; // Objects can roll up and down from this object
		static constexpr Type::Flags PassageFromRight = FLAG(9); // Murphy can pass through it from the right to the left
		static constexpr Type::Flags PassageFromLeft = FLAG(10); // Murphy can pass through it from the left to the right
		static constexpr Type::Flags PassageFromTop = FLAG(11); // Murphy can pass through it from the top to the bottom
		static constexpr Type::Flags PassageFromBottom = FLAG(12); // Murphy can pass through it from the bottom to the top
		static constexpr Type::Flags PassageVertical = PassageFromTop | PassageFromBottom; // Murphy can pass through it vertically
		static constexpr Type::Flags PassageHorizontal = PassageFromLeft | PassageFromRight; // Murphy can pass through it horizontally
		static constexpr Type::Flags Passage = PassageVertical | PassageHorizontal; // Murphy can pass through it in any direction

		static constexpr Type::Flags FallOnExplosion = FLAG(18); // This object explodes if something fall onto it
		static constexpr Type::Flags ExplosionType1 = FLAG(19); // 1*1 Re-explode
		static constexpr Type::Flags ExplosionType3 = FLAG(20) | ExplosionType1; // 3*3 Re-explode
		static constexpr Type::Flags ExplosionType5 = FLAG(21) | ExplosionType3; // 5*5 Re-explode
		static constexpr Type::Flags ExplosionType = ExplosionType1 | ExplosionType3 | ExplosionType5; // Re-explode
		static constexpr Type::Flags CanBeExploded = FLAG(22); // This object can be destroyed via explosion
		static constexpr Type::Flags LimitSpeed = FLAG(23); // This opbject has a limited fix speed regardles of other speed options, like a rock movement during a push action
		static constexpr Type::Flags PhysicsSpeed = FLAG(24); // This object has a physics acceleration option

		static constexpr Type::Flags MurphyCanSuck = FLAG(26); // Murphy can suck this object, so dont need to step on it to eat it
		static constexpr Type::Flags CanBeKilled = FLAG(27); // This object can be killed (for example Murphy by other entities)
		static constexpr Type::Flags GiveGravityDelay = FLAG(28); // When eating this object, gravity is delayed so Murphy can continue to climb up to the next object
		static constexpr Type::Flags ButtonPush = FLAG(29); // This object can be pressed like the Terminal entity
		static constexpr Type::Flags Give1Score = FLAG(30); // When eating this object, this one gives 1 score
		static constexpr Type::Flags Give1Unity = FLAG(31); // When eating this object, this one give 1 unity for later use
		static constexpr Type::Flags SwapsGravity = FLAG(32); // When passing through this object, the grivity option swaps (on <-> off)

		static constexpr Type::Flags FallDownRule = FLAG(33); // It tells that the object tries to fall down if it possible, but this flag does not lead any logic
		static constexpr Type::Flags RollHorizontalRule = FLAG(34);  // It tells that the object tries to roll right or left if it possible, but this flag does not lead any logic
	};

	namespace Module
	{
		namespace Flags
		{
			struct Data
			{
				using Flags = Object::Flags;
				Type::Flags flags = 0;
			};

			template<typename MODULES_T>
			struct Func
			{
				private: typename MODULES_T::DATA_T &data_;
				private: typename MODULES_T::FUNC_T &func_;
				public: Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func);

				public: void __init__(Type::ID id, Type::Coord coord);
				public: Json print();

				public: void SetFlags(Type::Flags flags_);
				public: Type::Flags GetFlags();
				public: void AddFlags(Type::Flags flags_);
				public: void RemoveFlags(Type::Flags flags_);
			};
		}
	}
}

#include "ObjectFlags.hpp"