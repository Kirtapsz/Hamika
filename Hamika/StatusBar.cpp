#include "StatusBar.h"

#include <KIR/KIR4_string.h>
#include "Object.h"
#include "Font.h"

namespace UI::Game
{
	KIR5::Bitmap StatusBar::statusBMP;

	StatusBar::Label::Label()
	{
		show();
		setTextColor(al_map_rgb(200, 200, 200));
		setTextFont(Res::MajorSnafu[30]);
		setTextAlignment(KIR5::CENTER);
	}

	std::string StatusBar::ToSString(int i) const
	{
		if (i < 10)
			return "0" + KIR4::to_string(i);
		else
			return KIR4::to_string(i);
	}

	void StatusBar::Restart()
	{
		HHLabel->setText("00");
		MMLabel->setText("00");
		SSLabel->setText("00");
	}

	void StatusBar::SetAimStr()
	{
		aimLabel->setText((KIR4::to_string(Aim - AimCollect)).c_str());
	}
	void StatusBar::SetUnityStr()
	{
		unitesLabel->setText(KIR4::to_string(UnitiyCollect).c_str());
	}
	void StatusBar::updateLoopCounter(unsigned long long _loopCounter)
	{
		std::time_t timeMS = _loopCounter * CA * 1000;

		std::time_t ms = timeMS % 1000;

		timeMS /= 1000;
		std::time_t sec = timeMS % 60;

		timeMS /= 60;
		std::time_t min = timeMS % 60;

		timeMS /= 60;
		std::time_t hour = timeMS % 24;

		HHLabel->setText(ToSString(hour));
		MMLabel->setText(ToSString(min));
		SSLabel->setText(ToSString(sec));
	}

	StatusBar::StatusBar()
	{
		*this << aimLabel << unitesLabel << HHLabel << MMLabel << SSLabel << mapNameLabel;

		mapNameLabel->position(92, 9);
		aimLabel->position(120, 46);
		unitesLabel->position(233, 46);
		HHLabel->position(318, 46);
		MMLabel->position(365, 46);
		SSLabel->position(412, 46);

		mapNameLabel->resize(365, 28);
		aimLabel->resize(68, 28);
		unitesLabel->resize(68, 28);
		HHLabel->resize(45, 28);
		MMLabel->resize(45, 28);
		SSLabel->resize(45, 28);

		statusBMP = Res::uielements[Res::UIElements::StatusBar];
		resize(550, 83);
		show();

		fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			statusBMP.drawScaled(x_, y_, w_, h_);

			DrawObject(ObjectID::Infotron, x_ + 92, y_ + 46, 28, 28);
			DrawObject(ObjectID::Utility2, x_ + 205, y_ + 46, 28, 28);
		}));
	}

	void StatusBar::SetMap(const std::shared_ptr<Res::BluePrint> &bluePrint_)
	{
		mapNameLabel->setText(bluePrint_->title);
		AimCollect = 0;
		UnitiyCollect = 0;
		Aim = bluePrint_->scoreToUnlock;
		SetAimStr();
		SetUnityStr();
		Restart();
	}
	int StatusBar::GetAimRemaining() const
	{
		return Aim - AimCollect;
	}
	int StatusBar::GetUnityCount() const
	{
		return UnitiyCollect;
	}
	void StatusBar::AddUnity(int collect)
	{
		UnitiyCollect += collect;
		if (UnitiyCollect < 0)
			UnitiyCollect = 0;
		SetUnityStr();
	}
	void StatusBar::AddAim(int collect)
	{
		AimCollect += collect;
		if (AimCollect < 0)
			AimCollect = 0;
		if (AimCollect > Aim)
			AimCollect = Aim;
		SetAimStr();
	}
	int StatusBar::Height() const
	{
		return height();
	}
	void StatusBar::Align()
	{
		align(KIR5::HCENTER | KIR5::BOTTOM);
		show();
		bringTop();
	}
}