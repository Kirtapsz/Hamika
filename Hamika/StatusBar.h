#pragma once

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_Label.h>
#include <KIR\AL\KIR5_bitmap.h>

#include "World.h"
#include "UItools.h"

namespace UI::Scene
{
	class StatusBar:public Panel
	{
		private: static KIR5::Bitmap statusBMP;
		private: class Label:public KIR5::Label<>
		{
			public: Label();
		};

		private: KIR5::Shared<Label> scoreLabel;
		private: KIR5::Shared<Label> unityLabel;

		private: KIR5::Shared<Label> HHLabel;
		private: KIR5::Shared<Label> MMLabel;
		private: KIR5::Shared<Label> SSLabel;
		private: KIR5::Shared<Label> mapNameLabel;
		private: std::string ToSString(int i) const;

		private: int unityCount = 0;
		private: int scoreToReach = 0;
		private: int scoreCount = 0;

		public: int getUnityCount() const;
		public: int getScoreToReach() const;
		public: int getScoreCount() const;
		public: int getScoreToCollect() const;

		public: void addUnity(int count);
		public: void addScore(int count);

		public: void setUnity(int count);
		public: void setScore(int count, int toReach);

		public: void Restart();
		public: void updateLoopCounter(unsigned long long _loopCounter);

		public: StatusBar();

		public: void SetMap(const std::shared_ptr<Res::BluePrint> &bluePrint_);
		public: int Height() const;
		public: void Align();
	};
}