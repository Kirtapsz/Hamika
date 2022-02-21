#pragma once

#pragma warning( push )
#pragma warning( disable : 4250 )

#include <KIR/KIR4_time.h>
#include <KIR/KIR4_console.h>

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Global.h"
#include "Bitmap.h"
#include "BitmapPool.h"

#undef GetObject

#define OBJECT_SIMPLE_DRAWNER_PARAM Type::Coord::Type x,Type::Coord::Type y,Type::Coord::Type w,Type::Coord::Type h
#define OBJECT_SIMPLE_DRAWNER_CALL x, y, w, h
typedef void(*SIMPLE_DRAWNER)(OBJECT_SIMPLE_DRAWNER_PARAM);

/*enum ObjectEvent:Type::Flags
{
	Timer = 1 << 1, //events->timer
	Tick = 1 << 2, //events->tick
	Update = 1 << 3, //events->update
	HightLayer = 1 << 8, //events->topDraw


	Remove = 1 << 5, //requests->remove
	Blasting = 1 << 6, //requests->blowUp
	UpdateRequest = 1 << 9, //requests->update
	Draw = 1 << 4, //requests->draw

	Moving = 1 << 12, //actions->move
	Rotating = 1 << 13, //actions->rotate

	TimerDone = 1 << 10, // remove
	TickDone = 1 << 11, // remove

	UpdateEnable = UpdateRequest | Update, //remove

	DrawScreen = 1 << 14 | Draw, //remove
};*/

template<typename OBJECT_HANDLER>
struct ObjectActionsModule
{
	struct ActionsType
	{
		bool move;
		bool rotate;
	} actions = {0};

	inline void __init__(Type::ID id, Type::Coord coord)
	{
		actions.move = false;
		actions.rotate = false;
	}
};

template<typename OBJECT_HANDLER>
struct ObjectEventsModule
{
	struct EventsType
	{
		bool timer;
		bool tick;
		bool topDraw;
	} events = {0};

	inline void __init__(Type::ID id, Type::Coord coord)
	{
		events.timer = false;
		events.tick = false;
		events.topDraw = false;
	}
};


template<typename OBJECT_HANDLER>
struct ObjectBase_
{
	Type::ID id = -1;
	const char *name = nullptr;
	bool isExists = false;

	inline void __init__(Type::ID id, Type::Coord coord)
	{
		this->id = id;
		name = nullptr;
		isExists = false;
	}
	virtual void Print() = 0;
};

template<typename OBJECT_HANDLER>
struct ObjectHitActionModule:
	virtual ObjectBase_<OBJECT_HANDLER>
{
	Type::Coord hitCoord = {0,0};

	inline void __init__(Type::ID id, Type::Coord coord)
	{
		hitCoord = coord;
	}

	virtual void SetHitCoord(Type::Coord coord_)
	{
		hitCoord = coord_;
	}
	Type::Coord GetHitCoord() const
	{
		return hitCoord;
	}
};

template<typename OBJECT_HANDLER>
struct ObjectRequestsModule:
	virtual ObjectHitActionModule<OBJECT_HANDLER>
{
	struct RequestsType
	{
		bool remove;
		bool blowUp;
		bool update;
		bool draw;
	} requests = {0};

	inline void __init__(Type::ID id, Type::Coord coord)
	{
		requests.remove = false;
		requests.blowUp = false;
		requests.update = false;
		requests.draw = false;
	}

	inline void blowUp(Type::Coord coord)
	{
		requests.blowUp = true;
		hitCoord = coord;
	}
};

