#pragma once

#include <vector>
#include <KIR\AL\KIR5_color.h>
#include <KIR\AL\KIR5_font.h>
#include <KIR\AL\KIR5_bitmap.h>


class StackTimer
{
	int count = 0;
	int current = 0;
	float timer = 0;
	public:
	StackTimer();
	void Start();
	void Initialize(int count);
	bool Add(float time);
	int Get() const;
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


//static KIR5::Bitmap unknown;
//static StackTimer gravityTimer;
//static Bitmap gravity;

KIR5::Bitmap CreateBitmapTransition(const KIR5::Bitmap &base, const KIR5::Bitmap &translation_);
//std::vector<KIR5::Bitmap> CreateBitmapTransition(KIR5::Bitmap base_, Bitmap translation_, const char *name);
//std::vector<KIR5::Bitmap> CreateBitmapTransition(Bitmap base_, Bitmap translation_, const char *name);