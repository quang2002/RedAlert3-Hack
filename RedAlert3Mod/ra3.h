#ifndef __RA3_H__
#define __RA3_H__

#pragma once
#include "stdafx.h"

#define RA3XAPI __stdcall

#define __$GameModuleName__ ("ra3ep1_1.0.game")
#define __$D3D9ModuleName__ ("d3d9.dll")
#define __$LoggerPath__ (".\\ra3x.log")

namespace ra3
{
	class main
	{
	public:
		static DWORD RA3XAPI engine(LPVOID);

		static BOOL RA3XAPI process();

		static BOOL RA3XAPI initvars();

		static struct {
			size_t game, d3d9;
		} vars;

		typedef HRESULT(RA3XAPI* fnPresent_t)(CONST LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

		static logger_t logger;
		static fnPresent_t oPresent, mPresent;
	};
}

#endif // !__RA3_H__
