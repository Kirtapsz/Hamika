#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

namespace Object
{
	namespace Module
	{
		namespace Flags
		{
			template <typename OBJECT>
			struct Data
			{
				enum Flags:Type::Flags
				{
					StepOn = 1 << 0, // Enything can step on this field
					MurphyStepOn = 1 << 1, // Only murphy can step on this field
					CanPushUp = 1 << 2, //el lehet felfelé tolni
					CanPushDown = 1 << 3, //el lehet lefelé tolni
					CanPushRight = 1 << 4, //el lehet jobbra tolni
					CanPushLeft = 1 << 5, //el lehet balra tolni
					CanPush = CanPushUp | CanPushDown | CanPushRight | CanPushLeft, //bármely irányba el lehet tolni
					MurphyDies = 1 << 6, // Murphy dies if step on it
					RollOffTop = 1 << 7, //a tetejérõl legurul
					RollOffBottom = 1 << 8, //az aljáról legurul
					RollOff = RollOffTop | RollOffBottom, //tetejérõl és aljáról is le lehet gurulni
					PassageFromRight = 1 << 9, //átlehet rajta bújni jobbról
					PassageFromLeft = 1 << 10, //átlehet rajta bújni balról
					PassageFromTop = 1 << 11, //átlehet rajta bújni fentrõl
					PassageFromBottom = 1 << 12, //átlehet rajta bújni lentrõl
					PassageVertical = PassageFromTop | PassageFromBottom, //átlehet rajta bújni jobbról és balról
					PassageHorizontal = PassageFromLeft | PassageFromRight, //átlehet rajta bújni fentrõl és lentrõl
					Passage = PassageVertical | PassageHorizontal, //átlehet rajta bújni minden irányból
					FallOnExplosion = 1 << 18, //ha ráesik valami felrobban
					ExplosionType1 = 1 << 19, //1*1-es újrarobbanás
					ExplosionType3 = 1 << 20 | ExplosionType1, //3*3-as újrarobbanás
					ExplosionType5 = 1 << 21 | ExplosionType3, //5*5-ös újrarobbanás
					ExplosionType = ExplosionType1 | ExplosionType3 | ExplosionType5,
					CanBeExploded = 1 << 22, //fel lehet robbantani
					LimitSpeed = 1 << 23, //limited fix speed regardles of other speed oprtions, like a rock movement during a push action
					PhysicsSpeed = 1 << 24, //gyorsuló mozgás
					MurphyCanSuck = 1 << 26, //a játékos fel tudja szedni rálépés nélkül "szippantással"
					CanBeKilled = 1 << 27, //meg lehet ölni
					GiveGravityDelay = 1 << 28, //nem esik vissza azonnal
					ButtonPush = 1 << 29, //megnyomható, pl egy terminal, hozzá tartozó függvény: virtual void ButtonPushed()
					Give1Score = 1 << 30, //a célhoz hozzátesz 1-et
					Give1Unity = 1 << 31, //1 bombát ad
					SwapsGravity = 1ll << 32ll, // when passing 
				};

				Type::Flags flags = 0;
			};
			template <typename DATA>
			struct Func: virtual DATA
			{
				void __init__(Type::ID id, Type::Coord coord);
				Json print();

				void SetFlags(Type::Flags flags_);
				Type::Flags GetFlags();
				void AddFlags(Type::Flags flags_);
				void RemoveFlags(Type::Flags flags_);
			};
		}
	}
}

#include "ObjectFlags.hpp"