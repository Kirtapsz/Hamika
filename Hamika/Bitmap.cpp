#include "Bitmap.h"
#include "Tools.h"

#include <KIR/KIR4_console.h>
#include <KIR/AL/KIR5_bitmap.h>
#include <KIR/AL/KIR5_bitmap_target.h>

namespace Res
{
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
	int Slides::getDrawNumber(std::float_t i) const
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

	void drawMissingObject(int x, int y, int w, int h, KIR5::Color color)
	{
		al_draw_filled_rectangle(x, y, x + w / 2, y + h / 2, KIR5::Color(255, 255, 255));
		al_draw_filled_rectangle(x + w / 2, y, x + w, y + h / 2, color);
		al_draw_filled_rectangle(x, y + h / 2, x + w / 2, y + h, color);
		al_draw_filled_rectangle(x + w / 2, y + h / 2, x + w, y + h, KIR5::Color(255, 255, 255));
	}

	KIR5::Bitmap BitmapBox::generate(int width, int height, KIR5::Color color)
	{
		KIR5::Bitmap bmp(width, height);
		KIR5::BitmapTarget target(bmp);
		drawMissingObject(0, 0, width, height, color);
		return bmp;
	}
	bool BitmapBox::initialize(std::uint32_t mode)
	{
		bitmap = generate(blockSizeInPixel, blockSizeInPixel, KIR5::Color(141, 74, 203));
		return true;
	}
	void BitmapBox::shutdown()
	{
		bitmap.free();
		subBitmap.free();
		Base::shutdown();
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

	KIR5::Bitmap StickyBitmap::get(const char *&key)
	{
		KIR5::Bitmap bitmap;
		bitmap.load(KIR5::pathCombine(path, std::string(key) + ".png").c_str());
		return bitmap;
	}
	void StickyBitmap::vadlidate(const char *&key) const
	{
	}


	KIR5::Bitmap UIElements::get(const char *&key)
	{
		if (key == ArrowRight)
		{
			KIR5::Bitmap _tmp = (*this)[ArrowUp];
			_tmp.rotate(ALLEGRO_PI * 1 / 2.f);
			return _tmp;
		}
		else if (key == ArrowDown)
		{
			KIR5::Bitmap _tmp = (*this)[ArrowUp];
			_tmp.flipVertical();
			return _tmp;
		}
		else if (key == ArrowLeft)
		{
			KIR5::Bitmap _tmp = (*this)[ArrowUp];
			_tmp.rotate(ALLEGRO_PI * 3 / 2.f);
			return _tmp;
		}
		else if (key == ArrowTiltedUpRight)
		{
			KIR5::Bitmap _tmp = (*this)[ArrowTiltedUpLeft];
			_tmp.flipHorizontal();
			return _tmp;
		}
		else if (key == ArrowTiltedDownRight)
		{
			KIR5::Bitmap _tmp = (*this)[ArrowTiltedUpLeft];
			_tmp.flipHorizontal();
			_tmp.flipVertical();
			return _tmp;
		}
		else if (key == ArrowTiltedDownLeft)
		{
			KIR5::Bitmap _tmp = (*this)[ArrowTiltedUpLeft];
			_tmp.flipVertical();
			return _tmp;
		}
		else if (key == StickRight)
		{
			KIR5::Bitmap _tmp = (*this)[StickUp];
			_tmp.rotate(ALLEGRO_PI * 1 / 2.f);
			return _tmp;
		}
		else if (key == StickDown)
		{
			KIR5::Bitmap _tmp = (*this)[StickUp];
			_tmp.flipVertical();
			return _tmp;
		}
		else if (key == StickLeft)
		{
			KIR5::Bitmap _tmp = (*this)[StickUp];
			_tmp.rotate(ALLEGRO_PI * 3 / 2.f);
			return _tmp;
		}
		else
		{
			return StickyBitmap::get(key);
		}
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

	void BitmapPool::finalise(KIR5::Bitmap &_fallbackBitmap)
	{
		addBitmap(_fallbackBitmap, Fallback);
		for (auto &x : layout)
		{
			for (auto &y : x)
			{
				y.subBitmap.sub(poolBitmap, y.x, y.y, y.w, y.h);
			}
		}
		fallbackBitmap = (*this)[Fallback];
	}
	void BitmapPool::shutdown()
	{
		layout.clear();
		poolBitmap.free();
		fallbackBitmap.free();
		Base::shutdown();
	}

	KIR5::SubBitmap &BitmapPool::operator[](const char *name)
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
		return fallbackBitmap;
	}


	bool Tiles::initialize(std::uint32_t mode)
	{
		maxBitmapSize = al_get_display_option(*display, ALLEGRO_MAX_BITMAP_SIZE);
		std::string basePath = KIR5::pathCombine(KIR5::getModuleDirectory(), path);

		KIR5::Bitmap lineDisappear(KIR5::pathCombine(basePath, "Original\\LineDisappear.png"));
		KIR5::Bitmap squareDisappear(KIR5::pathCombine(basePath, "Original\\SquareDisappear.png"));
		KIR5::Bitmap squareDisappearV4(KIR5::pathCombine(basePath, "Original\\RectangleDisappear_v4.png"));
		KIR5::Bitmap tmp;

		// Murphy
		tmp.load(KIR5::pathCombine(basePath, "Murphy\\MurphyBase.png"));
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 3) * 0, (tmp.height() / 4) * 0, (tmp.width() / 3), (tmp.height() / 4))), _Player_Base);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 3) * 1, (tmp.height() / 4) * 0, (tmp.width() / 3), (tmp.height() / 4) * 4)), _Player_Sniff);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 3) * 2, (tmp.height() / 4) * 0, (tmp.width() / 3), (tmp.height() / 4) * 4)), _Player_Push);

		tmp.load(KIR5::pathCombine(basePath, "Murphy\\MurphyMove.png"));
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 0, 0, tmp.width() / 4, tmp.height())), _Player_MovingUp);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 1, 0, tmp.width() / 4, tmp.height())), _Player_MovingRight);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 2, 0, tmp.width() / 4, tmp.height())), _Player_MovingDown);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 3, 0, tmp.width() / 4, tmp.height())), _Player_MovingLeft);

		tmp.load(KIR5::pathCombine(basePath, "Murphy\\MurphyPassIn.png"));
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 0, 0, tmp.width() / 4, tmp.height())), _Player_PassInDown);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 1, 0, tmp.width() / 4, tmp.height())), _Player_PassInLeft);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 2, 0, tmp.width() / 4, tmp.height())), _Player_PassInUp);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 3, 0, tmp.width() / 4, tmp.height())), _Player_PassInRight);

		tmp.load(KIR5::pathCombine(basePath, "Murphy\\MurphyPassOut.png"));
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 0, 0, tmp.width() / 4, tmp.height())), _Player_PassOutDown);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 1, 0, tmp.width() / 4, tmp.height())), _Player_PassOutLeft);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 2, 0, tmp.width() / 4, tmp.height())), _Player_PassOutUp);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(tmp, (tmp.width() / 4) * 3, 0, tmp.width() / 4, tmp.height())), _Player_PassOutRight);

		// 001
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\001-ZonkMoveHorizontal.png")), _001_ZonkMoveHorizontal);

		// 002
		tmp.load(KIR5::pathCombine(basePath, "Original\\002-BaseX.png"));
		addBitmap(tmp, _002_BaseX);
		addBitmap(CreateBitmapTransition(tmp, squareDisappearV4), _002_BaseX_square_disappear);

		// 003
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\003-HardwareBlueLed.png")), _003_HardwareBlueLed);

		// 004
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\004-HardwareBrick1.png")), _004_HardwareBrick1);

		// 005
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\005-HardwareBlueResistance.png")), _005_HardwareBlueResistance);

		// 006
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\006-HardwareGreenLed.png")), _006_HardwareGreenLed);

		// 007
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\007-HardwareRedLed.png")), _007_HardwareRedLed);

		// 008
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\008-HardwareResource.png")), _008_HardwareResource);

		// 009
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\009-HardwareResistance.png")), _009_HardwareResistance);

		// 010
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\010-HardwareStripLine.png")), _010_HardwareStripLine);

		// 011
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\011-HardwareYellowResistance.png")), _011_HardwareYellowResistance);

		// 012
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\012-HardwareRedCapacitor.png")), _012_HardwareRedCapacitor);

		// 013
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\013-Bug.png")), _013_Bug);

		// 014
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\014-InfotronDisappear.png")), _014_InfotronDisappear);
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\014-InfotronMoveHorizontal.png")), _014_InfotronMoveHorizontal);

		// 015
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\015-Exit.png")), _015_Exit);

		// 016
		KIR5::Bitmap PortsBmp(KIR5::pathCombine(basePath, "Original\\Ports.png"));
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(PortsBmp, 0, PortsBmp.width() * 6, PortsBmp.width(), PortsBmp.width())), _016_CrossPort);

		// 017
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(PortsBmp, 0, PortsBmp.width() * 2, PortsBmp.width(), PortsBmp.width())), _017_DualPortHorizontal);

		// 018
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(PortsBmp, 0, PortsBmp.width() * 5, PortsBmp.width(), PortsBmp.width())), _018_DualPortVertical);

		// 019
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(PortsBmp, 0, PortsBmp.width() * 3, PortsBmp.width(), PortsBmp.width())), _019_OneWayPortDown);

		// 020
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(PortsBmp, 0, PortsBmp.width() * 1, PortsBmp.width(), PortsBmp.width())), _020_OneWayPortLeft);

		// 021
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(PortsBmp, 0, PortsBmp.width() * 0, PortsBmp.width(), PortsBmp.width())), _021_OneWayPortRight);

		// 022
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(PortsBmp, 0, PortsBmp.width() * 4, PortsBmp.width(), PortsBmp.width())), _022_OneWayPortUp);

		// 023
		KIR5::Bitmap RAMchipBmp(KIR5::pathCombine(basePath, "Original\\RAMchip.png"));
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(RAMchipBmp, (RAMchipBmp.width() / 3) * 1, (RAMchipBmp.height() / 2) * 1, (RAMchipBmp.width() / 3), (RAMchipBmp.height() / 2))), _023_RAMChipsMini);

		// 024
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(RAMchipBmp, (RAMchipBmp.width() / 3) * 0, (RAMchipBmp.height() / 2) * 0, (RAMchipBmp.width() / 3), (RAMchipBmp.height() / 2))), _024_RAMChipsLeft);

		// 025
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(RAMchipBmp, (RAMchipBmp.width() / 3) * 1, (RAMchipBmp.height() / 2) * 0, (RAMchipBmp.width() / 3), (RAMchipBmp.height() / 2))), _025_RAMChipsRight);

		// 026
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\026-Electrons.png")), _026_Electrons);

		// 027
		KIR5::Bitmap SnikSnakBmp(KIR5::pathCombine(basePath, "Original\\027-SnikSnak.png"));
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(SnikSnakBmp, (SnikSnakBmp.width() / 5) * 0, 0, (SnikSnakBmp.width() / 5), SnikSnakBmp.height())), _027_SnikSnakMoveUp);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(SnikSnakBmp, (SnikSnakBmp.width() / 5) * 1, 0, (SnikSnakBmp.width() / 5), SnikSnakBmp.height())), _027_SnikSnakMoveRight);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(SnikSnakBmp, (SnikSnakBmp.width() / 5) * 2, 0, (SnikSnakBmp.width() / 5), SnikSnakBmp.height())), _027_SnikSnakMoveDown);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(SnikSnakBmp, (SnikSnakBmp.width() / 5) * 3, 0, (SnikSnakBmp.width() / 5), SnikSnakBmp.height())), _027_SnikSnakMoveLeft);
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(SnikSnakBmp, (SnikSnakBmp.width() / 5) * 4, 0, (SnikSnakBmp.width() / 5), SnikSnakBmp.height())), _027_SnikSnakRotate);

		// 028
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\028-Terminal.png")), _028_Terminal);

		// 029
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\029-Utility1.png")), _029_Utility1);

		// 030
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\030-Utility2.png")), _030_Utility2);
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\030-Utility2Activated.png")), _030_Utility2Activated);

		// 031
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\031-Utility3.png")), _031_Utility3);

		// 032
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\032-Explosion.png")), _032_Explosion);

		// 033

		// 034

		// 035
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(RAMchipBmp, (RAMchipBmp.width() / 3) * 2, (RAMchipBmp.height() / 2) * 0, (RAMchipBmp.width() / 3), (RAMchipBmp.height() / 2))), _035_RAMChipsTop);

		// 036
		addBitmap(KIR5::Bitmap(al_create_sub_bitmap(RAMchipBmp, (RAMchipBmp.width() / 3) * 2, (RAMchipBmp.height() / 2) * 1, (RAMchipBmp.width() / 3), (RAMchipBmp.height() / 2))), _036_RAMChipsBottom);

		// 037
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\037-HardwareSwitch.png")), _037_HardwareSwitch);

		// 038
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\038-OneWayPortDownGrav.png")), _038_OneWayPortDownGrav);

		// 039
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\039-OneWayPortLeftGrav.png")), _039_OneWayPortLeftGrav);

		// 040
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\040-OneWayPortRightGrav.png")), _040_OneWayPortRightGrav);

		// 041
		addBitmap(KIR5::Bitmap(KIR5::pathCombine(basePath, "Original\\041-OneWayPortUpGrav.png")), _041_OneWayPortUpGrav);


		finalise(BitmapBox::generate(blockSizeInPixel, blockSizeInPixel, KIR5::Color(255, 0, 255)));

		return true;
	}

	BitmapBox bitmapBox("");
	Tiles tiles("Hamika\\texture\\Tiles");
	UIElements uielements("Hamika\\texture\\UIelement");

};
