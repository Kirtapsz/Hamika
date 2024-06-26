#ifndef __MAP_DRAWNER_H__
#define __MAP_DRAWNER_H__

#include <KIR\AL\KIR5_font.h>
#include <KIR\AL\KIR5_bitmap.h>

#include "Types.h"
#include "Matrix.h"
#include "SceneBlock.h"
#include "Object.h"
#include "BaseFunctions.h"
#include "Bitmap.h"

//#define SHOW_COORDS
//#define SHOW_TIMER

template <typename ACTIVE_BLOCK_T>
class SceneDrawer
{
	private: std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> map;

	private: static Object::Animator::EntityData gravityAnimator;
	private: static Res::Slides gravitySlides;

	private: unsigned long long TotalPointDrawCount;
	private: unsigned long long TotalObjectDrawCount;

	public: bool layerActive = true;
	public: bool blockRefreshActive = true;

	private: bool *globalGravity;
	public: void setGlobalGravity(bool *globalGravity);

	private: KIR5::Bitmap RedrawnedBitmap;
	private: KIR5::Bitmap LayerBitmap;
	private: KIR5::Bitmap BlocksBitmap;
	private: KIR5::Bitmap TempBitmap;

	private: const int BlocksBitmapBufferSize = 2;

	private: Type::Size DrawSize;
	private: Type::Size LastDrawOffset;
	private: Type::Size BlocksBitmapDrawOffset;
	private: Type::Size DrawOffset;//bal fels� x,y

	private: bool CameraVcenter;
	private: bool CameraHcenter;
	private: bool BlocksBitmapVcenter;
	private: bool BlocksBitmapHcenter;

	private: Type::Move CameraLast;
	private: Type::Move CameraBegin;
	private: Type::Move CameraEnd;
	private: Type::Move CameraSize;//mennyi f�r r�
	private: Type::Move CameraCounts;//mennyit lehet oldalra l�ni, hogy el�rj�k a kamera sz�l�t

	private: Type::Size BlocksBitmapSize;//darabsz�m
	private: Type::Size BlocksBitmapCounts;//mennyit lehet oldalra l�ni, hogy el�rj�k a buffer sz�l�t

	private: Type::Coord DrawBeginSource;
	private: Type::Coord DrawBeginLast;
	private: Type::Coord DrawEndLast;
	private: Type::Coord DrawBegin;
	private: Type::Coord DrawEnd;

	private: float CameraX1;//t�rk�pen mik a hat�rok
	private: float CameraX2;
	private: float CameraY1;
	private: float CameraY2;
	private: void RedrawnRow(Type::Coord::base row, Type::Coord::base begin, Type::Coord::base end);
	private: void RedrawnCol(Type::Coord::base col, Type::Coord::base begin, Type::Coord::base end);

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

		  //kamera k�z�pen van
	public: void MoveCameraTo(Type::Move camera);
	public: void InitializeDrawOptions(Type::Size cameraPhySize, Type::CameraSize cameraSizeAdjust);

	public: SceneDrawer();
	public: void DrawBlocks(int x, int y);

	public: void SetMap(std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> &map);
	public: void Redrawn(Type::Coord coord);
};

#endif

#include "SceneDrawer.hpp"
