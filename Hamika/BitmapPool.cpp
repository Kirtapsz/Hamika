#include "BitmapPool.h"
#include "MainEvent.h"

#include <KIR/AL/KIR5_bitmap.h>
#include <KIR/AL/KIR5_bitmap_target.h>
#include <KIR/AL/KIR5_color.h>

#include <vector>


BitmapPool::BitmapPool()
{

}

BitmapPool::Param BitmapPool::createNewPool(const KIR5::Bitmap &bmp, const char *name)
{
	layout.clear();

	poolBitmap = al_create_bitmap(bmp.width(), bmp.height());
	KIR5::BitmapTarget target(poolBitmap);
	al_clear_to_color({0,0,0,0});
	bmp.draw(0, 0);

	Param param = {name,0,0,bmp.width(), bmp.height()};
	std::vector<Param> paramv;
	paramv.push_back(param);
	layout.push_back(paramv);

	return param;
}
BitmapPool::Param BitmapPool::extendPool(const KIR5::Bitmap &bmp, const char *name)
{
	Param param;
	int w = bmp.width();
	int h = bmp.height();

	Param paramTmp = {0};

	for (auto &x : layout)
	{
		paramTmp = x[x.size() - 1];
		if (paramTmp.w == w && paramTmp.y + paramTmp.h + h <= maxBitmapSize && (
			poolBitmap.width() >= poolBitmap.height() ||
			paramTmp.y + paramTmp.h + h <= poolBitmap.height()
			))
		{
			param = {name,
				paramTmp.x,
				paramTmp.y + paramTmp.h,
				w,h};

			if (poolBitmap.height() < paramTmp.y + paramTmp.h + h)
			{
				KIR5::Bitmap newPoolBitmap = al_create_bitmap(poolBitmap.width(), paramTmp.y + paramTmp.h + h);
				{
					KIR5::BitmapTarget target(newPoolBitmap);
					al_clear_to_color({0,0,0,0});
					poolBitmap.draw(0, 0);
				}
				poolBitmap = newPoolBitmap;
			}

			x.push_back(param);

			return param;
		}
	}

	KIR5::Bitmap newPoolBitmap = al_create_bitmap(paramTmp.x + paramTmp.w + w, (std::max)(poolBitmap.height(), h));
	{
		KIR5::BitmapTarget target(newPoolBitmap);
		al_clear_to_color({0,0,0,0});
		poolBitmap.draw(0, 0);
	}
	poolBitmap = newPoolBitmap;

	param = {name,
		paramTmp.x + paramTmp.w,
		0,
		w,h};
	std::vector<Param> paramv;
	paramv.push_back(param);
	layout.push_back(paramv);
	return param;
}

BitmapPool::Param BitmapPool::getParamForBitmap(const KIR5::Bitmap &bmp, const char *name)
{
	Param param;

	if (layout.size() == 0)
	{
		param = createNewPool(bmp, name);
	}
	else
	{
		param = extendPool(bmp, name);
	}

	return param;
}

void BitmapPool::addBitmap(const KIR5::Bitmap &bmp, const char *name)
{
	Param param = getParamForBitmap(bmp, name);
	KIR5::BitmapTarget target(poolBitmap);
	bmp.draw(param.x, param.y);
}

void BitmapPool::finalise(KIR5::Bitmap &errorBitmap_)
{
	addBitmap(errorBitmap_, "Error");
	for (auto &x : layout)
	{
		for (auto &y : x)
		{
			y.subBitmap.sub(poolBitmap, y.x, y.y, y.w, y.h);
		}
	}
	errorBitmap = get("Error");
}

KIR5::SubBitmap &BitmapPool::get(const char *name)
{
	for (auto &x : layout)
	{
		for (auto &y : x)
		{
			if (strcmp(y.name, name) == 0)
			{
				return y.subBitmap;
			}
		}
	}
	return errorBitmap;
}