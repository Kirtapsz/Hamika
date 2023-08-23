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
#include "ObjectStack.h"

#undef GetObject

#define OBJECT_SIMPLE_DRAWNER_PARAM Type::Coord::base x,Type::Coord::base y,Type::Coord::base w,Type::Coord::base h
#define OBJECT_SIMPLE_DRAWNER_CALL x, y, w, h
typedef void(*SIMPLE_DRAWNER)(OBJECT_SIMPLE_DRAWNER_PARAM);


namespace Editor
{
	namespace Object
	{
		using namespace ::Object;

		template <typename OBJECT>
		struct DataCo:
			virtual Module::Base::Data<OBJECT>,
			virtual Module::Execute::Data<OBJECT>,
			virtual Module::Move::Data<OBJECT>,
			virtual Module::Stack::Data<OBJECT>,
			virtual Module::Rotate::Data<OBJECT>,
			virtual Module::Events::Data<OBJECT>,
			virtual Module::Actions::Data<OBJECT>,
			virtual Module::Draw::Data<OBJECT>,
			virtual Module::Coord::Data<OBJECT>,
			virtual Module::Requests::Data<OBJECT>
		{
			struct Interface:
				Module::Draw::Data<OBJECT>::Interface
			{
				virtual OBJECT *GetObject(Type::Coord) = 0;
				virtual Type::Flags GetBlockFlags(Type::Coord) const = 0;
				virtual int selectStatus(Type::Coord) const = 0;
				virtual bool isTarget(Type::Coord) const = 0;
			};
			Interface *scene;
			inline void setScene(Interface *scene_)
			{
				scene = scene_;
			}
		};
		struct Brick:
			virtual DataCo<Brick>,
			virtual Module::Base::Func<DataCo<Brick>>,
			virtual Module::Execute::Func<DataCo<Brick>>,
			virtual Module::Move::Func<DataCo<Brick>>,
			virtual Module::Stack::Func<DataCo<Brick>>,
			virtual Module::Rotate::Func<DataCo<Brick>>,
			virtual Module::Actions::Func<DataCo<Brick>>,
			virtual Module::Events::Func<DataCo<Brick>>,
			virtual Module::Draw::Func<DataCo<Brick>>,
			virtual Module::Coord::Func<DataCo<Brick>>
		{

		};
	}
}

