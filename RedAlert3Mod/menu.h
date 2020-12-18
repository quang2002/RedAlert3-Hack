#ifndef __MENU_H__
#define __MENU_H__


#pragma once
#include "stdafx.h"
#include "ra3.h"
#include "drawing.h"

namespace ra3
{
	class cmenu
	{
	public:
		static void render(CONST LPDIRECT3DDEVICE9 pDevice);

		static void assign(main::fnPresent_t& mPresent);

		static HRESULT RA3XAPI Present(CONST LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	};
}




#endif // !__MENU_H__
