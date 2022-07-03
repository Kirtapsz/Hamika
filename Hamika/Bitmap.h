#pragma once

#include <vector>
#include <KIR\AL\KIR5_color.h>
#include <KIR\AL\KIR5_font.h>
#include <KIR\AL\KIR5_bitmap.h>


struct StackTimer
{
	float time;
	float timer;
	int numberOfFrames;
	int drawNumber;

	void Initialize();
	void SetAnimationTime(float AnimationTime);
	void SetNumberOfFrames(int FramesNumber);
	int GetDrawNumber();
	bool UpdateDrawNumber();
	void UpdateTimer();
};

class Slides
{
	private: std::vector<KIR5::SubBitmap> bitmaps;
	private: KIR5::SubBitmap errorBitmap;

	public: Slides();

	public: void initialize(KIR5::Bitmap &bitmap, KIR5::SubBitmap &errorBitmap);
	public: void initialize(KIR5::SubBitmap &bitmap, KIR5::SubBitmap &errorBitmap);
	public: int getCount() const;
	public: int getDrawNumber(float i) const;
	public: KIR5::SubBitmap &operator[](size_t i);
};

void drawMissingObject(int x, int y, int w, int h, KIR5::Color color);
KIR5::Bitmap unknownBitmap(KIR5::Color color);

KIR5::Bitmap CreateBitmapTransition(const KIR5::Bitmap &base, const KIR5::Bitmap &translation_);