namespace Object
{
	template <typename OBJECT>
	struct DataCo:
		virtual Module::Base::Data<OBJECT>,
		virtual Module::Execute::Data<OBJECT>,
		virtual Module::Move::Data<OBJECT>,
		virtual Module::Stack::Data<OBJECT>,
		virtual Module::Rotate::Data<OBJECT>,
		virtual Module::Events::Data<OBJECT>,
		virtual Module::Draw::Data<OBJECT>,
		virtual Module::Coord::Data<OBJECT>,
		virtual Module::Requests::Data<OBJECT>,
		virtual Module::Actions::Data<OBJECT>,
		virtual Module::Flags::Data<OBJECT>
	{
		struct Interface:
			Module::Draw::Data<OBJECT>::Interface
		{
			// murphy
			virtual void murphyMoved(OBJECT *object) = 0;
			virtual void murphyDead(OBJECT *) = 0;
			virtual void murphyVictory() = 0;

			// art
			virtual void addUnity(int) = 0;
			virtual void addScore(int) = 0;
			virtual int getUnity() const = 0;
			virtual int getScoreToCollect() const = 0;

			// field
			virtual Type::Flags GetBlockFlags(Type::Coord) = 0;
			virtual OBJECT *GetObject(Type::Coord) = 0;
			virtual OBJECT *GetObjectOut(Type::Coord) = 0;
			virtual OBJECT *GetObjectOutU(Type::Coord) = 0;
			virtual OBJECT *GetRemain(Type::Coord) = 0;
			virtual Type::Flags GetUnionFlags(Type::Coord) = 0;
			virtual Type::Flags GetSectionFlags(Type::Coord) = 0;
			virtual Type::Coord GetGoto(Type::Coord) = 0;
			virtual Type::Coord GetComefrom(Type::Coord) = 0;
			virtual bool IsObjectOut(Type::Coord) = 0;
			virtual bool IsGlobalGravity() const = 0;
			virtual void switchGravity() = 0;
			virtual bool IamRemain(OBJECT *) = 0;
			virtual Type::Size MapSize() = 0;
			virtual OBJECT *GetObjectU(Type::Coord) = 0;

			// action
			virtual float getMoveProgress(OBJECT *_object) const = 0;
			virtual void blowup(OBJECT *_object) = 0;
			virtual void ObjectMove(Type::Coord, Type::Coord, Type::ID) = 0;
			virtual void ObjectPut(Type::Coord, Type::ID) = 0;
			virtual void RemainPut(Type::Coord, Type::ID) = 0;
			virtual void ObjectArrived(Type::Coord) = 0;
			virtual void ObjectVirtualArrived(Type::Coord) = 0;
			virtual void ObjectDisappear(Type::Coord) = 0;
			virtual bool rollTrigger(OBJECT *obj_, unsigned __int16 typeID, float chancePerSec) = 0;
		};
		Interface *scene;
		inline void setScene(Interface *scene_)
		{
			scene = scene_;
		}
	};
	struct Brick:
		virtual DataCo<Brick>,
		virtual Module::Base::Func<DataCo<Brick>>,
		virtual Module::Execute::Func<DataCo<Brick>>,
		virtual Module::Move::Func<DataCo<Brick>>,
		virtual Module::Stack::Func<DataCo<Brick>>,
		virtual Module::Rotate::Func<DataCo<Brick>>,
		virtual Module::Events::Func<DataCo<Brick>>,
		virtual Module::Draw::Func<DataCo<Brick>>,
		virtual Module::Coord::Func<DataCo<Brick>>,
		virtual Module::Requests::Func<DataCo<Brick>>,
		virtual Module::Actions::Func<DataCo<Brick>>,
		virtual Module::Flags::Func<DataCo<Brick>>
	{
		inline void __init__(Type::ID id, Type::Coord coord)
		{
			Module::Requests::Func<DataCo<Brick>>::__init__(id, coord);
			Module::Actions::Func<DataCo<Brick>>::__init__(id, coord);
			Module::Events::Func<DataCo<Brick>>::__init__(id, coord);

			Module::Base::Func<DataCo<Brick>>::__init__(id, coord);
			Module::Stack::Func<DataCo<Brick>>::__init__(id, coord);
			Module::Execute::Func<DataCo<Brick>>::__init__(id, coord);
			Module::Move::Func<DataCo<Brick>>::__init__(id, coord);
			Module::Rotate::Func<DataCo<Brick>>::__init__(id, coord);
			Module::Coord::Func<DataCo<Brick>>::__init__(id, coord);
			Module::Flags::Func<DataCo<Brick>>::__init__(id, coord);
			Module::Draw::Func<DataCo<Brick>>::__init__(id, coord);

			TranslationTo = ObjectID::Space;
			ObjectIDremain = ObjectID::Space;
		}
		inline Json print()
		{
			Json json;

			json["TranslationTo"] = TranslationTo;
			json["ObjectIDremain"] = ObjectIDremain;

			json["\\ObjectRequestsModule"] = Module::Requests::Func<DataCo<Brick>>::print();
			json["\\ObjectActionsModule"] = Module::Actions::Func<DataCo<Brick>>::print();
			json["\\ObjectEventsModule"] = Module::Events::Func<DataCo<Brick>>::print();

			json["\\Object_"] = Module::Base::Func<DataCo<Brick>>::print();
			json["\\ObjectStackModule"] = Module::Stack::Func<DataCo<Brick>>::print();
			json["\\ObjectEventModule"] = Module::Execute::Func<DataCo<Brick>>::print();
			json["\\ObjectMoveModule"] = Module::Move::Func<DataCo<Brick>>::print();
			json["\\ObjectRotationModule"] = Module::Rotate::Func<DataCo<Brick>>::print();
			json["\\ObjectCoordModule"] = Module::Coord::Func<DataCo<Brick>>::print();
			json["\\ObjectFlagsModule"] = Module::Flags::Func<DataCo<Brick>>::print();
			json["\\ObjectDrawModule"] = Module::Draw::Func<DataCo<Brick>>::print();

			return json;
		}


		public:

		static void PrintFlags(Type::Flags flags);
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
		inline auto Disappear()
		{
			return scene->ObjectDisappear(coord);
		}
		inline auto GetObject(Type::Coord coord)
		{
			return scene->GetObject(coord);
		}
		inline auto GetObjectOut(Type::Coord coord)
		{
			return scene->GetObjectOut(coord);
		}
		inline auto GetRemain(Type::Coord coord)
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


		virtual void SetCoord(Type::Coord coord);


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
		bool CanMovePosByRotationH(Type::Coord to, Type::Rotation rotation);
		public:
		bool CanMovePos(Type::Coord to, Type::Rotation rotation);
		bool CanMoveDown();
		bool CanMoveUp();
		bool CanMoveLeft();
		bool CanMoveRight();
	};


	//Unknown/empty object
	namespace Unknown
	{
		extern const char *name;

		void Initializer(OBJECT_INITIALIZER_PARAM);
		void Create(OBJECT_CREATER_PARAM);
		OBJECT_PRINTER_RET Print(OBJECT_PRINTER_PARAM);
		void Timer(OBJECT_TIMER_PARAM);
		void Update(OBJECT_UPDATE_PARAM);
		void Drawner(OBJECT_DRAWNER_PARAM);
		void simpleDraw(OBJECT_SIMPLE_DRAWNER_PARAM);
	}
}

#pragma warning( pop )