template<typename OBJECT_HANDLER>
struct ObjectStackModule:
	virtual ObjectBase_<OBJECT_HANDLER>
{
	//	Specific *s=(Specific*)(stack->specific); stack->specific+=sizeof(Specific);
	//#define pops(Specific) Specific *s=(Specific*)(specific); specific+=sizeof(Specific);
	////#define popsn(Specific,name) Specific *name=(Specific*)(stack->specific); stack->specific+=sizeof(Specific);
	//
	//#define getss(Specific) Specific *s=(Specific*)(stack->specific);
	//#define gets(Specific) Specific *s=(Specific*)(specific);
	//
	////#define getsn(Specific,name) Specific *name=(Specific*)(stack->specific);
	//#define maks(object) unsigned char *specific_=(unsigned char*)object->specific; unsigned char **specific=&specific_;


	struct Stack
	{
		OBJECT_HANDLER *o = nullptr;
		unsigned char *specific = nullptr;
		unsigned int pos = 0;
	};

	//OBJECT SPECIFIC
	unsigned char specific[256] = {0};

	inline void __init__(Type::ID id, Type::Coord coord)
	{
		memset(specific, 0, sizeof(specific));
	}

#define pops(Specific,name) \
unsigned int pos=stack->pos; \
stack->pos+=sizeof(Specific); \
Specific *name=(Specific*)(&stack->specific[pos]);

#define gets(Specific,name) \
unsigned int pos=stack->pos; \
Specific *name=(Specific*)(&stack->specific[stack->pos]);

#define regets(Specific,name) \
name=(Specific*)(&stack->specific[pos]); 

#define maks(object) \
ObjectBase::Stack stack_; \
ObjectBase::Stack *stack=&stack_; \
stack->o=object; \
stack->specific=object->specific; 

};

template<typename OBJECT_HANDLER>
struct ObjectEventModule:
	virtual ObjectBase_<OBJECT_HANDLER>,
	virtual ObjectRequestsModule<OBJECT_HANDLER>,
	virtual ObjectStackModule<OBJECT_HANDLER>,
	virtual ObjectHitActionModule<OBJECT_HANDLER>
{
	//Type::Flags events = 0;

	inline void __init__(Type::ID id, Type::Coord coord)
	{
		//events = 0;
		printFnc = nullptr;
		updaterFnc = nullptr;
		timerFnc = nullptr;
		tickerFnc = nullptr;
	}

	//void SetEvent(Type::Flags ev)
	//{
	//	events = ev;
	//}
	//void SetBlasingEvent(Type::Coord coord)
	//{
	//	events = ObjectEvent::Blasting;
	//	hitCoord = coord;
	//}
	//void AddEvent(Type::Flags ev)
	//{
	//	events |= ev;
	//}
	//void RemoveEvent(Type::Flags ev)
	//{
	//	events &= ~ev;
	//}
	//Type::Flags GetEvents()
	//{
	//	return events;
	//}


#define OBJECT_PRINTER_PARAM ObjectBase::Stack *stack
#define OBJECT_PRINTER_CALL stack
	typedef void(*PRINT)(Stack *);
	PRINT printFnc;
	void RunPrinter()
	{
		if (printFnc)
		{
			Stack stack;
			stack.o = dynamic_cast<OBJECT_HANDLER *>(this);
			stack.specific = this->specific;
			printFnc(&stack);
		}
	}

	//LÉTRHOZÁS #####################################################################################
#define OBJECT_CREATER_PARAM ObjectBase::Stack *stack
#define OBJECT_CREATER_CALL stack
	typedef void(*CREATER)(Stack *);
	void RunCreate(CREATER creater = NULL);

	//INICIALIZÁLÓ
#define OBJECT_INITIALIZER_PARAM
	typedef void(*INITIALIZER)(OBJECT_INITIALIZER_PARAM);

	//UPDATE #####################################################################################
#define OBJECT_UPDATE_PARAM ObjectBase::Stack *stack
#define OBJECT_UPDATE_CALL stack
	typedef void(*UPDATE)(Stack *);
	UPDATE updaterFnc;
	unsigned long long updateNumber;
	unsigned long long totalUpdateNumber;

	void RunUpdate()
	{
		requests.update = false;
		updateNumber = totalUpdateNumber++;
		if (updaterFnc)
		{
			Stack stack;
			stack.o = dynamic_cast<OBJECT_HANDLER *>(this);
			stack.specific = this->specific;
			updaterFnc(&stack);
		}
	}


	//TIMER EVENT #####################################################################################
#define OBJECT_TIMER_PARAM ObjectBase::Stack *stack
#define OBJECT_TIMER_CALL stack
	typedef void(*TIMER)(Stack *);
	TIMER timerFnc;
	void RunTimer()
	{
		if (timerFnc)
		{
			Stack stack;
			stack.o = dynamic_cast<OBJECT_HANDLER *>(this);
			stack.specific = this->specific;
			timerFnc(&stack);
		}
	}

	//TIMER EVENT #####################################################################################
#define OBJECT_TICK_PARAM ObjectBase::Stack *stack
#define OBJECT_TICK_CALL stack
	typedef void(*TICK)(Stack *);
	TICK tickerFnc;
	void RunTick()
	{
		if (tickerFnc)
		{
			Stack stack;
			stack.o = dynamic_cast<OBJECT_HANDLER *>(this);
			stack.specific = this->specific;
			tickerFnc(&stack);
		}
	}
};

