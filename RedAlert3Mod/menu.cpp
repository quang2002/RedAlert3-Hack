#include "menu.h"

void ra3::cmenu::render(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice << directx::rectangle_t(20, 20, 100, 100, 0x80404040, directx::eColor::Red, 5);
	pDevice << directx::circle_t(200, 200, 100, directx::eColor::Blue, directx::eColor::Red, 5);
}


void ra3::cmenu::assign(main::fnPresent_t& mPresent)
{
	mPresent = ra3::cmenu::Present;
}

HRESULT RA3XAPI ra3::cmenu::Present(const LPDIRECT3DDEVICE9 pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	__asm pushad;

	render(pDevice);

	__asm popad;
	return ra3::main::oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}