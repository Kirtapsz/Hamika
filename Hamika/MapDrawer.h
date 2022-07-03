#ifndef __MAP_DRAWNER_H__
#define __MAP_DRAWNER_H__

#include <KIR\AL\KIR5_font.h>
#include <KIR\AL\KIR5_bitmap.h>

#include "Types.h"
#include "Array2D.h"
#include "ActiveBlock.h"
#include "Object.h"
#include "BaseFunctions.h"
#include "Bitmap.h"

//#define SHOW_COORDS
//#define SHOW_TIMER

template <typename ACTIVE_BLOCK_T>
class MapDrawer
{
	public: void PrintBlock(Type::Coord coord);

	private: std::shared_ptr<Array2D<ACTIVE_BLOCK_T>> map;

	private: static StackTimer gravityAnimator;
	private: static Slides gravitySlides;

	private: unsigned long long TotalPointDrawCount;
	private: unsigned long long TotalObjectDrawCount;

	public: bool layerActive = true;
	public: bool blockRefreshActive = true;

	private: KIR5::Bitmap RedrawnedBitmap;
	private: KIR5::Bitmap LayerBitmap;
	private: KIR5::Bitmap BlocksBitmap;
	private: KIR5::Bitmap TempBitmap;

	private: const int BlocksBitmapBufferSize = 2;

	private: Type::Size DrawSize = {blockSizeInPixel,blockSizeInPixel};
	private: Type::Size LastDrawOffset;
	private: Type::Size BlocksBitmapDrawOffset;
	private: Type::Size DrawOffset;//bal felsõ x,y

	private: bool CameraVcenter;
	private: bool CameraHcenter;
	private: bool BlocksBitmapVcenter;
	private: bool BlocksBitmapHcenter;

	private: Type::Move CameraLast;
	private: Type::Move CameraBegin;
	private: Type::Move CameraEnd;
	private: Type::Move CameraSize;//mennyi fér rá
	private: Type::Move CameraCounts;//mennyit lehet oldalra léni, hogy elérjük a kamera szélét

	private: Type::Size BlocksBitmapSize;//darabszám
	private: Type::Size BlocksBitmapCounts;//mennyit lehet oldalra léni, hogy elérjük a buffer szélét

	private: Type::Coord DrawBeginSource;
	private: Type::Coord DrawBeginLast;
	private: Type::Coord DrawEndLast;
	private: Type::Coord DrawBegin;
	private: Type::Coord DrawEnd;

	private: float CameraX1;//térképen mik a határok
	private: float CameraX2;
	private: float CameraY1;
	private: float CameraY2;
	private: float CalculatePerSec;
	private: void RedrawnRow(Type::Coord::Type row, Type::Coord::Type begin, Type::Coord::Type end);
	private: void RedrawnCol(Type::Coord::Type col, Type::Coord::Type begin, Type::Coord::Type end);

	public: Type::Coord GetDrawBeginSource() const;
	public: Type::Coord GetDrawBegin() const;
	public: Type::Coord GetDrawEnd() const;
	public: Type::Size GetBitmapDrawOffset() const;
	public: Type::Size GetBitmapSize() const;

	public: Type::Size GetDrawSize() const;
	public: Type::Size GetDrawOffSet() const;

	public: Type::Move GetCameraSize() const;
	public: Type::Move GetCamera() const;

	public: Type::Coord GetFromCursor(int x, int y);

		  //kamera középen van
	public: void MoveCameraTo(Type::Move camera);
	public: void InitializeDrawOptions(int width, int height, float CalculatePerSec);

	public: MapDrawer();
	public: void DrawBlocks(int x, int y);

	public: void SetMap(std::shared_ptr<Array2D<ACTIVE_BLOCK_T>> &map);
	public: void Redrawn(Type::Coord coord);
};

#endif

#include "MapDrawer.hpp"