template<typename OBJECT_HANDLER>
struct ObjectMoveModule:
	virtual ObjectBase_<OBJECT_HANDLER>,
	virtual ObjectActionsModule<OBJECT_HANDLER>
{
	inline void __init__(Type::ID id, Type::Coord coord)
	{
		move = {0,0};
	}

	Type::Move move = {0,0};

	void SetMoveUnsafe(Type::Move move_ = {0,0})
	{
		move = move_;
	}
	void SetMove(Type::Move move_ = {0,0})
	{
		move = move_;
		SetDrawCoord();
	}
	Type::Move GetMove()
	{
		return move;
	}
	bool IsMove()
	{
		return move.x != 0 || move.y != 0;
	}
	bool IsMoveHorizontal()
	{
		return move.x != 0;
	}
	bool IsMoveVertical()
	{
		return move.y != 0;
	}
	bool IsMoving()
	{
		return actions.move;
	}
	bool IsMoveLeft()
	{
		return move.x > 0;
	}
	bool IsMoveRight()
	{
		return move.x < 0;
	}
	bool IsMoveDown()
	{
		return move.y < 0;
	}
	bool IsMoveUp()
	{
		return move.y > 0;
	}
	/*
	beállítja autómatikusan a movet a rotation alapján

	a move paraméter adja meg a move x vagy y-t, pozitív egész számnak kell lennie
	*/
	void SetMove(Type::Rotation rotation, Type::Move move = {1,1})
	{
		if (rotation == Type::Rotations::Up)
			SetMove({this->move.x,move.y});
		else if (rotation == Type::Rotations::Down)
			SetMove({this->move.x,-move.y});
		else if (rotation == Type::Rotations::Left)
			SetMove({move.x,this->move.y});
		else if (rotation == Type::Rotations::Right)
			SetMove({-move.x,this->move.y});
	}
	void SetMoveUnsafe(Type::Rotation rotation, Type::Move move = {1,1})
	{
		if (rotation == Type::Rotations::Up)
			SetMoveUnsafe({this->move.x,move.y});
		else if (rotation == Type::Rotations::Down)
			SetMoveUnsafe({this->move.x,-move.y});
		else if (rotation == Type::Rotations::Left)
			SetMoveUnsafe({move.x,this->move.y});
		else if (rotation == Type::Rotations::Right)
			SetMoveUnsafe({-move.x,this->move.y});
	}

	virtual void SetDrawCoord() = 0;
};

template<typename OBJECT_HANDLER>
struct ObjectRotationModule:
	virtual ObjectBase_<OBJECT_HANDLER>
{
	inline void __init__(Type::ID id, Type::Coord coord)
	{
		rotation = 0;
	}

	Type::Rotation rotation = 0;

	static Type::Rotation GetRealRotation(Type::Rotation rotation)
	{
		if (rotation < 0)
			rotation += (int)(rotation / Type::Rotations::_360) * Type::Rotations::_360 + Type::Rotations::_360;
		else if (rotation > Type::Rotations::_360)
			rotation -= (int)(rotation / Type::Rotations::_360) * Type::Rotations::_360;
		return rotation;
	}
	Type::Rotation GetRotation() const
	{
		return rotation;
	}
};

template<typename OBJECT_HANDLER>
struct ObjectCoordModule:
	virtual ObjectBase_<OBJECT_HANDLER>
{
	inline void __init__(Type::ID id, Type::Coord coord)
	{
		this->coord = coord;
	}

	Type::Coord coord;

	Type::Coord GetCoord() const
	{
		return coord;
	}
	Type::Coord GetCoordLeft() const
	{
		return{coord.x - 1,coord.y};
	}
	Type::Coord GetCoordRight() const
	{
		return{coord.x + 1,coord.y};
	}
	Type::Coord GetCoordUp() const
	{
		return{coord.x,coord.y - 1};
	}
	Type::Coord GetCoordDown() const
	{
		return{coord.x,coord.y + 1};
	}
};

