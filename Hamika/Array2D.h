#pragma once

#include <vector>
#include "Types.h"

template<class Data>
class Array2D
{
	private: Type::Size size = {0,0};

	protected: std::vector<std::vector<Data>> data;

	public: inline operator Type::Size() const
	{
		return size;
	}
	public: inline Data &operator[](Type::Coord coord)
	{
#ifdef DEBUG
		if (coord.x >= size.width || coord.y >= size.height || coord.x < 0 || coord.y < 0)
			clog << KIR4::LRED << "Coord ERROR!: " << coord << KIR4::eol << KIR4::pp;
#endif // DEBUG
		return data[coord.x][coord.y];
	}
	public: inline const Data &operator[](Type::Coord coord) const
	{
#ifdef DEBUG
		if (coord.x >= size.width || coord.y >= size.height || coord.x < 0 || coord.y < 0)
			clog << KIR4::LRED << "Coord ERROR!: " << coord << KIR4::eol << KIR4::pp;
#endif // DEBUG
		return data[coord.x][coord.y];
	} 

	public: Array2D(Type::Size size)
	{
		if (size.width > 0 && size.height > 0)
		{
			this->size = size;

			data.resize(size.width);
			for (Type::Size::Type i = 0; i < size.width; i++)
				data[i].resize(size.height);
		}
	}
	public: Array2D()
	{

	}
	public: Array2D(Array2D &&array2D):
		size(array2D.size),
		data(std::move(array2D.data))
	{

	}
	public: virtual ~Array2D()
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


	public: template<typename F> inline void foreach(F &f)
	{
		Type::Coord coord = {0};
		for (coord.x = 0; coord.x < size.width; ++coord.x)
		{
			for (coord.y = 0; coord.y < size.height; ++coord.y)
			{
				f(coord, (*this)[coord]);
			}
		}
	}
	public: template<typename F> inline void reverse_foreach(F &f)
	{
		Type::Coord coord = {0};
		for (coord.x = size.width - 1; coord.x >= 0; --coord.x)
		{
			for (coord.y = size.height - 1; coord.y >= 0; --coord.y)
			{
				f(coord, (*this)[coord]);
			}
		}
	}
	public: template<typename F> inline void forrange(Type::Coord start, Type::Coord end, F &f)
	{
		Type::Coord coord = {0};
		for (coord.x = start.x; coord.x < end.x; ++coord.x)
		{
			for (coord.y = start.y; coord.y < end.y; ++coord.y)
			{
				f(coord, (*this)[coord]);
			}
		}
	}
};
