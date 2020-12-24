#include "menu.h"

HHOOK ra3::render_t::menu_t::hhook = NULL;

std::vector<ra3::render_t::menu_t*> menu_ins;

directx::font_t consolas("consolas", 14, 6, FW_NORMAL, FALSE);

struct {
	std::uint32_t unlimitedCoin, unlimitedEnergy;
}vars;

void ra3::render_t::render(const LPDIRECT3DDEVICE9 pDevice)
{
	consolas.init(pDevice);

	static bool isShow = false;
	static menu_t* menu = new menu_t(20, 20, 300, 0, 20, 5);
	if (menu->items.size() == 0)
	{
		menu->add("Unlimited Coin", &vars.unlimitedCoin, { "OFF", "ON" });
		menu->add("Unlimited Energy", &vars.unlimitedEnergy, { "OFF", "ON" });
	}

	if (vars.unlimitedCoin)
	{
		std::size_t(__thiscall * sub_00848170)(DWORD*) = decltype(sub_00848170)(0x00848170);
		std::size_t(__thiscall * sub_0084AA90)(DWORD*, UINT) = decltype(sub_0084AA90)(0x0084AA90);

		DWORD* ptr_this = *(DWORD**)(ra3::main::vars.game + 0x8FC734);
		ptr_this = (DWORD*)sub_00848170(ptr_this);
		ptr_this = (DWORD*)sub_0084AA90(ptr_this, NULL);
		if (ptr_this)
		{
			*(ptr_this + 1) = 99999999;
		}
	}

	if (vars.unlimitedEnergy)
	{
		DWORD* ptr_this = *(DWORD**)(ra3::main::vars.game + 0x8FC734);

		auto load_ptr = [](DWORD base, std::vector<DWORD> offsets) -> DWORD*
		{
			for (const auto& offset : offsets)
			{
				if (&offset == offsets.end()._Ptr) return (DWORD*)(base + offset);
				base = *(DWORD*)(base + offset);
				if (!base) return NULL;
			}
		};

		ptr_this = load_ptr((DWORD)ptr_this, { 0x28, 0xC, 0x6C, 0x4 });
		*ptr_this = 9999999;
	}

	if (GetAsyncKeyState(VK_F4) & 1) isShow = !isShow;
	if (isShow) menu->render(pDevice);
}

void ra3::render_t::assign(main::fnPresent_t& mPresent)
{
	mPresent = ra3::render_t::Present;
}

HRESULT RA3XAPI ra3::render_t::Present(const LPDIRECT3DDEVICE9 pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	__asm pushad;

	render(pDevice);

	__asm popad;
	return ra3::main::oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

ra3::render_t::menu_t::menu_t()
{
	init(0, 0, 0, 0, 0, 0);
	menu_ins.push_back(this);
}

ra3::render_t::menu_t::menu_t(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h, std::uint32_t item_height, std::uint32_t item_offset)
{
	init(x, y, w, h, item_height, item_offset);
	menu_ins.push_back(this);
}

ra3::render_t::menu_t::~menu_t()
{
	std::size_t index = 0;
	for (const auto& menu : menu_ins)
	{
		if (menu == this) break;
		index++;
	}
	menu_ins.erase(menu_ins.begin() + index);
}

void ra3::render_t::menu_t::add(std::string text, std::uint32_t* pchoice, std::vector<std::string> statements)
{
	items.push_back({ text, pchoice, statements });
}

void ra3::render_t::menu_t::del(std::uint32_t index)
{
	items.erase(items.begin() + index);
}

void ra3::render_t::menu_t::init(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h, std::uint32_t item_height, std::uint32_t item_offset)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->item_height = item_height;
	this->item_offset = item_offset;

	//if (!hhook)
	//	hhook = SetWindowsHookEx(WH_MOUSE_LL, MSHookCallback, GetModuleHandle(NULL), 0);
}

void ra3::render_t::menu_t::render(const LPDIRECT3DDEVICE9 pDevice)
{
	using namespace directx;
	h = items.size() * (item_height + item_offset) + item_offset;
	pDevice << rectangle_t(x, y, w, h, 0x40404040, eColor::White);

	POINT mpos;
	GetCursorPos(&mpos);
	ScreenToClient(GetForegroundWindow(), &mpos);

	auto inArea = [&mpos](std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h) {
		return mpos.x >= x && mpos.x <= x + w && mpos.y >= y && mpos.y <= y + h;
	};

	std::size_t index = 0;
	for (const auto& item : items)
	{
		std::uint32_t item_y = index * (item_height + item_offset);
		if (inArea(x + 5, y + item_offset + item_y, w - 10, item_height))
		{
			pDevice << rectangle_t(x + 5, y + item_offset + item_y, w - 10, item_height, 0x20FFFFFF, eColor::Blue);
			if (GetAsyncKeyState(VK_LBUTTON) & 1 && GetAsyncKeyState(VK_LBUTTON) < 0)
			{
				++(*item.pchoice);
				if (*item.pchoice == item.statements.size()) *item.pchoice = 0;
			}
		}
		else pDevice << rectangle_t(x + 5, y + item_offset + item_y, w - 10, item_height, 0x20FFFFFF, eColor::Green);
		consolas.write(x + 10, y + item_offset * 1.5f + item_y, eColor::White, DT_LEFT | DT_NOCLIP, item.text);
		consolas.write(x + w - 10, y + item_offset * 1.5f + item_y, eColor::White, DT_RIGHT | DT_NOCLIP, item.statements[*item.pchoice]);
		++index;
	}
}

LRESULT ra3::render_t::menu_t::MSHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
	POINT const& pt = p->pt;

	static auto inArea = [&pt](ra3::render_t::menu_t* menu) -> bool
	{
		return pt.x >= menu->x && pt.x <= menu->x + menu->w && pt.y >= menu->y && pt.y <= menu->y + menu->h;
	};

	for (const auto& menu : menu_ins)
	{
		if (inArea(menu) && GetAsyncKeyState(VK_LBUTTON))
		{
			MessageBox(0, L"inArea", 0, 0);
			return 0;
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
