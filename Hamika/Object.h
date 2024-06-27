#pragma once

#pragma warning( push )
#pragma warning( disable : 4250 )

#include <KIR/KIR4_time.h>
#include <KIR/KIR4_console.h>

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"
#include "Bitmap.h"

#include "ObjectActions.h"
#include "ObjectBase.h"
#include "ObjectCoord.h"
#include "ObjectDraw.h"
#include "ObjectEvents.h"
#include "ObjectExecute.h"
#include "ObjectFlags.h"
#include "ObjectMove.h"
#include "ObjectRequests.h"
#include "ObjectRotate.h"
#include "ObjectEntityData.h"

#undef GetObject

namespace Editor
{
	namespace Object
	{
		using namespace ::Object;

		struct Data;
		struct Brick;
		struct SceneInterface;
		struct Modules;

		struct Modules
		{
			using BRICK_T = Brick;
			using DATA_T = Data;
			using FUNC_T = BRICK_T;
			using SCENE_INTERFACE_T = SceneInterface;

			using BASE_DATA_T = Module::Base::Data<Modules>;
			using EXECUTE_DATA_T = Module::Execute::Data<Modules>;
			using MOVE_DATA_T = Module::Move::Data;
			using ENTITY_DATA_DATA_T = Module::EntityData::Data<Modules>;
			using ROTATE_DATA_T = Module::Rotate::Data;
			using EVENTS_DATA_T = Module::Events::Data;
			using ACTIONS_DATA_T = Module::Actions::Data;
			using DRAW_DATA_T = Module::Draw::Data<Modules>;
			using COORD_DATA_T = Module::Coord::Data;
			using REQUESTS_DATA_T = Module::Requests::Data;
			using FLAGS_DATA_T = Module::Flags::Data;

			using BASE_FUNC_T = Module::Base::Func<Modules>;
			using EXECUTE_FUNC_T = Module::Execute::Func<Modules>;
			using MOVE_FUNC_T = Module::Move::Func<Modules>;
			using ENTITY_DATA_FUNC_T = Module::EntityData::Func<Modules>;
			using ROTATE_FUNC_T = Module::Rotate::Func<Modules>;
			using EVENTS_FUNC_T = Module::Events::Func<Modules>;
			using ACTIONS_FUNC_T = Module::Actions::Func<Modules>;
			using DRAW_FUNC_T = Module::Draw::Func<Modules>;
			using COORD_FUNC_T = Module::Coord::Func<Modules>;
		};

		struct SceneInterface:
			Modules::DRAW_DATA_T::Interface
		{
			virtual Brick *GetObject(Type::Coord) = 0;
			virtual Type::Flags GetBlockFlags(Type::Coord) const = 0;
			virtual int selectStatus(Type::Coord) const = 0;
			virtual bool isTarget(Type::Coord) const = 0;
		};
		struct Data:
			Modules::BASE_DATA_T,
			Modules::EXECUTE_DATA_T,
			Modules::MOVE_DATA_T,
			Modules::ENTITY_DATA_DATA_T,
			Modules::ROTATE_DATA_T,
			Modules::EVENTS_DATA_T,
			Modules::ACTIONS_DATA_T,
			Modules::DRAW_DATA_T,
			Modules::COORD_DATA_T,
			Modules::REQUESTS_DATA_T,
			Modules::FLAGS_DATA_T
		{
		};
		struct Brick:
			Data,
			Modules::BASE_FUNC_T,
			Modules::EXECUTE_FUNC_T,
			Modules::MOVE_FUNC_T,
			Modules::ENTITY_DATA_FUNC_T,
			Modules::ROTATE_FUNC_T,
			Modules::EVENTS_FUNC_T,
			Modules::ACTIONS_FUNC_T,
			Modules::DRAW_FUNC_T,
			Modules::COORD_FUNC_T
		{
			inline Brick():
				Modules::BASE_FUNC_T(*this, *this),
				Modules::EXECUTE_FUNC_T(*this, *this),
				Modules::MOVE_FUNC_T(*this, *this),
				Modules::ENTITY_DATA_FUNC_T(*this, *this),
				Modules::ROTATE_FUNC_T(*this, *this),
				Modules::EVENTS_FUNC_T(*this, *this),
				Modules::ACTIONS_FUNC_T(*this, *this),
				Modules::DRAW_FUNC_T(*this, *this),
				Modules::COORD_FUNC_T(*this, *this)
			{

			}
			Brick(const Brick &brick) = delete;
			Brick(const Brick &&brick) = delete;
		};
	}
}

namespace Object
{
	struct Data;
	struct Brick;
	struct SceneInterface;
	struct Modules;

	struct Modules
	{
		using DATA_T = Data;
		using BRICK_T = Brick;
		using FUNC_T = BRICK_T;
		using SCENE_INTERFACE_T = SceneInterface;

