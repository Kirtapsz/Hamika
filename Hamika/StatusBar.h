#pragma once

#include <KIR\AL\KIR5_panel.h>
#include <KIR\AL\KIR5_Label.h>
#include <KIR\AL\KIR5_bitmap.h>

#include "World.h"

class StatusBar:public KIR5::Panel
{
	private: static KIR5::Bitmap statusBMP;
	private: class Label:public KIR5::Label<>
	{
		public: Label();
	};

	private: KIR5::EVENT<Label> aimLabel;
	private: KIR5::EVENT<Label> unitesLabel;
	private: KIR5::EVENT<Label> HHLabel;
	private: KIR5::EVENT<Label> MMLabel;
	private: KIR5::EVENT<Label> SSLabel;
	private: KIR5::EVENT<Label> mapNameLabel;
	private: int HH;
	private: int MM;
	private: int SS;
	private: std::string ToSString(int i) const;

	public: void Restart();
	public: int AimCollect;
	public: int UnitiyCollect;
	public: int Aim;
	public: void SetAimStr();
	public: void SetUnityStr();

	public: StatusBar();

	public: void SetMap(const BluePrint &disp_map);
	public: int GetAimRemaining() const;
	public: int GetUnityCount() const;
	public: void AddUnity(int collect);
	public: void AddAim(int collect);
	public: int Height() const;
	public: void Align();
};