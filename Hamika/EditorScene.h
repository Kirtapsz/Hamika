#pragma once

#include "Types.h"
#include "World.h"
#include "Objects.h"

#include <KIR\AL\KIR5_panel.h>
#include <KIR\KIR4_time.h>
#include <KIR\AL\KIR5_display.h>

#include <memory>

#include "StatusBar.h"
#include "SceneDrawer.h"
#include "Object.h"
#include "UItools.h"
#include "EditorSceneBlock.h"


namespace Editor::UI::Scene
{
	using namespace ::UI;

	class Edit:public Panel, public Object::SceneInterface
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

		private: std::shared_ptr<Matrix<SceneBlock<Object::Brick>>> map = std::shared_ptr<Matrix<SceneBlock<Object::Brick>>>(new Matrix<SceneBlock<Object::Brick>>());
		private: SceneDrawer_T<SceneBlock<Object::Brick>> drawer;

		public: Edit();
		public: ~Edit();

		public: void setTarget(Type::Move camera);
		public: void SetMap(std::shared_ptr<Res::BluePrint> &bluePrint);
		public: void mapLayoutUpdated();
		public: void blocksUpdated();

		public: bool isOperationModeAll() const;
		public: Type::Coord getTarget() const;

		public: virtual Type::Pixels getBlockSize() const;
		public: virtual Object::Brick *GetObject(Type::Coord);
		public: virtual Type::Flags GetBlockFlags(Type::Coord) const;
		public: virtual int selectStatus(Type::Coord) const;
		public: virtual bool isTarget(Type::Coord) const;
	};
}