template<typename OBJECT_HANDLER>
struct ObjectFlagsModule:
	virtual ObjectBase_<OBJECT_HANDLER>
{
	inline void __init__(Type::ID id, Type::Coord coord)
	{
		flags = 0;
	}

	enum Flags:Type::Flags
	{
		StepOn = 1 << 0,//ráléphet, megeheti
		PlayerDies = 1 << 1,//ha megeszi belehal
		CanPushUp = 1 << 2,//el lehet felfelé tolni
		CanPushDown = 1 << 3,//el lehet lefelé tolni
		CanPushRight = 1 << 4,//el lehet jobbra tolni
		CanPushLeft = 1 << 5,//el lehet balra tolni
		CanPush = CanPushUp | CanPushDown | CanPushRight | CanPushLeft,//bármely irányba el lehet tolni
		PlayerStepOn = (1 << 6),//a player és más rá tud lépni
		RollOffTop = 1 << 7,//a tetejérõl legurul
		RollOffBottom = 1 << 8,//az aljáról legurul
		RollOff = RollOffTop | RollOffBottom,//tetejérõl és aljáról is le lehet gurulni
		PassageFromRight = 1 << 9,//átlehet rajta bújni jobbról
		PassageFromLeft = 1 << 10,//átlehet rajta bújni balról
		PassageFromTop = 1 << 11,//átlehet rajta bújni fentrõl
		PassageFromBottom = 1 << 12,//átlehet rajta bújni lentrõl
		PassageVertical = PassageFromTop | PassageFromBottom,//átlehet rajta bújni jobbról és balról
		PassageHorizontal = PassageFromLeft | PassageFromRight,//átlehet rajta bújni fentrõl és lentrõl
		Passage = PassageVertical | PassageHorizontal,//átlehet rajta bújni minden irányból
		FallOnExplosion = 1 << 18,//ha ráesik valami felrobban
		ExplosionType1 = 1 << 19,//1*1-es újrarobbanás
		ExplosionType3 = 1 << 20 | ExplosionType1,//3*3-as újrarobbanás
		ExplosionType5 = 1 << 21 | ExplosionType3,//5*5-ös újrarobbanás
		CanBeExplosion = 1 << 22,//fel lehet robbantani
		LimitSpeed = 1 << 23,//egy megadott sebességgel mozog
		PhysicsSpeed = 1 << 24,//gyorsuló mozgás
		InstantSpeed = 1 << 25,//azonnal a maximum sebességre ugrik
		PlayerCanSniff = 1 << 26,//a játékos fel tudja szedni rálépés nélkül "szippantással"
		CanBeKilled = 1 << 27,//meg lehet ölni
		GiveGravityDelay = 1 << 28,//nem esik vissza azonnal
		ButtonPush = 1 << 29,//megnyomható, pl egy terminal, hozzá tartozó függvény: virtual void ButtonPushed()
		Give1Aim = 1 << 30,//a célhoz hozzátesz 1-et
		Give1Unity = 1 << 31,//1 bombát ad
	};

	Type::Flags flags = 0;

	void SetFlags(Type::Flags flags_)
	{
		flags = flags_;
	}
	Type::Flags GetFlags()
	{
		return flags;
	}
	void AddFlags(Type::Flags flags_)
	{
		flags |= flags_;
	}
	void RemoveFlags(Type::Flags flags_)
	{
		flags &= ~flags_;
	}
};

