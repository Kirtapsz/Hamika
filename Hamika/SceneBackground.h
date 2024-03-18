#ifndef __SCENE_BACKGROUND_H__
#define __SCENE_BACKGROUND_H__

#include <KIR\AL\KIR5_font.h>
#include <KIR\AL\KIR5_bitmap.h>
#include <KIR/AL/KIR5_bitmap_target.h>

#include "UItools.h"
#include "Types.h"
#include "Matrix.h"
#include "SceneBlock.h"
#include "Object.h"
#include "BaseFunctions.h"
#include "Bitmap.h"

namespace UI::Scene
{
	class BackgoundInterface: virtual public Panel
	{
		public: virtual void setCamera(Type::Camera _camera) = 0;
		public: virtual void setCameraScale(float _camera_scale) = 0;
	};

	class SingleSeamlessBackground: virtual public BackgoundInterface
	{
		private: float source_scale = 1.f; // scale of the source bitmap
		private: KIR5::Bitmap source; // bitmap to be drawned N times to cover the background

		private: KIR5::Bitmap buffer; // buffer bitmap
		private: static constexpr int buffer_size_c = 400; // extra size of the buffer bitmap on the edges
		private: bool redraw = true; // request redraw the buffer bitmap

		private: Type::Camera original_camera{0.f,0.f}; // real camera position, this is aligned to teh top-left corner
		private: Type::var2D<__int32> camera{0,0}; // real camera position, this is aligned to teh top-left corner
		private: float camera_scale = 1.f;
		private: Type::var2D<__int32> camera_snapshot{0,0}; // camera position when the buffer bitmap redrawned
		private: Type::var2D<__int32> camera_shift{0,0}; // snapshot and the current camera shift, responsible to shift the drawn position and check if it is out of buffer

		public: void setBitmap(const KIR5::Bitmap &_bitmap, float _scale = 1.f);
		public: virtual void setCamera(Type::Camera _camera);
		public: virtual void setCameraScale(float _camera_scale);
		public: Type::Camera getCamera() const;

		public: SingleSeamlessBackground();
	};
}

#endif
