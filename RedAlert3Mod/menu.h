#ifndef __MENU_H__
#define __MENU_H__


#pragma once
#include "stdafx.h"
#include "ra3.h"
#include "drawing.h"

namespace ra3
{
	class render_t
	{
	public:
		class menu_t;

		static void render(CONST LPDIRECT3DDEVICE9 pDevice);

		static void assign(main::fnPresent_t& mPresent);

		static HRESULT RA3XAPI Present(CONST LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	};

	class render_t::menu_t
	{
	public:
		menu_t();
		menu_t(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h, std::uint32_t item_height, std::uint32_t item_offset);

		~menu_t();

		struct item_t
		{
			std::string text;
			std::uint32_t* pchoice;
			std::vector<std::string> statements;
		};

		void add(std::string text, std::uint32_t* pchoice, std::vector<std::string> statements);
		void del(std::uint32_t index);

		void init(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h, std::uint32_t item_height, std::uint32_t item_offset);

		void render(CONST LPDIRECT3DDEVICE9 pDevice);

		static LRESULT CALLBACK MSHookCallback(int nCode, WPARAM wParam, LPARAM lParam);
	
		std::uint32_t x, y, w, h;
		std::uint32_t item_height, item_offset;
		std::vector<item_t> items;
		static HHOOK hhook;
	};
}




#endif // !__MENU_H__
