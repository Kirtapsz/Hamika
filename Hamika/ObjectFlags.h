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
					CanPushUp = 1 << 2, //el lehet felfel� tolni
					CanPushDown = 1 << 3, //el lehet lefel� tolni
					CanPushRight = 1 << 4, //el lehet jobbra tolni
					CanPushLeft = 1 << 5, //el lehet balra tolni
					CanPush = CanPushUp | CanPushDown | CanPushRight | CanPushLeft, //b�rmely ir�nyba el lehet tolni
					MurphyDies = 1 << 6, // Murphy dies if step on it
					RollOffTop = 1 << 7, //a tetej�r�l legurul
					RollOffBottom = 1 << 8, //az alj�r�l legurul
					RollOff = RollOffTop | RollOffBottom, //tetej�r�l �s alj�r�l is le lehet gurulni
					PassageFromRight = 1 << 9, //�tlehet rajta b�jni jobbr�l
					PassageFromLeft = 1 << 10, //�tlehet rajta b�jni balr�l
					PassageFromTop = 1 << 11, //�tlehet rajta b�jni fentr�l
					PassageFromBottom = 1 << 12, //�tlehet rajta b�jni lentr�l
					PassageVertical = PassageFromTop | PassageFromBottom, //�tlehet rajta b�jni jobbr�l �s balr�l
					PassageHorizontal = PassageFromLeft | PassageFromRight, //�tlehet rajta b�jni fentr�l �s lentr�l
					Passage = PassageVertical | PassageHorizontal, //�tlehet rajta b�jni minden ir�nyb�l
					FallOnExplosion = 1 << 18, //ha r�esik valami felrobban
					ExplosionType1 = 1 << 19, //1*1-es �jrarobban�s
					ExplosionType3 = 1 << 20 | ExplosionType1, //3*3-as �jrarobban�s
					ExplosionType5 = 1 << 21 | ExplosionType3, //5*5-�s �jrarobban�s
					ExplosionType = ExplosionType1 | ExplosionType3 | ExplosionType5,
					CanBeExploded = 1 << 22, //fel lehet robbantani
					LimitSpeed = 1 << 23, //limited fix speed regardles of other speed oprtions, like a rock movement during a push action
					PhysicsSpeed = 1 << 24, //gyorsul� mozg�s
					MurphyCanSuck = 1 << 26, //a j�t�kos fel tudja szedni r�l�p�s n�lk�l "szippant�ssal"
					CanBeKilled = 1 << 27, //meg lehet �lni
					GiveGravityDelay = 1 << 28, //nem esik vissza azonnal
					ButtonPush = 1 << 29, //megnyomhat�, pl egy terminal, hozz� tartoz� f�ggv�ny: virtual void ButtonPushed()
					Give1Score = 1 << 30, //a c�lhoz hozz�tesz 1-et
					Give1Unity = 1 << 31, //1 bomb�t ad
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