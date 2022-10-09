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

#include "StatusBar.h"
#include "MapDrawer.h"
#include "Object.h"
#include "ActiveBlock.h"
#include "KeyboardController.h"
#include "RngController.h"
#include "Account.h"

class ActiveMap:public KIR5::Panel, public ObjectBase::Interface
{
	private: std::shared_ptr<Account> account;

	private: LoopControllerInterface loopControllerInterface;
	private: std::shared_ptr<Logger> logger;

	private: std::shared_ptr<KeyboardController> keyboardController;
	private: std::shared_ptr<RngController> rngController;

	private: std::shared_ptr<Array2D<ActiveBlock<ObjectBase>>> map = std::shared_ptr<Array2D<ActiveBlock<ObjectBase>>>(new Array2D<ActiveBlock<ObjectBase>>());
	private: std::vector<ObjectBase *> objects;
	private: std::vector<ObjectBase *> remains;
	private: void buildObjectsHolder();

	private: MapDrawer<ActiveBlock<ObjectBase>> drawer;
	private: KIR5::EVENT<StatusBar> statusbar;
	private: KIR5::EVENT<KIR5::Panel> drawnerPanel;

	private: ObjectBase *murphy = nullptr;
	private: ObjectBase bedrock = ObjectBase(*this);
	private: ObjectBase space = ObjectBase(*this);

	private: bool enableupdateskip;
	private: bool victory;
	private: bool globalGravity;
	private: Type::CameraSize cameraSize;

	private: unsigned long long loopCounter;
	private: static constexpr unsigned long long startLoopInit = CPS * 0.5f;
	private: unsigned long long startLoop;
	private: static constexpr unsigned long long stopLoopInit = CPS * 2;
	private: unsigned long long stopLoop;

	public: ActiveMap();
	public: ~ActiveMap();

	public: void CopyObject(Type::Coord coordDst, Type::Coord coordSrc);
	public: void CopyRemain(Type::Coord coordDst, Type::Coord coordSrc);
	public: void CopyObjectToRemain(Type::Coord coordDst, Type::Coord coordSrc);
	public: void CopyRemainToObject(Type::Coord coordDst, Type::Coord coordSrc);

	public: void startMap(const BluePrint &disp_map, const std::shared_ptr<Account> &account);
	private: void stopMap();
	public: virtual void Redrawn(Type::Coord coord);

	public: void UpdateRun();

	public: void UpdateBlock(Type::Coord coord);
	public: void UpdateSquare33(Type::Coord coord);
	public: void UpdateVertical3(Type::Coord coord);
	public: void UpdateHorizontal3(Type::Coord coord);
	public: bool TestObject(Type::Coord coord) const;
	public: bool TestRemain(Type::Coord coord) const;

	public: void DeleteRemain(Type::Coord coord);
	public: void DeleteObject(Type::Coord coord);
	public: void BlowUpBlock(Type::Coord coord);
	public: void Blasting(Type::Coord coord);
	public: void StepDisappear(Type::Coord coord);
		  //public: void SetObject(Type::Coord coord, ObjectBase *object);
	public: void ExplosionPut(Type::Coord coord, Type::ID IDto);

		  /*Object interfaces*/
	public: virtual void murphyMoved(ObjectBase *object);
	public: virtual void murphyDead(ObjectBase *);
	public: virtual void murphyVictory();

	public: virtual Type::Flags GetBlockFlags(Type::Coord);
	public: virtual ObjectBase *GetObject(Type::Coord);
	public: virtual ObjectBase *GetObjectOut(Type::Coord);
	public: virtual ObjectBase *GetRemain(Type::Coord);
	public: virtual Type::Flags GetUnionFlags(Type::Coord);
	public: virtual Type::Flags GetSectionFlags(Type::Coord);
	public: virtual void ObjectMove(Type::Coord, Type::Coord, Type::ID);
	public: virtual void ObjectPut(Type::Coord, Type::ID);
	public: virtual void RemainPut(Type::Coord, Type::ID);
	public: virtual void ObjectArrived(Type::Coord);
	public: virtual void ObjectVirtualArrived(Type::Coord);
	public: virtual void ObjectDisappear(Type::Coord);
	public: virtual Type::Size GetDrawSize() const;
	public: virtual bool EnableUpdateSkip();
	public: virtual bool IsObjectOut(Type::Coord);
	public: virtual Type::Coord GetGoto(Type::Coord);
	public: virtual Type::Coord GetComefrom(Type::Coord);
	public: virtual Type::Size MapSize();
	public: virtual ObjectBase *GetObjectU(Type::Coord);
	public: virtual Type::Size GetDrawOffSet() const;
	public: virtual void AddUnity(int);
	public: virtual void AddAim(int);
	public: virtual int GetUnityCount() const;
	public: virtual int GetAimRemaining() const;
	public: virtual bool IamRemain(ObjectBase *);
	public: virtual bool IsGlobalGravity() const;
	public: virtual void switchGravity();
	public: virtual bool rollTrigger(ObjectBase *obj_, unsigned __int16 typeID, float chancePerSec);
};