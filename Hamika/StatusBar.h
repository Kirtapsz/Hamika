#pragma once

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_Label.h>
#include <KIR\AL\KIR5_bitmap.h>

#include "World.h"
#include "UItools.h"

namespace UI::Game
{
	class StatusBar:public Panel
	{
		private: static KIR5::Bitmap statusBMP;
		private: class Label:public KIR5::Label<>
		{
			public: Label();
		};

		private: KIR5::Shared<Label> aimLabel;
		private: KIR5::Shared<Label> unitesLabel;
		private: KIR5::Shared<Label> HHLabel;
		private: KIR5::Shared<Label> MMLabel;
		private: KIR5::Shared<Label> SSLabel;
		private: KIR5::Shared<Label> mapNameLabel;
		private: std::string ToSString(int i) const;

		public: void Restart();
		public: int AimCollect;
		public: int UnitiyCollect;
		public: int Aim;
		public: void SetAimStr();
		public: void SetUnityStr();
		public: void updateLoopCounter(unsigned long long _loopCounter);

		public: StatusBar();

		public: void SetMap(const std::shared_ptr<Res::BluePrint> &bluePrint_);
		public: int GetAimRemaining() const;
		public: int GetUnityCount() const;
		public: void AddUnity(int collect);
		public: void AddAim(int collect);
		public: int Height() const;
		public: void Align();
	};
}