template<typename OBJECT_HANDLER>
struct ObjectDrawModule:
	virtual ObjectRequestsModule<OBJECT_HANDLER>,
	virtual ObjectActionsModule<OBJECT_HANDLER>,
	virtual ObjectEventsModule<OBJECT_HANDLER>,
	virtual ObjectBase_<OBJECT_HANDLER>,
	virtual ObjectHitActionModule<OBJECT_HANDLER>,
	virtual ObjectStackModule<OBJECT_HANDLER>,
	virtual ObjectEventModule<OBJECT_HANDLER>,
	virtual ObjectMoveModule<OBJECT_HANDLER>,
	virtual ObjectRotationModule<OBJECT_HANDLER>,
	virtual ObjectCoordModule<OBJECT_HANDLER>,
	virtual ObjectFlagsModule<OBJECT_HANDLER>
{
	inline void __init__(Type::ID id, Type::Coord coord)
	{
		DrawCoord.x = coord.x * DrawSize().width;
		DrawCoord.y = coord.y * DrawSize().height;

		drawnerFnc = nullptr;
	}

	unsigned long long int DrawNumber = 0;
	static unsigned long long int SObjectDrawCounts;
	unsigned long long int DrawnedCount = 0;
	typedef void(*StatisDrawType)(int, int, int, int);

	struct Interface
	{
		virtual Type::Size GetDrawSize() const = 0;
		virtual Type::Size GetDrawOffSet() const = 0;
	} *drawIef;

	Type::Coord DrawCoord = {0,0};

	virtual void Draw()
	{
		if (drawnerFnc)
		{
			Stack stack;
			stack.o = dynamic_cast<OBJECT_HANDLER *>(this);
			stack.specific = this->specific;
			drawnerFnc(&stack, this->GetDrawCoord().x, this->GetDrawCoord().y, DrawSize().width, DrawSize().height);
		}
		else
		{
			ObjectBase::unknownBmp.drawScaled(this->GetDrawCoord().x, this->GetDrawCoord().y, DrawSize().width, DrawSize().height);
		}
	}
	virtual void SDraw()//teszteés céljából hogy a conter mûködjön
	{
		DrawnedCount++;
		if (DrawnedCount >= 2)
		{
			clog << "WARNING! An object has been drawned two times (" << GetCoord().x << "," << GetCoord().y << "):" << KIR4::eol;
			Print();
		}
		SObjectDrawCounts++;
		Draw();
	}
	void SetDrawCoord()
	{
		Type::Coord
			NewDrawCoord = {Type::Coord::Type((coord.x + move.x) * Type::Move::Type(DrawSize().width)), Type::Coord::Type((coord.y + move.y) * Type::Move::Type(DrawSize().height))};

		if (NewDrawCoord != DrawCoord)
		{
			DrawCoord = NewDrawCoord;
			requests.draw = true;
			//ief.Redrawn(coord);
		}
	}
	Type::Coord GetDrawCoord()
	{
		return{DrawCoord.x - DrawOffSet().width,DrawCoord.y - DrawOffSet().height};
	}

	inline auto DrawSize()
	{
		return drawIef->GetDrawSize();
	}
	inline auto DrawOffSet()
	{
		return drawIef->GetDrawOffSet();
	}

	inline ObjectDrawModule(Interface *drawIef):
		drawIef(drawIef)
	{

	}

	//DRAW #####################################################################################
#define OBJECT_DRAWNER_PARAM ObjectBase::Stack *stack, Type::Coord::Type x,Type::Coord::Type y,Type::Coord::Type w,Type::Coord::Type h
#define OBJECT_DRAWNER_CALL stack, x, y, w, h
	typedef void(*DRAWNER)(Stack *, Type::Coord::Type x, Type::Coord::Type y, Type::Coord::Type w, Type::Coord::Type h);
	unsigned long long int drawCounter = 0;//ez az objektum hányszor lett eddig újrarajzolva
	unsigned long long int drawAtOnceCounter = 0;//ez az objektum hányszor lett újrarajzolva egy ciklus alatt
	unsigned long long int drawCounterID = 0;//az objektum hányadikként lett rajzolva az összesbõl
	static unsigned long long int totalDrawCounter;//az összes objektum eddigi újrarajzolása
	DRAWNER drawnerFnc;
	void RunSDraw()//teszteés céljából hogy a conter mûködjön
	{
		drawCounter++;
		drawAtOnceCounter++;
		if (drawAtOnceCounter >= 2)
		{
			clog << "WARNING! An object has been drawned two times (" << GetCoord().x << "," << GetCoord().y << "):" << KIR4::eol;
			Print();
		}
		drawCounterID = ++totalDrawCounter;
		Stack stack;
		stack.o = this;
		stack.specific = this->specific;
		drawnerFnc(&stack, this->GetDrawCoord().x, this->GetDrawCoord().y, DrawSize().width, DrawSize().height);
	}

};

template<typename OBJECT_HANDLER>
unsigned long long int ObjectDrawModule<OBJECT_HANDLER>::SObjectDrawCounts = 0;

template<typename OBJECT_HANDLER>
unsigned long long int ObjectDrawModule<OBJECT_HANDLER>::totalDrawCounter = 0;

