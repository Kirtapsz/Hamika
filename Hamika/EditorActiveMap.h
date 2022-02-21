#pragma once

#include "Types.h"
#include "BluePrints.h"
#include "Objects.h"

#include <KIR\AL\KIR5_panel.h>
#include <KIR\KIR4_time.h>
#include <KIR\AL\KIR5_display.h>

#include <memory>

#include "StatusBar.h"
#include "MapDrawer.h"
#include "Object.h"
#include "EditorActiveBlock.h"


namespace Editor
{
	class ActiveMap:public KIR5::Panel, public EditorObjectBase::Interface
	{
		private: Type::Coord targetCoord;

		private: bool isRShift = false;
		private: bool isRCtrl = false;

		private: Type::Coord holdCoordBegin;
		private: Type::Coord holdCoordEnd;

		private: Type::Move holdCamera;
		private: int mouseHoldx;
		private: int mouseHoldy;
		private: bool mouseSelectHold;
		private: bool mouseMoveHold;
		private: bool mouseAxe;

		private: std::shared_ptr<Array2D<ActiveBlock<EditorObjectBase>>> map = std::shared_ptr<Array2D<ActiveBlock<EditorObjectBase>>>(new Array2D<ActiveBlock<EditorObjectBase>>());
		private: MapDrawer<ActiveBlock<EditorObjectBase>> drawer;

		public: ActiveMap();
		public: ~ActiveMap();

		public: void setTarget(Type::Move camera);
		public: void SetMap(std::shared_ptr<BluePrint> &bluePrint);
		public: void mapLayoutUpdated();
		public: void blocksUpdated();

		public: bool isOperationModeAll() const;
		public: Type::Coord getTarget() const;

		public: virtual Type::Size GetDrawSize() const;
		public: virtual Type::Size GetDrawOffSet() const;
		public: virtual EditorObjectBase *GetObject(Type::Coord);
		public: virtual Type::Flags ActiveMap::GetBlockFlags(Type::Coord) const;
		public: virtual int selectStatus(Type::Coord) const;
		public: virtual bool isTarget(Type::Coord) const;
	};
}