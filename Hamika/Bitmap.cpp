#include "Bitmap.h"
#include "Tools.h"

#include <KIR/KIR4_console.h>
#include <KIR\AL\KIR5_bitmap_target.h>

#include <algorithm>

void drawMissingObject(int x, int y, int w, int h, KIR5::Color color)
{
	al_draw_filled_rectangle(x, y, x + w / 2, y + h / 2, KIR5::Color(255, 255, 255));
	al_draw_filled_rectangle(x + w / 2, y, x + w, y + h / 2, color);
	al_draw_filled_rectangle(x, y + h / 2, x + w / 2, y + h, color);
	al_draw_filled_rectangle(x + w / 2, y + h / 2, x + w, y + h, KIR5::Color(255, 255, 255));
}


KIR5::Bitmap unknownBitmap(KIR5::Color color)
{
	KIR5::Bitmap bmp(blockSizeInPixel, blockSizeInPixel);
	KIR5::BitmapTarget target(bmp);
	drawMissingObject(0, 0, blockSizeInPixel, blockSizeInPixel, color);
	return bmp;
}



void StackTimer::Initialize()
{
	timer = 0.f;
	time = 0.f;
	numberOfFrames = 0;
	drawNumber = 0;
}
void StackTimer::SetAnimationTime(float time)
{
	std::uniform_real_distribution<float> distribution(0.f, time);

	this->time = time;
	this->timer = distribution(generator);
	UpdateDrawNumber();
}
void StackTimer::SetNumberOfFrames(int numberOfFrames)
{
	this->numberOfFrames = numberOfFrames;
}
int StackTimer::GetDrawNumber()
{
	return drawNumber;
}
bool StackTimer::UpdateDrawNumber()
{
	int drawNumber_ = limiter<int>(0, this->numberOfFrames - 1, (this->timer / this->time) * this->numberOfFrames);
	if (this->drawNumber != drawNumber_)
	{
		this->drawNumber = drawNumber_;
		return true;
	}
	return false;
}
void StackTimer::UpdateTimer()
{
	timer += CA;
	if (timer > time)
	{
		timer -= time;
	}
}

Slides::Slides()
{

}
void Slides::initialize(KIR5::Bitmap &bitmap, KIR5::SubBitmap &errorBitmap_)
{
	errorBitmap = errorBitmap_;
	if (bitmap)
	{
		int size = bitmap.width();

		bitmaps.resize(bitmap.height() / bitmap.width());
		for (size_t i = 0; i < bitmaps.size(); i++)
			bitmaps[i].sub(bitmap, 0, i * size, size, size);
	}
	else
	{
		bitmaps.clear();
	}
}
void Slides::initialize(KIR5::SubBitmap &bitmap, KIR5::SubBitmap &errorBitmap_)
{
	errorBitmap = errorBitmap_;
	if (bitmap)
	{
		int size = bitmap.width();

		bitmaps.resize(bitmap.height() / bitmap.width());
		for (size_t i = 0; i < bitmaps.size(); i++)
			bitmaps[i].sub(bitmap, 0, i * size, size, size);
	}
	else
	{
		bitmaps.clear();
	}
}
int Slides::getCount() const
{
	return bitmaps.size();
}
int Slides::getDrawNumber(float i) const
{
	return limiter<int>(0, bitmaps.size() - 1, i * bitmaps.size());
}
KIR5::SubBitmap &Slides::operator[](size_t i)
{
	if (i >= bitmaps.size())
	{
		clog << KIR4::LRED << "Bitmap error! try to get not exists bitmap! [ " << i << " ]" << KIR4::eol;
		return errorBitmap;
	}
	else
		return bitmaps[i];
}

KIR5::Bitmap CreateBitmapTransition(const KIR5::Bitmap &base, const KIR5::Bitmap &translation_)
{
	if (base.width() != translation_.width())
	{
		return KIR5::Bitmap();
	}

	KIR5::Bitmap result = translation_.clone();

	KIR5::Bitmap::lock_ABGR lresult;
	KIR5::Bitmap::lock_ABGR lbase;

	lresult.lock(result);
	lbase.lock(base);

	float rate;

	unsigned __int8 *itr;
	unsigned __int8 a;
	unsigned __int8 *itb;

	for (int y = 0; y < result.height(); ++y)
	{
		itb = lbase.pixels() + lbase.pitch() * (y % base.height());
		itr = lresult.pixels() + lresult.pitch() * y;
		for (int x = 0; x < result.width(); ++x)
		{
			a = *itr++;
			if (a > *itb++)
			{
				*itr = *itb;

				*itr++ = *itb++;
				*itr++ = *itb++;
				*itr++ = *itb++;
			}
			else
			{
				rate = a / float(255);
				*itr++ = (*itb++) * rate;
				*itr++ = (*itb++) * rate;
				*itr++ = (*itb++) * rate;
			}
		}
	}

	lresult.unlock();
	lbase.unlock();

	return result;
}