struct EditorObjectBase:
	virtual ObjectDrawModule<EditorObjectBase>
{
	struct Interface:
		ObjectDrawModule<EditorObjectBase>::Interface
	{
		virtual EditorObjectBase *GetObject(Type::Coord) = 0;
		virtual Type::Flags GetBlockFlags(Type::Coord) const = 0;
		virtual int selectStatus(Type::Coord) const = 0;
		virtual bool isTarget(Type::Coord) const = 0;
	};
	Interface &ief;

	inline EditorObjectBase(Interface &ief):
		ief(ief),
		ObjectDrawModule(&ief)
	{

	}

	inline void Print()
	{

	}
};

struct ObjectBase:
	virtual ObjectRequestsModule<ObjectBase>,
	virtual ObjectActionsModule<ObjectBase>,
	virtual ObjectEventsModule<ObjectBase>,
	virtual ObjectBase_<ObjectBase>,
	virtual ObjectHitActionModule<ObjectBase>,
	virtual ObjectStackModule<ObjectBase>,
	virtual ObjectEventModule<ObjectBase>,
	virtual ObjectMoveModule<ObjectBase>,
	virtual ObjectRotationModule<ObjectBase>,
	virtual ObjectCoordModule<ObjectBase>,
	virtual ObjectFlagsModule<ObjectBase>,
	virtual ObjectDrawModule<ObjectBase>
{
	inline void __init__(Type::ID id, Type::Coord coord)
	{
		ObjectRequestsModule<ObjectBase>::__init__(id, coord);
		ObjectActionsModule<ObjectBase>::__init__(id, coord);
		ObjectEventsModule<ObjectBase>::__init__(id, coord);

		ObjectBase_<ObjectBase>::__init__(id, coord);
		ObjectHitActionModule<ObjectBase>::__init__(id, coord);
		ObjectStackModule<ObjectBase>::__init__(id, coord);
		ObjectEventModule<ObjectBase>::__init__(id, coord);
		ObjectMoveModule<ObjectBase>::__init__(id, coord);
		ObjectRotationModule<ObjectBase>::__init__(id, coord);
		ObjectCoordModule<ObjectBase>::__init__(id, coord);
		ObjectFlagsModule<ObjectBase>::__init__(id, coord);
		ObjectDrawModule<ObjectBase>::__init__(id, coord);
	}

	static BitmapPool bitmapPool;
	static KIR5::SubBitmap unknownBmp;

	public:

	static void PrintFlags(Type::Flags flags);
	unsigned long long int UpdateNumber = 0;
	static unsigned long long int SUpdateNumber;
	public:
	typedef void(*InitializeType)();
	struct Interface:
		ObjectDrawModule<ObjectBase>::Interface
	{
		virtual void playerMoved(ObjectBase *object) = 0;
		virtual void playerDead(ObjectBase *) = 0;
		virtual void playerVictory() = 0;

		virtual Type::Flags GetBlockFlags(Type::Coord) = 0;
		virtual ObjectBase *GetObject(Type::Coord) = 0;
		virtual ObjectBase *GetObjectOut(Type::Coord) = 0;
		virtual ObjectBase *GetRemain(Type::Coord) = 0;
		virtual Type::Flags GetUnionFlags(Type::Coord) = 0;
		virtual Type::Flags GetSectionFlags(Type::Coord) = 0;
		virtual void ObjectMove(Stack *stack, Type::Coord, Type::Coord, Type::ID) = 0;
		virtual void ObjectPut(Type::Coord, Type::ID) = 0;
		virtual void RemainPut(Type::Coord, Type::ID) = 0;
		virtual void ObjectArrived(Type::Coord) = 0;
		virtual void ObjectVirtualArrived(Type::Coord) = 0;
		virtual void ObjectDisappear(Type::Coord) = 0;
		virtual bool EnableUpdateSkip() = 0;
		virtual bool IsObjectOut(Type::Coord) = 0;
		virtual Type::Coord GetGoto(Type::Coord) = 0;
		virtual Type::Coord GetComefrom(Type::Coord) = 0;
		virtual Type::Size MapSize() = 0;
		virtual ObjectBase *GetObjectU(Type::Coord) = 0;
		virtual void AddUnity(int) = 0;
		virtual void AddAim(int) = 0;
		virtual int GetUnityCount() const = 0;
		virtual int GetAimRemaining() const = 0;
		virtual bool IamRemain(ObjectBase *) = 0;

		virtual bool rollTrigger(ObjectBase *obj_, float chancePerSec) = 0;
	};
	//ief easy
	inline auto ObjectBase::Move(Stack *stack, Type::Coord from, Type::Coord to, Type::ID remain)
	{
		return ief.ObjectMove(stack, from, to, remain);
	}
	inline auto ObjectBase::Arrived()
	{
		return ief.ObjectArrived(coord);
	}
	inline auto ObjectBase::Disappear()
	{
		return ief.ObjectDisappear(coord);
	}
	inline auto ObjectBase::GetObject(Type::Coord coord)
	{
		return ief.GetObject(coord);
	}
	inline auto ObjectBase::GetObjectOut(Type::Coord coord)
	{
		return ief.GetObjectOut(coord);
	}
	inline auto ObjectBase::GetRemain(Type::Coord coord)
	{
		return ief.GetRemain(coord);
	}
	public:
	Type::Move MoveSpeed = {1,1};
	Type::Speed RotationSpeed = 1;
	Type::ID TranslationTo = ObjectID::Space;
	Type::ID ObjectIDremain = ObjectID::Space;


	public:
	Interface &ief;
	ObjectBase(Interface &ief);
	virtual ~ObjectBase();
	bool Roll(double PpM);
	public:
	public:
	/*
	csakis külsõ hívás szabad! rekurzív nem

	visszatérési érték, sikeres lefutás true
	*/

	virtual void Update();
	//autómatikusan a megfelelõ irányba lépteti, true visszatérés ha megérkezett
	//akkor kell meghívni amikor a fordást el akarjuk kezdeni, még a léptetés elött
	void StartRotation();
	bool RotationLeft();
	bool RotationRight();

	//a mértékegység hogy 1 másodperc alatt mennyit haladjon, 1 jelent egy teljes négyzetet, 2.5: két és fél négyzet másodpercenként....
	void SetMoveSpeed(Type::Move::Type speed);
	//a mértékegység hogy 1 másodperc alatt mennyit haladjon, 1 jelent egy teljes négyzetet, 2.5: két és fél négyzet másodpercenként....
	void SetMoveSpeed(Type::Move speed);
	//a mértékegység hogy 1 másodperc alatt mennyit haladjon, 360 egy teljes körbefordulás 1másodperc alatt...
	void SetRotationSpeed(Type::Speed speed);
	void SetTranslationID(Type::ID id);
	void SetObjectIDremain(Type::ID id);


	virtual void SetCoord(Type::Coord coord);


	void SetRotation(Type::Rotation rotation);

	//GET
	//1 másik oldalon még teljesen  0 nyugalmi állapot
	float GetAbsMove();
	bool IsRotating();
	Type::Coord GetForwardCoord();
	Type::Coord GetForwardCoord(Type::Rotation rotation);
	Type::Rotation GetRoundRotation(Type::Rotation rotation);

	Type::Move GetMoveSpeed();
	Type::Move::Type GetMoveSpeedVertical();
	Type::Move::Type GetMoveSpeedHorizontal();
	Type::Speed GetRotationSpeed();
	Type::ID GetTranslationTo();
	Type::ID GetObjectIDremain();
	Type::Rotation GetAngel();
	virtual void Print();
	static void Initialize();



	//AUTOMOVE
	private:
	Type::Move::Type currentspeed = 0;
	Type::Move::Type limitspeed;
	void IncreaseSpeed(Type::Move::Type max);
	bool CanMovePosByRotationH(Type::Coord to, Type::Rotation rotation);
	public:
	bool CanMovePos(Type::Coord to, Type::Rotation rotation);
	bool CanMoveDown();
	bool CanMoveUp();
	bool CanMoveLeft();
	bool CanMoveRight();



	//akkor kell meghívni ha nicns további lépés, azaz megáll az object
	void StopStep();
	void DisablePhysics();
	void EnablePhysics();
	void EnebleLimitSpeed(Type::Move::Type max);
	void DisableLimitSpeed();

	void StepUp();
	void StepDown();
	void StepLeft();
	void StepRight();
	void Step();

	//nem érkezett meg, hanme utolérte az elötte lévõt
	bool hitactive = false;
	virtual void AutoStepHit();
	virtual void ObjectArrived();
	//AUTOMOVE END


};

#pragma warning( pop )