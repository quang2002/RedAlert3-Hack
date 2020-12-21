#ifndef __DRAWING_H__
#define __DRAWING_H__

#pragma once
#include "stdafx.h"

namespace directx
{
	enum eColor
	{
		None,
		White = 0xFFFFFFFF,
		Black = 0xFF000000,
		Red = 0xFFFF0000,
		Green = 0xFF00FF00,
		Blue = 0xFF0000FF
	};

	class rectangle_t
	{
		std::uint32_t x, y, w, h, thickness;
		D3DCOLOR color, border_color;
	public:
		rectangle_t();
		rectangle_t(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h, D3DCOLOR color, D3DCOLOR border_color = eColor::None, std::uint32_t thickness = 1);
		rectangle_t(D3DRECT rect, D3DCOLOR color, D3DCOLOR border_color = eColor::None, std::uint32_t thickness = 1);

		void set_position(std::uint32_t x = -1, std::uint32_t y = -1);
		void set_size(std::uint32_t w = -1, std::uint32_t h = -1);
		void set_color(D3DCOLOR color = eColor::None, D3DCOLOR border_color = eColor::None);
		void set_thickness(std::uint32_t thickness);

		static void fill(LPDIRECT3DDEVICE9 device, std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h, D3DCOLOR color);
		static void border(LPDIRECT3DDEVICE9 device, std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h, std::uint32_t thickness, D3DCOLOR color);

		friend LPDIRECT3DDEVICE9 operator << (LPDIRECT3DDEVICE9 device, const rectangle_t& rect);
	};

	class circle_t
	{
		std::uint32_t x, y, radius, thickness;
		D3DCOLOR color, border_color;
	public:
		circle_t();
		circle_t(std::uint32_t x, std::uint32_t y, std::uint32_t radius, D3DCOLOR color, D3DCOLOR border_color = eColor::None, std::uint32_t thickness = 1);

		void set_position(std::uint32_t x, std::uint32_t y);
		void set_radius(std::uint32_t radius);

		friend LPDIRECT3DDEVICE9 operator << (LPDIRECT3DDEVICE9 device, const circle_t& circle);
	};
}

#endif // !__DRAWING_H__
