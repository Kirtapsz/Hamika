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


struct SceneLayer
{
	static constexpr unsigned int blocks = 0;
	static constexpr unsigned int erases = 1;
	static constexpr unsigned int refresh = 2;
	static constexpr unsigned int gravity = 3;
	static constexpr unsigned int coord = 4;

	static constexpr unsigned int number_of_layers = 5;

	bool enable_;
	KIR5::Bitmap bitmap_;
};

template <typename ACTIVE_BLOCK_T>
class BlocksDrawer
{
	private: unsigned long long total_draw_cycle_{0};
	private: unsigned long long total_block_draw_count_{0};
	private: unsigned long long total_object_draw_count_{0};

	private: static Object::Animator::EntityData gravity_animator_;
	private: static Res::Slides gravity_slides_;
	private: bool global_gravity_;
	private: bool global_gravity_updated_;
	public: void setGlobalGravity(bool _global_gravity);

	private: std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> map_;
	public: void setMap(std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> &_map);

	private: Type::Pixels block_size_;
	public: Type::Pixels getBlockSize() const;
	public: void setBlockSize(Type::Size _camera_size, Type::CameraSize _block_size_adjust);

	private: Type::Pixels bitmap_size_;
	public: Type::Pixels getBitmapSize() const;

	private: Type::Size block_count_;
	private: Type::Pixels draw_box_offset_;
	public: Type::Pixels getDrawBoxOffset() const;
	private: Type::rec2D<Type::Coord> draw_box_;
	public: Type::rec2D<Type::Coord> getDrawBox() const;
	private: Type::rec2D<Type::Coord> draw_box_edges_;

	private: KIR5::Bitmap bitmap_temp;
	private: std::array<SceneLayer, 5> layers_{{
		{true},
		{true},
		{false},
		{true},
		{true},
	}};
	public: void enableLayer(unsigned int _layer_id, bool _enabled);
	public: bool isLayerEnabled(unsigned int _layer_id) const;

	public: BlocksDrawer();

	public: void updateConfiguration(Type::Size _number_of_blocks);

	private: void shiftBlocks(Type::Coord box_shift);
	private: void checkForDrawRequests();
	private: void fadeBitmap(KIR5::Bitmap &_bitmap);
	private: void clearBlocks();
	private: void clearRedrawIndicator();
	private: void clearDrawnedIndicator();
	private: void clearDrawCountIndicator();
	private: void drawBitmapOfRefresh();
	private: void drawBitmapOfCoords();
	private: void drawGravity();

	private: static constexpr int TOP_DRAW{1};
	private: static constexpr int STANDARD_DRAW{2};
	private:
	template <int DRAW_T>
	void drawObjects();

	public: void updateBitmap();
	public: void updateDrawBox(Type::Coord _corner);
	public: void drawBitmaps(Type::Pixels _target);
	public: Type::Coord getTargetOfCursor(Type::Pixels _cur) const;

	private: void setRedrawOnBlockRepeat(Type::Coord coord);

	public: void setRedrawOnBlock(Type::Coord coord);

	private: Type::rec2D<Type::Pixels> getObjectRect(const Type::Coord &coord, const ACTIVE_BLOCK_T &block) const;
};

template <typename ACTIVE_BLOCK_T>
class MoveDrawer
{
	private: BlocksDrawer<ACTIVE_BLOCK_T> blocks_drawer_;
	public: Type::Pixels getBlockSize() const;
	public: void setGlobalGravity(bool _global_gravity);
	public: void setRedrawOnBlock(Type::Coord coord);
	public: void enableLayer(unsigned int _layer_id, bool _enabled);
	public: bool isLayerEnabled(unsigned int _layer_id) const;
	public: Type::Pixels getBitmapSize() const;

	private: std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> map_;
	public: void setMap(std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> &_map);

	private: Type::Pixels block_offset_;

	private: Type::Coord bufferred_blocks_count_;

	private: Type::Move update_camera_;
	private: Type::Move camera_center_;
	private: Type::Move last_camera_center_;
	private: Type::rec2D<Type::Move> camera_cage_;
	private: Type::Pixels camera_size_;

	private: Type::Move camera_count_;
	public: Type::Move getCameraSizeInBlocks() const;

	private: Type::Pixels camera_radius_size_;
	private: Type::Move camera_radius_count_;
	private: Type::rec2D<Type::Move> camera_move_box_;
	private: Type::rec2D<Type::Coord> camera_box_;

	public: void updateConfiguration(Type::Size _camera_size, Type::CameraSize _block_size_adjust, int _bufferred_blocks_count = 2);

	private: void updateCameraBox(bool _force_update_draw_box);
	public: void moveCamera(Type::Move camera);
	private: void updateCamera();
	public: Type::Move getCameraCenter() const;

	public: void updateBitmap();
	public: void drawBitmaps(Type::Pixels _target);

	public: Type::Coord getTargetOfCursor(Type::Pixels _cur) const;
};

template <typename ACTIVE_BLOCK_T>
class SceneDrawer: public ACTIVE_BLOCK_T
{
	private: MoveDrawer<ACTIVE_BLOCK_T> move_drawer_;
	public: Type::Pixels getBlockSize() const;
	public: void setGlobalGravity(bool _global_gravity);
	public: void setRedrawOnBlock(Type::Coord coord);
	public: void enableLayer(unsigned int _layer_id, bool _enabled);
	public: bool isLayerEnabled(unsigned int _layer_id) const;
	public: void moveCamera(Type::Move camera);
	public: Type::Move getCameraCenter() const;
	public: void updateBitmap();
	public: void setMap(std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> &_map);
	public: Type::Move getCameraSizeInBlocks() const;

	private: Type::Pixels panel_shift_{0,0};
	public: void updateConfiguration(Type::Size _camera_size, Type::CameraSize _block_size_adjust, int _bufferred_blocks_count = 2);
	public: void drawBitmaps(Type::Pixels _target);
	public: Type::Coord getTargetOfCursor(Type::Pixels _cur) const;
};

template<typename T>
using SceneDrawer_T = SceneDrawer<T>;

#endif

#include "SceneDrawer.hpp"