		using BASE_DATA_T = Module::Base::Data<Modules>;
		using EXECUTE_DATA_T = Module::Execute::Data<Modules>;
		using MOVE_DATA_T = Module::Move::Data;
		using ENTITY_DATA_DATA_T = Module::EntityData::Data<Modules>;
		using ROTATE_DATA_T = Module::Rotate::Data;
		using EVENTS_DATA_T = Module::Events::Data;
		using ACTIONS_DATA_T = Module::Actions::Data;
		using DRAW_DATA_T = Module::Draw::Data<Modules>;
		using COORD_DATA_T = Module::Coord::Data;
		using REQUESTS_DATA_T = Module::Requests::Data;
		using FLAGS_DATA_T = Module::Flags::Data;

		using BASE_FUNC_T = Module::Base::Func<Modules>;
		using EXECUTE_FUNC_T = Module::Execute::Func<Modules>;
		using MOVE_FUNC_T = Module::Move::Func<Modules>;
		using ENTITY_DATA_FUNC_T = Module::EntityData::Func<Modules>;
		using ROTATE_FUNC_T = Module::Rotate::Func<Modules>;
		using EVENTS_FUNC_T = Module::Events::Func<Modules>;
		using ACTIONS_FUNC_T = Module::Actions::Func<Modules>;
		using DRAW_FUNC_T = Module::Draw::Func<Modules>;
		using COORD_FUNC_T = Module::Coord::Func<Modules>;
		using REQUESTS_FUNC_T = Module::Requests::Func<Modules>;
		using FLAGS_FUNC_T = Module::Flags::Func<Modules>;
	};


