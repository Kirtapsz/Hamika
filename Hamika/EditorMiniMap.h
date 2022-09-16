#pragma once

#include "Types.h"
#include "World.h"
#include "Objects.h"
#include "Font.h"
#include "BlueprintPanel.h"
#include "EditorActiveBlock.h"

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_list_panel.h>
#include <KIR\KIR4_time.h>
#include <KIR\AL\KIR5_scroll_bar.h>
#include <KIR\AL\KIR5_label.h>
#include <KIR\AL\KIR5_text_box.h>
#include <KIR\AL\KIR5_button.h>

#include <memory>

namespace Editor
{
	class MiniMap: public KIR5::Panel
	{
		private: static constexpr int blockDimension = 5;

		private: bool mouseHold;

		private: Type::Move camera;
		private: Type::Move cameraSize;

		private: std::shared_ptr<Array2D<ActiveBlock<EditorObjectBase>>> map;
		private: KIR5::Bitmap bitmap;

		public: MiniMap();
		public: ~MiniMap();

		private: void setCameraToClick(int x, int y);
		public: void SetMap(std::shared_ptr<Array2D<ActiveBlock<EditorObjectBase>>> &map);
		public: void updateBlocks();
		public: void updatePosition(Type::Move center, Type::Move size);
	};
}