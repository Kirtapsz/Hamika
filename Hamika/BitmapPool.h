#pragma once

#include <KIR/AL/KIR5_bitmap.h>
#include <KIR/AL/KIR5_bitmap_target.h>
#include <KIR/AL/KIR5_color.h>

#include <vector>

class BitmapPool
{
	private: struct Param
	{
		const char *name;
		int x, y, w, h;
		KIR5::SubBitmap subBitmap;
	};
	private: KIR5::Bitmap poolBitmap;
	private: KIR5::SubBitmap errorBitmap;
	private: std::vector<std::vector<Param>> layout;

	public: BitmapPool();

	private: Param createNewPool(const KIR5::Bitmap &bmp, const char *name);
	private: Param extendPool(const KIR5::Bitmap &bmp, const char *name);
	private: Param getParamForBitmap(const KIR5::Bitmap &bmp, const char *name);

	public: void addBitmap(const KIR5::Bitmap &bmp, const char *name);
	public: void finalise(KIR5::Bitmap &errorBitmap);
	public: KIR5::SubBitmap &get(const char *name);
};