	struct SceneInterface:
		Modules::DRAW_DATA_T::Interface
	{
		// murphy
		virtual void murphyMoved(Brick *object) = 0;
		virtual void murphyDead(Brick *) = 0;
		virtual void murphyVictory() = 0;

		// art
		virtual void addUnity(int) = 0;
		virtual void addScore(int) = 0;
		virtual int getUnity() const = 0;
		virtual int getScoreToCollect() const = 0;

		// field
		virtual Type::Flags GetBlockFlags(Type::Coord) = 0;
		virtual Brick &GetObject(Type::Coord) = 0;
		virtual Brick &GetObjectOut(Type::Coord) = 0;
		virtual Brick *GetObjectOutU(Type::Coord) = 0;
		virtual Brick &GetRemain(Type::Coord) = 0;
		virtual Type::Flags GetUnionFlags(Type::Coord) = 0;
		virtual Type::Flags GetSectionFlags(Type::Coord) = 0;
		virtual Type::Coord GetGoto(Type::Coord) = 0;
		virtual Type::Coord GetComefrom(Type::Coord) = 0;
		virtual bool IsObjectOut(Type::Coord) = 0;
		virtual bool IsGlobalGravity() const = 0;
		virtual void switchGravity() = 0;
		virtual bool IamRemain(const Brick &_object) = 0;
		virtual Type::Size MapSize() = 0;
		virtual Brick *GetObjectU(Type::Coord) = 0;

		// action
		virtual float getMoveProgress(Brick &_object) const = 0;
		virtual void blowup(Brick &_object, Type::Coord center = Type::Coord::Invalid) = 0;
		virtual void ObjectMove(Type::Coord, Type::Coord, Type::ID) = 0;
		virtual void ObjectPut(Type::Coord, Type::ID) = 0;
		virtual void RemainPut(Type::Coord, Type::ID) = 0;
		virtual void ObjectArrived(Type::Coord) = 0;
		virtual void ObjectVirtualArrived(Type::Coord) = 0;
		virtual void ObjectDisappear(Type::Coord) = 0;
		virtual bool rollTrigger(Brick *obj_, unsigned __int16 typeID, float chancePerSec) = 0;
	};
	struct Data:
		Modules::BASE_DATA_T,
		Modules::EXECUTE_DATA_T,
		Modules::MOVE_DATA_T,
		Modules::ENTITY_DATA_DATA_T,
		Modules::ROTATE_DATA_T,
		Modules::EVENTS_DATA_T,
		Modules::ACTIONS_DATA_T,
		Modules::DRAW_DATA_T,
		Modules::COORD_DATA_T,
		Modules::REQUESTS_DATA_T,
		Modules::FLAGS_DATA_T
	{
	};
	struct Brick:
		Data,
		Modules::BASE_FUNC_T,
		Modules::EXECUTE_FUNC_T,
		Modules::MOVE_FUNC_T,
		Modules::ENTITY_DATA_FUNC_T,
		Modules::ROTATE_FUNC_T,
		Modules::EVENTS_FUNC_T,
		Modules::ACTIONS_FUNC_T,
		Modules::DRAW_FUNC_T,
		Modules::COORD_FUNC_T,
		Modules::REQUESTS_FUNC_T,
		Modules::FLAGS_FUNC_T
	{
		inline Brick():
			Modules::BASE_FUNC_T(*this, *this),
			Modules::EXECUTE_FUNC_T(*this, *this),
			Modules::MOVE_FUNC_T(*this, *this),
			Modules::ENTITY_DATA_FUNC_T(*this, *this),
			Modules::ROTATE_FUNC_T(*this, *this),
			Modules::EVENTS_FUNC_T(*this, *this),
			Modules::ACTIONS_FUNC_T(*this, *this),
			Modules::DRAW_FUNC_T(*this, *this),
			Modules::COORD_FUNC_T(*this, *this),
			Modules::REQUESTS_FUNC_T(*this, *this),
			Modules::FLAGS_FUNC_T(*this, *this)
		{
		}
		Brick(const Brick &brick) = delete;
		Brick(const Brick &&brick) = delete;

		inline void __init__(Type::ID id, Type::Coord coord)
		{
			Modules::REQUESTS_FUNC_T::__init__(id, coord);
			Modules::ACTIONS_FUNC_T::__init__(id, coord);
			Modules::EVENTS_FUNC_T::__init__(id, coord);

			Modules::BASE_FUNC_T::__init__(id, coord);
			Modules::ENTITY_DATA_FUNC_T::__init__(id, coord);
			Modules::EXECUTE_FUNC_T::__init__(id, coord);
			Modules::MOVE_FUNC_T::__init__(id, coord);
			Modules::ROTATE_FUNC_T::__init__(id, coord);
			Modules::COORD_FUNC_T::__init__(id, coord);
			Modules::FLAGS_FUNC_T::__init__(id, coord);
			Modules::DRAW_FUNC_T::__init__(id, coord);

			TranslationTo = ObjectID::Space;
			ObjectIDremain = ObjectID::Space;
		}
		inline Json print()
		{
			Json json;

			json["TranslationTo"] = TranslationTo;
			json["ObjectIDremain"] = ObjectIDremain;

			json["\\ObjectRequestsModule"] = Modules::REQUESTS_FUNC_T::print();
			json["\\ObjectActionsModule"] = Modules::ACTIONS_FUNC_T::print();
			json["\\ObjectEventsModule"] = Modules::EVENTS_FUNC_T::print();

			json["\\Object_"] = Modules::BASE_FUNC_T::print();
			json["\\ObjectEntityDataModule"] = Modules::ENTITY_DATA_FUNC_T::print();
			json["\\ObjectEventModule"] = Modules::EXECUTE_FUNC_T::print();
			json["\\ObjectMoveModule"] = Modules::MOVE_FUNC_T::print();
			json["\\ObjectRotationModule"] = Modules::ROTATE_FUNC_T::print();
			json["\\ObjectCoordModule"] = Modules::COORD_FUNC_T::print();
			json["\\ObjectFlagsModule"] = Modules::FLAGS_FUNC_T::Func::print();
			json["\\ObjectDrawModule"] = Modules::DRAW_FUNC_T::print();

			return json;
		}

		public:
		typedef void(*InitializeType)();
		//scene easy
		inline auto Move(Type::Coord from, Type::Coord to, Type::ID remain)
		{
			return scene->ObjectMove(from, to, remain);
		}
		inline auto Arrived()
		{
			return scene->ObjectArrived(coord);
		}
		inline auto &GetObject(Type::Coord coord)
		{
			return scene->GetObject(coord);
		}
		inline auto &GetObjectOut(Type::Coord coord)
		{
			return scene->GetObjectOut(coord);
		}
		inline auto &GetRemain(Type::Coord coord)
		{
			return scene->GetRemain(coord);
		}


		public:
		Type::ID TranslationTo = ObjectID::Space; // when its blow up
		Type::ID ObjectIDremain = ObjectID::Space; // when it disappear


		public:

		bool Roll(double PpM);
		public:
		public:
		/*
		csakis külsõ hívás szabad! rekurzív nem

		visszatérési érték, sikeres lefutás true
		*/


		void SetTranslationID(Type::ID id);
		void SetObjectIDremain(Type::ID id);


		void SetCoord(Type::Coord coord);


		//GET
		//1 másik oldalon még teljesen  0 nyugalmi állapot
		float GetAbsMove();
		Type::Coord GetForwardCoord();
		Type::Coord GetForwardCoord(Type::Rotation rotation);

		Type::ID GetTranslationTo();
		Type::ID GetObjectIDremain();
		static void Initialize();



		//AUTOMOVE
		private:
		bool CanMovePosByRotationH(Type::Coord to, Type::Rotation rotation) const;
		public:
		bool CanMovePos(Type::Coord to, Type::Rotation rotation);
		bool CanMoveDown();
		bool CanMoveUp();
		bool CanMoveLeft();
		bool CanMoveRight();
	};
}

#pragma warning( pop )