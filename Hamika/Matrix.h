#pragma once

#include <vector>
#include "Types.h"
#include <KIR/KIR4_console.h>

template<class Data>
class Matrix
{
	private: Type::Size size = {0,0};

	protected: std::vector<std::vector<Data>> data;

	public: inline operator Type::Size() const
	{
		return size;
	}
	public: inline Data &operator[](Type::Coord coord)
	{
#ifdef _DEBUG
		if (coord.x >= size.width || coord.y >= size.height || coord.x < 0 || coord.y < 0)
			clog << KIR4::LRED << "Coord ERROR!: " << coord << KIR4::eol << KIR4::pp;
#endif // _DEBUG
		return data[coord.x][coord.y];
	}
	public: inline const Data &operator[](Type::Coord coord) const
	{
#ifdef _DEBUG
		if (coord.x >= size.width || coord.y >= size.height || coord.x < 0 || coord.y < 0)
			clog << KIR4::LRED << "Coord ERROR!: " << coord << KIR4::eol << KIR4::pp;
#endif // _DEBUG
		return data[coord.x][coord.y];
	}

	public: Matrix(Type::Size size)
	{
		if (size.width > 0 && size.height > 0)
		{
			this->size = size;

			data.resize(size.width);
			for (Type::Size::Type i = 0; i < size.width; i++)
				data[i].resize(size.height);
		}
	}
	public: Matrix()
	{

	}
	public: Matrix(Matrix &&array2D):
		size(array2D.size),
		data(std::move(array2D.data))
	{

	}
	public: virtual ~Matrix()
	{
		data.clear();
		size = {0,0};
	}
	public: inline bool Exists() const
	{
		return size.width > 0;
	}
	public: inline bool Test(Type::Coord coord) const
	{
		return
			coord.x < size.width &&
			coord.y < size.height &&
			coord.x >= 0 &&
			coord.y >= 0;
	}

	public: void resize(Type::Size size)
	{
		if (size.width > 0 && size.height > 0)
		{
			this->size = size;
			data.clear();
			data.resize(size.width);
			for (Type::Size::Type i = 0; i < size.width; i++)
				data[i].resize(size.height);
		}
	}


	public: template<typename F> constexpr void foreach(F &f)
	{
		Type::Coord coord = {0};
		auto x_ = &data.front();
		for (coord.x = 0; coord.x < size.width; ++coord.x, ++x_)
		{
			auto y_ = &x_->front();
			for (coord.y = 0; coord.y < size.height; ++coord.y, ++y_)
			{
				f(coord, *y_);
			}
		}
	}
	public: template<typename F> constexpr void foreach(F &f) const
	{
		Type::Coord coord = {0};
		auto x_ = &data.front();
		for (coord.x = 0; coord.x < size.width; ++coord.x, ++x_)
		{
			auto y_ = &x_->front();
			for (coord.y = 0; coord.y < size.height; ++coord.y, ++y_)
			{
				f(coord, *y_);
			}
		}
	}
	public: template<typename F> constexpr void reverse_foreach(F &f)
	{
		Type::Coord coord = {0};
		auto x_ = &data.back();
		for (coord.x = size.width - 1; coord.x >= 0; --coord.x, --x_)
		{
			auto y_ = &x_->back();
			for (coord.y = size.height - 1; coord.y >= 0; --coord.y, --y_)
			{
				f(coord, *y_);
			}
		}
	}
	public: template<typename F> constexpr void forrange(Type::Coord start, Type::Coord end, F &f)
	{
		Type::Coord coord = {0};
		auto x_ = &data.front() + start.x;
		for (coord.x = start.x; coord.x < end.x; ++coord.x, ++x_)
		{
			auto y_ = &x_->front() + start.y;
			for (coord.y = start.y; coord.y < end.y; ++coord.y, ++y_)
			{
				f(coord, *y_);
			}
		}
	}
};
